#include <rack.hpp>
#include "widget/SvgWidget.hpp"
#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscOutboundPacketStream.h"

namespace URack {

struct Dispatcher {

	static Dispatcher instance;
	std::vector<UdpTransmitSocket*> transmitSockets;

	static void create();
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
