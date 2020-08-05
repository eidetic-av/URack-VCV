#pragma once

#include <functional>

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
    std::vector<OscArg> args;
    bool isQuery;
    std::function<void(void*, std::vector<std::string>)> queryFunctor;
    void* moduleInstance;
};

struct Dispatcher {
    static bool instantiated;

    static std::vector<SocketInfo *> sockets;
    static std::vector<OscUpdate *> updateQueue;

    static int connect_host(std::string hostIp = LOCALHOST,
                            int hostPort = SENDPORT);
    static void disconnect_host(int host);

    static void send(std::vector<int> hosts, std::string address, float value);
    static void send(std::vector<int> hosts, std::string address,
                     std::vector<OscArg> args);
    static void send(int host, std::string address, float value);
    static void send(int host, std::string address, std::vector<OscArg> args);

    static void query(int host, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance);
    static void query(std::vector<int> hosts, std::string address, std::function<void(void*, std::vector<std::string>)> functor, void* instance);

    static void dispatchUpdates();

    std::thread oscDispatcherThread;

    Dispatcher() {
        // start the osc update thread
        oscDispatcherThread = std::thread(&dispatchUpdates);
        oscDispatcherThread.detach();
        instantiated = true;
    }
};

struct QueryResponse {
    std::string responderIp;
    std::string address;
    std::function<void(void*, std::vector<std::string>)> functor;
    void* instance;
};

struct Listener {
    static bool initialised;

    static std::thread oscListenerThread;
    static UdpListeningReceiveSocket *receiveSocket;

    static std::vector<QueryResponse*> queryResponseQueue;

    class PacketListener : public osc::OscPacketListener {
        virtual void
        ProcessMessage(const osc::ReceivedMessage &msg,
                       const IpEndpointName &remoteEndpoint) override;
    };

    static PacketListener *oscPacketListener;

    static void create(int listenPort = LISTENPORT);
};


} // namespace URack
