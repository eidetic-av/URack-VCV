#include "URack.hpp"

#include <initializer_list>
#include <iterator>

using namespace rack::logger;

namespace URack {

std::vector<SocketInfo> Dispatcher::sockets = {};

int Dispatcher::create(const char* hostIp, int hostPort) {
	// instantiate the dispatcher
	instance = Dispatcher();

	int index = sockets.size();

	sockets.push_back({new UdpTransmitSocket(IpEndpointName(hostIp, hostPort)),
			hostIp, hostPort});

	char buffer[UDP_BUFFER_SIZE];
	osc::OutboundPacketStream p(buffer, UDP_BUFFER_SIZE);
	p << osc::BeginMessage("/Dispatcher/Create") << hostIp << hostPort
		<< osc::EndMessage;
	sockets[index].transmitSocket->Send(p.Data(), p.Size());

	std::string msg =
		"constructed dispatcher " + std::to_string(index) + " at ";
	msg += hostIp;
	msg += ":";
	msg += std::to_string(hostPort);

	DEBUG("%s", msg.c_str());

	return index;
}

void Dispatcher::destroy(int host) {}

void Dispatcher::send(int hostNum, std::string address, float value) {
	Dispatcher::send(hostNum, address, std::vector<OscArg>{value});
}

void Dispatcher::send(int hostNum, std::string address,
		std::vector<OscArg> args) {
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
	sockets[hostNum].transmitSocket->Send(p.Data(), p.Size());
}
}  // namespace URack
