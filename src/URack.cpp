#include "URack.hpp"

#include <initializer_list>

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

void Dispatcher::send(int hostNum, std::string address, OscArg args[],
		unsigned int argLength) {
	char buffer[UDP_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
	p << osc::BeginMessage(address.c_str());
	for (unsigned int i = 0; i < argLength; i++) {
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
	transmitSockets[hostNum]->Send(p.Data(), p.Size());
}
}  // namespace URack
