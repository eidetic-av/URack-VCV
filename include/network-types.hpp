#pragma once

#include <functional>
#include <initializer_list>
#include <iterator>
#include <mutex>
#include <tuple>

#include "constants.hpp"

using namespace rack;
using namespace rack::logger;

namespace URack {

struct OscArg {
    enum type { Int, Float, String };
    OscArg(int e) {
        m_data.INT = e;
        m_type = Int;
    }
    OscArg(float e) {
        m_data.FLOAT = e;
        m_type = Float;
    }
    OscArg(const char *e) {
        m_data.STRING = e;
        m_type = String;
    }
    type get_type() const { return m_type; }
    int get_int() const { return m_data.INT; }
    float get_float() const { return m_data.FLOAT; }
    const char *get_string() const { return m_data.STRING; }

    type m_type;
    union {
        int INT;
        float FLOAT;
        const char *STRING;
    } m_data;
};

struct SocketInfo {
    UdpTransmitSocket *transmitSocket;
    std::string ip;
    int port;
    int hostNum;
};

struct OscUpdate {
    int hostNum;
    std::string address;
    bool isValue;
    std::vector<OscArg> args;
    bool isQuery;
    std::function<void(void*, std::vector<std::string>)> queryFunctor;
    void* moduleInstance;
    bool isAction;
    std::string actionArgument;
};

struct QueryResponse {
    std::string responderIp;
    std::string address;
    std::function<void(void*, std::vector<std::string>)> functor;
    void* instance;
};

struct Dispatcher {
    std::vector<SocketInfo *> sockets;
    std::vector<OscUpdate *> updateQueue;

    std::mutex updateMutex;

    std::vector<QueryResponse*>* queryResponseQueue;

    int connect_host(std::string hostIp = LOCALHOST, int hostPort = SENDPORT) {
        int index = sockets.size();

        auto socketInfo = new SocketInfo;
        socketInfo->transmitSocket =
            new UdpTransmitSocket(IpEndpointName(hostIp.c_str(), hostPort));
        socketInfo->ip = hostIp;
        socketInfo->port = hostPort;
        socketInfo->hostNum = index;

        sockets.push_back(socketInfo);

        char buffer[UDP_BUFFER_SIZE];
        osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
        p << osc::BeginMessage("/Dispatcher/Create") << hostIp.c_str() << hostPort
          << osc::EndMessage;
        sockets[index]->transmitSocket->Send(p.Data(), p.Size());

        std::string msg = "Added host entry " + std::to_string(index) + " at ";
        msg += hostIp;
        msg += ":";
        msg += std::to_string(hostPort);
        DEBUG("%s", msg.c_str());

        return index;
    }

    void send(std::vector<int> hosts, std::string address, float value) {
        for (int host : hosts)
            send(host, address, value);
    }

    void send(std::vector<int> hosts, std::string address, std::vector<OscArg> value) {
        for (int host : hosts)
            send(host, address, value);
    }

    void send(int hostNum, std::string address, float value) {
        send(hostNum, address, std::vector<OscArg>{value});
    }

    void send(int hostNum, std::string address, std::vector<OscArg> args) {
        std::lock_guard<std::mutex> guard(updateMutex);
        auto update = new OscUpdate;
        update->hostNum = hostNum;
        update->address = address;
        update->args = args;
        update->isValue = true;
        update->isQuery = false;
        update->isAction = false;
        updateQueue.push_back(update);
    }

    void query(std::vector<int> hosts, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance) {
        for (int host : hosts)
            query(host, address, functor, instance);
    }

    void query(int hostNum, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance) {
        std::lock_guard<std::mutex> guard(updateMutex);
        auto update = new OscUpdate;
        update->hostNum = hostNum;
        update->address = address;
        update->isValue = false;
        update->isQuery = true;
        update->isAction = false;
        update->queryFunctor = functor;
        update->moduleInstance = instance;
        updateQueue.push_back(update);
    }

    void action(std::vector<int> hosts, std::string address, std::string argument) {
        for (int host : hosts)
            action(host, address, argument);
    }

    void action(int hostNum, std::string address, std::string argument) {
        std::lock_guard<std::mutex> guard(updateMutex);
        auto update = new OscUpdate;
        update->hostNum = hostNum;
        update->address = address;
        update->isValue = false;
        update->isQuery = false;
        update->isAction = true;
        update->actionArgument = argument;
        updateQueue.push_back(update);
    }

    static void dispatchUpdates(Dispatcher* dispatcher) {
        while (true) {
            // send OSC updates at 1000Hz
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::lock_guard<std::mutex> guard(dispatcher->updateMutex);

            for (auto update : dispatcher->updateQueue) {
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
                    if (hostNum < (int)dispatcher->sockets.size())
                        dispatcher->sockets[hostNum]->transmitSocket->Send(p.Data(), p.Size());

                } else if (update->isQuery) {
                    // querys
                    char buffer[UDP_BUFFER_SIZE];
                    osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
                    p << osc::BeginMessage(address.c_str());
                    p << "Query";
                    p << osc::EndMessage;
                    if (hostNum < (int)dispatcher->sockets.size())
                        dispatcher->sockets[hostNum]->transmitSocket->Send(p.Data(), p.Size());
                    // and queue the callback when we receive the response
                    auto response = new QueryResponse;
                    response->responderIp = dispatcher->sockets[hostNum]->ip;
                    response->address = address;
                    response->functor = update->queryFunctor;
                    response->instance = update->moduleInstance;
                    dispatcher->queryResponseQueue->push_back(response);
                } else if (update->isAction) {
                    // actions are like queries that don't wait for a response
                    char buffer[UDP_BUFFER_SIZE];
                    osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
                    p << osc::BeginMessage(address.c_str());
                    p << "Action";
                    p << update->actionArgument.c_str();
                    p << osc::EndMessage;
                    if (hostNum < (int)dispatcher->sockets.size())
                        dispatcher->sockets[hostNum]->transmitSocket->Send(p.Data(), p.Size());
                }
            }
            dispatcher->updateQueue.clear();
        }
    }
};

struct Listener {
    std::vector<QueryResponse*>* queryResponseQueue;

    class PacketListener : public osc::OscPacketListener {
            virtual void ProcessMessage(const osc::ReceivedMessage &msg,
                                        const IpEndpointName &remoteEndpoint) override;
    };
};

struct NetworkManager {
    Dispatcher *dispatcher;
    Listener *listener;
    std::thread dispatcherThread;
    std::thread listenerThread;
    bool initialised = false;

    std::vector<QueryResponse*> queryResponseQueue;

    NetworkManager() {
        //start the osc update thread
        dispatcher = new Dispatcher;
        dispatcher->queryResponseQueue = &queryResponseQueue;
        dispatcherThread = std::thread(&Dispatcher::dispatchUpdates, dispatcher);
        dispatcherThread.detach();
        // start the listener thread
        listener = new Listener;
        listener->queryResponseQueue = &queryResponseQueue;
        auto oscPacketListener = new Listener::PacketListener();
        auto receiveSocket = new UdpListeningReceiveSocket(
            IpEndpointName(IpEndpointName::ANY_ADDRESS, LISTENPORT), oscPacketListener);
        listenerThread = std::thread(&UdpListeningReceiveSocket::Run, receiveSocket);
        listenerThread.detach();
        initialised = true;
    }
};

extern NetworkManager* networkManager;

} // namespace URack
