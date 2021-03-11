#include "urack-module.hpp"

namespace URack {

// pointers that reference objects shared by all UModules
URack::NetworkManager *networkManager = NULL;
URack::Settings *settings = NULL;
std::map<std::string, URack::UModule *> *UModule::moduleInstances = NULL;
std::vector<int> *defaultHosts = NULL;

// static variable with an instance per plugin
bool URack::Settings::initialised = false;

// osc Packet Listener thread function implementation
// TODO I can't figure out how to put this into a header since it overrides
// a virtual function
void Listener::PacketListener::ProcessMessage(
    const osc::ReceivedMessage &msg, const IpEndpointName &remoteEndpoint) {
  std::string addressString = msg.AddressPattern();
  addressString.erase(0, 1);
  // create a list of the Osc Address elements
  std::vector<std::string> address;

  DEBUG(addressString.c_str());

  char delimiter = '/';
  std::string tmp = "";
  std::istringstream iss(addressString);
  while (std::getline(iss, tmp, delimiter))
    if (!tmp.empty())
      address.push_back(tmp);

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
    for (auto const &moduleKvp : *UModule::moduleInstances) {
      auto module = moduleKvp.second;
      for (int const &activeHostNum : module->activeHosts) {
        SocketInfo *activeSocket =
            URack::networkManager->dispatcher->sockets[activeHostNum];
        if (activeSocket == NULL)
          continue;
        std::string activeSocketIp = activeSocket->ip;
        if (activeSocketIp == ipString) {
          module->deactivateHost(ipString);
          module->activateHost(ipString);
        }
      }
    }
  } else if (address[0] == "QueryConnections") {
    // this is sent when a module is loaded in the URack Player to see
    // if there are any connections it has not yet accounted for
    auto args = msg.ArgumentStream();
    const char *queriedInstanceAddress;
    args >> queriedInstanceAddress;
    std::string instanceAddress = queriedInstanceAddress;
    UModule::moduleInstances->operator[](instanceAddress)->sendConnections();
  } else if (address[0] == "Instance") {
    // this is an update for an instance parameter
    std::string instanceAddress =
        address[0] + "/" + address[1] + "/" + address[2];
    std::string parameterName = address[3];
    if (UModule::moduleInstances->count(instanceAddress) == 1) {
      UModule *moduleInstance =
          UModule::moduleInstances->operator[](instanceAddress);
      int outputNum = moduleInstance->listenerOutputs[parameterName];
      auto args = msg.ArgumentStream();
      float voltage;
      args >> voltage;
      moduleInstance->setVoltage(outputNum, voltage);
    }
  } else {
    // if it's none of the above, loop through the queryResponseQueue
    // and perform any query callbacks from the results we get
    std::vector<int> completedResponses;
    for (uint i = 0; i < URack::networkManager->queryResponseQueue.size();
         i++) {
      auto query = URack::networkManager->queryResponseQueue[i];
      if (query->address.find(addressString) != std::string::npos) {
        auto arg = msg.ArgumentsBegin();
        std::string message = (arg++)->AsString();
        std::string delimiter = ";";
        // the first part of the message is the responder ip
        auto responseIp = message.substr(0, message.find(delimiter));
        if (responseIp != query->responderIp)
          continue;
        message.erase(0, message.find(delimiter) + delimiter.length());
        // at the moment, all results are in the form of a string vector
        std::vector<std::string> results;
        size_t pos = 0;
        while ((pos = message.find(delimiter)) != std::string::npos) {
          results.push_back(message.substr(0, pos));
          message.erase(0, pos + delimiter.length());
        }
        query->functor(query->instance, results);
        completedResponses.push_back(i);
      }
      delete query;
    }
    // stop checking responses that we've already called the functor for
    for (uint i = 0; i < completedResponses.size(); i++) {
      int idx = completedResponses[i] - i;
      URack::networkManager->queryResponseQueue.erase(
          URack::networkManager->queryResponseQueue.begin() + idx);
    }
  }
};
} // namespace URack
