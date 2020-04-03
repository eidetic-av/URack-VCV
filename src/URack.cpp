#include "URack.hpp"

#include <initializer_list>
#include <iterator>
#include <mutex>
#include <tuple>

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
        }
        updateQueue.clear();
    }
}

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
}

void Settings::load() {
    std::string settingsFilename = asset::user("Eidetic-URack.json");
    FILE *file = fopen(settingsFilename.c_str(), "r");
    if (!file)
        return;
    DEFER({ fclose(file); });

    json_error_t error;
    json_t *settings = json_loadf(file, 0, &error);
    if (!settings)
        return;

    std::map<int, std::tuple<std::string, int>> hosts;
    json_t *hostsJ = json_object_get(settings, "hosts");
    json_t *host;
    int hi = 0;
    while ((host = json_array_get(hostsJ, hi++)) != NULL) {
        int hostNum = json_integer_value(json_object_get(host, "hostNum"));
        int port = json_integer_value(json_object_get(host, "port"));
        std::string ip = json_string_value(json_object_get(host, "ip"));
        hosts[hostNum] = {ip, port};
    }
    for (unsigned int i = 0; i < hosts.size(); i++)
        Dispatcher::connect_host(std::get<0>(hosts[i]), std::get<1>(hosts[i]));
}

void Settings::save() {
    // save host ip entries
    // to plugin settings json

    json_t *settings = json_object();
    json_t *hosts = json_array();
    for (auto socketInfo : Dispatcher::sockets) {
        json_t *hostOption = json_object();
        json_object_set_new(hostOption, "hostNum",
                            json_integer(socketInfo->hostNum));
        json_object_set_new(hostOption, "ip",
                            json_string(socketInfo->ip.c_str()));
        json_object_set_new(hostOption, "port", json_integer(socketInfo->port));
        json_array_append_new(hosts, hostOption);
    }
    json_object_set_new(settings, "hosts", hosts);

    std::string settingsFilename = asset::user("Eidetic-URack.json");
    FILE *file = fopen(settingsFilename.c_str(), "w");
    if (file) {
        json_dumpf(settings, file, JSON_INDENT(2));
        fclose(file);
    }

    json_decref(settings);
}
} // namespace URack
