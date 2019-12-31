#include <rack.hpp>
#include "widget/SvgWidget.hpp"
#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscOutboundPacketStream.h"

#define LOCALHOST "127.0.0.1"
#define PORT 54321
#define UDP_BUFFER_SIZE 1024

namespace URack {

struct Dispatcher {
	static Dispatcher instance;
	static std::vector<UdpTransmitSocket*> transmitSockets;

	static void create();
	static void destroy();

	Dispatcher() {}
};

// Custom type ports

struct PointCloudPort {

};

struct PointCloudInput : PointCloudPort { };
struct PointCloudOutput : PointCloudPort { };

struct PointCloudPortWidget : rack::widget::SvgWidget {

	PointCloudPortWidget();

};

}
