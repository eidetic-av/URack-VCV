#include "UModule.hpp"

namespace URack {

    std::map<std::string, URack::UModule*> URack::UModule::instances;
    int UModule::defaultHostNum = 0;

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

        if (address[0] == "Instance") {
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
