#include "urack.hpp"

#include <initializer_list>
#include <iterator>
#include <mutex>
#include <tuple>

using namespace rack;
using namespace rack::logger;

namespace URack {

std::vector<SocketInfo *> Dispatcher::sockets;

bool Dispatcher::instantiated = false;

int Dispatcher::connect_host(std::string hostIp, int hostPort) {
    // if a Dispatcher doesn't exist on the first attempt at connection,
    // instantiate the dispatcher to start the OSC message updates
    if (!Dispatcher::instantiated)
        new Dispatcher();

    int index = Dispatcher::sockets.size();

    auto socketInfo = new SocketInfo;
    socketInfo->transmitSocket =
        new UdpTransmitSocket(IpEndpointName(hostIp.c_str(), hostPort));
    socketInfo->ip = hostIp;
    socketInfo->port = hostPort;
    socketInfo->hostNum = index;

    Dispatcher::sockets.push_back(socketInfo);

    char buffer[UDP_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
    p << osc::BeginMessage("/Dispatcher/Create") << hostIp.c_str() << hostPort
      << osc::EndMessage;
    Dispatcher::sockets[index]->transmitSocket->Send(p.Data(), p.Size());

    std::string msg = "Added host entry " + std::to_string(index) + " at ";
    msg += hostIp;
    msg += ":";
    msg += std::to_string(hostPort);
    DEBUG("%s", msg.c_str());

    Settings::save();

    return index;
}

void Dispatcher::disconnect_host(int host) {}

std::vector<OscUpdate *> Dispatcher::updateQueue;
std::mutex updateMutex;

std::vector<QueryResponse*> Listener::queryResponseQueue;

void Dispatcher::send(std::vector<int> hosts, std::string address,
                      float value) {
    for (int host : hosts)
        Dispatcher::send(host, address, value);
}

void Dispatcher::send(std::vector<int> hosts, std::string address,
                      std::vector<OscArg> value) {
    for (int host : hosts)
        Dispatcher::send(host, address, value);
}

void Dispatcher::send(int hostNum, std::string address, float value) {
    Dispatcher::send(hostNum, address, std::vector<OscArg>{value});
}

void Dispatcher::send(int hostNum, std::string address,
                      std::vector<OscArg> args) {
    std::lock_guard<std::mutex> guard(updateMutex);
    auto update = new OscUpdate;
    update->hostNum = hostNum;
    update->address = address;
    update->args = args;
    update->isValue = true;
    update->isQuery = false;
    update->isAction = false;
    Dispatcher::updateQueue.push_back(update);
}

void Dispatcher::query(std::vector<int> hosts, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance) {
    for (int host : hosts)
        Dispatcher::query(host, address, functor, instance);
}

void Dispatcher::query(int hostNum, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance) {
    std::lock_guard<std::mutex> guard(updateMutex);
    auto update = new OscUpdate;
    update->hostNum = hostNum;
    update->address = address;
    update->isValue = false;
    update->isQuery = true;
    update->isAction = false;
    update->queryFunctor = functor;
    update->moduleInstance = instance;
    Dispatcher::updateQueue.push_back(update);
}

void Dispatcher::action(std::vector<int> hosts, std::string address, std::string argument) {
    for (int host : hosts)
        Dispatcher::action(host, address, argument);
}

void Dispatcher::action(int hostNum, std::string address, std::string argument) {
    std::lock_guard<std::mutex> guard(updateMutex);
    auto update = new OscUpdate;
    update->hostNum = hostNum;
    update->address = address;
    update->isValue = false;
    update->isQuery = false;
    update->isAction = true;
    update->actionArgument = argument;
    Dispatcher::updateQueue.push_back(update);
}

void Dispatcher::dispatchUpdates() {
    while (true) {
        // send OSC updates at 1000Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::lock_guard<std::mutex> guard(updateMutex);

        for (auto update : updateQueue) {
            auto hostNum = update->hostNum;
            auto address = update->address;

            // regular value updates
            if (update->isValue) {
                auto args = update->args;
                char buffer[UDP_BUFFER_SIZE];
                osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
                p << osc::BeginMessage(address.c_str());
                for (unsigned int i = 0; i < args.size(); i++) {
                    switch (args[i].get_type()) {
                        case OscArg::Int:
                            p << args[i].get_int();
                            break;
                        case OscArg::Float:
                            p << args[i].get_float();
                            break;
                        case OscArg::String:
                            p << args[i].get_string();
                            break;
                    }
                }
                p << osc::EndMessage;
                if (hostNum < (int)Dispatcher::sockets.size())
                    Dispatcher::sockets[hostNum]->transmitSocket->Send(p.Data(),
                                                                       p.Size());
            } else if (update->isQuery) {
                // querys
                char buffer[UDP_BUFFER_SIZE];
                osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
                p << osc::BeginMessage(address.c_str());
                p << "Query";
                p << osc::EndMessage;
                if (hostNum < (int)Dispatcher::sockets.size())
                    Dispatcher::sockets[hostNum]->transmitSocket->Send(p.Data(),
                                                                       p.Size());
                // and queue the callback when we receive the response
                auto response = new QueryResponse;
                response->responderIp = Dispatcher::sockets[hostNum]->ip;
                response->address = address;
                response->functor = update->queryFunctor;
                response->instance = update->moduleInstance;
                Listener::queryResponseQueue.push_back(response);
            } else if (update->isAction) {
                // actions are like queries that don't wait for a response
                char buffer[UDP_BUFFER_SIZE];
                osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
                p << osc::BeginMessage(address.c_str());
                p << "Action";
                p << update->actionArgument.c_str();
                p << osc::EndMessage;
                if (hostNum < (int)Dispatcher::sockets.size())
                    Dispatcher::sockets[hostNum]->transmitSocket->Send(p.Data(),
                                                                       p.Size());
            }
        }
        updateQueue.clear();
    }
}

bool Listener::initialised = false;
UdpListeningReceiveSocket *Listener::receiveSocket = NULL;
std::thread Listener::oscListenerThread;
Listener::PacketListener *Listener::oscPacketListener;

void Listener::create(int listenPort) {
    Listener::oscPacketListener = new Listener::PacketListener();
    Listener::receiveSocket = new UdpListeningReceiveSocket(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, listenPort),
        oscPacketListener);
    Listener::oscListenerThread =
        std::thread(&UdpListeningReceiveSocket::Run, receiveSocket);
    oscListenerThread.detach();
    Listener::initialised = true;
}

} // namespace URack
