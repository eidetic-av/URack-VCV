#include <thread>

#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscOutboundPacketStream.h"
#include "../lib/oscpack/osc/OscPacketListener.h"
#include "plugin.hpp"

/* #define LOCALHOST "127.0.0.1" */
#define LOCALHOST "169.254.226.61"
#define SENDPORT 54321
#define LISTENPORT 54320
#define UDP_BUFFER_SIZE 1024

namespace URack {

const float EPSILON = std::numeric_limits<float>::epsilon();
const float OSC_UPDATE_PERIOD = 0.001f;

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
};

struct Dispatcher {
    static Dispatcher *instance;

    static void dispatch_updates(Dispatcher *instance);

    std::thread oscDispatcherThread;
    std::queue<OscUpdate *> updateQueue;

    static std::vector<SocketInfo *> sockets;

    static int connect_host(std::string hostIp = LOCALHOST,
                            int hostPort = SENDPORT);
    static void disconnect_host(int host);

    static void send(int host, std::string address, float value);
    static void send(int host, std::string address, std::vector<OscArg> args);

    Dispatcher() {
        instance = this;
        // start the osc update thread
        oscDispatcherThread = std::thread(&dispatch_updates, instance);
        oscDispatcherThread.detach();
    }
};

struct Listener {
    static std::thread oscListenerThread;
    static UdpListeningReceiveSocket *receiveSocket;

    class PacketListener : public osc::OscPacketListener {
        virtual void
        ProcessMessage(const osc::ReceivedMessage &msg,
                       const IpEndpointName &remoteEndpoint) override;
    };

    static PacketListener *oscPacketListener;

    static void create(int listenPort = LISTENPORT);
};

struct Settings {
    static void load();
    static void save();
};
} // namespace URack
