#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscOutboundPacketStream.h"
#include "plugin.hpp"

/* #define LOCALHOST "172.16.1.76" */
/* #define LOCALHOST "192.168.1.119" */
#define LOCALHOST "192.168.0.100"
/* #define LOCALHOST "169.254.206.226" */
/* #define LOCALHOST "127.0.0.1" */
#define PORT 54321
#define UDP_BUFFER_SIZE 1024

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
	OscArg(const char * e) {
		m_data.STRING = e;
		m_type = String;
	}
	type get_type() const { return m_type; }
	int get_int() const { return m_data.INT; }
	float get_float() const { return m_data.FLOAT; }
	const char * get_string() const { return m_data.STRING; }

	type m_type;
	union {
		int INT;
		float FLOAT;
		const char * STRING;
	} m_data;
};

struct Dispatcher {
	static Dispatcher instance;
	static std::vector<UdpTransmitSocket*> transmitSockets;

	static void create();

	static void send(int host, std::string address, OscArg args [], unsigned int argLength);

	Dispatcher() {}
};
}
