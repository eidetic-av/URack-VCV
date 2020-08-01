#include "urack-module.hpp"

namespace URack {

    std::map<std::string, URack::UModule*> URack::UModule::instances;
    std::vector<int> UModule::defaultHosts = { 0 };

    void Listener::PacketListener::ProcessMessage(
        const osc::ReceivedMessage& msg, const IpEndpointName& remoteEndpoint) {
        std::string addressString = msg.AddressPattern();
        addressString.erase(0, 1);
        // create a list of the Osc Address elements
        std::vector<std::string> address;

        char delimiter = '/';
        std::string tmp = "";
        std::istringstream iss(addressString);
        while (std::getline(iss, tmp, delimiter))
            if (!tmp.empty()) address.push_back(tmp);

        if (address[0] == "Initialise") {
            // this is a message sent when the URack player first starts up
            // so we use it to initialise a scene if a patch loaded in VCV
            // Rack before the URack Player started up

            // the only argument sent is the URack Player machine's IP(v4)
            auto args = msg.ArgumentStream();
            const char *ipAddress;
            args >> ipAddress;
            std::string ipString = ipAddress;
            // loop through each loaded module and re activate the host if
            // it's connected to the endpoint that sent the "Initialise"
            for (auto const& moduleInstance : UModule::instances) {
                UModule* module = moduleInstance.second;
                for (int const& activeHostNum : module->activeHosts) {
                    SocketInfo* activeSocket = Dispatcher::sockets[activeHostNum];
                    if (activeSocket == NULL) continue;
                    std::string activeSocketIp = activeSocket->ip;
                    if (activeSocketIp == ipString) {
                        module->deactivateHost(ipString);
                        module->activateHost(ipString);
                    }
                }
            }
        }
        else if (address[0] == "QueryConnections") {
            // this is sent when a module is loaded in the URack Player to see
            // if there are any connections it has not yet accounted for
            auto args = msg.ArgumentStream();
            const char *queriedInstanceAddress;
            args >> queriedInstanceAddress;
            std::string instanceAddress = queriedInstanceAddress;
            UModule::instances[instanceAddress]->sendConnections();
        }
        else if (address[0] == "Instance") {
            // this is an update for an instance parameter
            std::string instanceAddress =
                address[0] + "/" + address[1] + "/" + address[2];
            std::string parameterName = address[3];
            if (UModule::instances.count(instanceAddress) == 1) {
                UModule* moduleInstance = UModule::instances[instanceAddress];
                int outputNum = moduleInstance->listenerOutputs[parameterName];
                auto args = msg.ArgumentStream();
                float voltage;
                args >> voltage;
                moduleInstance->setVoltage(outputNum, voltage);
            }
        }
    };
}  // namespace URack
