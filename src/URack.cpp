#include "URack.hpp"

#include <rack.hpp>

using namespace rack::logger;

namespace URack {

std::vector<UdpTransmitSocket*> Dispatcher::transmitSockets = {};

void Dispatcher::create() {
	// instantiate the dispatcher
	instance = Dispatcher();

	// add a local socket by default
	transmitSockets.push_back(
			new UdpTransmitSocket(IpEndpointName(LOCALHOST, PORT)));

	char buffer[UDP_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
	p << osc::BeginMessage("/dispatcher/create") << LOCALHOST << PORT
		<< osc::EndMessage;
	transmitSockets[0]->Send(p.Data(), p.Size());

	DEBUG("%s", "Constructed a dispatcher instance");
}

}  // namespace URack
