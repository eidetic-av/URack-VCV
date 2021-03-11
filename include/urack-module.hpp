#pragma once

#include "dsp/digital.hpp"
#include "plugin.hpp"
#include "point-cloud.hpp"
#include "settings.hpp"

#include "urack.hpp"

#ifdef ARCH_WIN
#if !defined _WIN32_WINNT || _WIN32_WINNT < 0x0600
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#include <ws2tcpip.h>
#define dlsym(_handle, _symbol)                                                \
  reinterpret_cast<void *>(                                                    \
      GetProcAddress(static_cast<HINSTANCE>(_handle), _symbol))
#else
#include <arpa/inet.h>
#include <dlfcn.h>
#endif

namespace URack {

extern std::vector<int> *defaultHosts;

struct UModule : Module {

  static std::map<std::string, UModule *> *moduleInstances;

  std::vector<int> activeHosts = {};

  std::string instanceAddress;

  float updateTimer = -1;
  std::vector<float> lastParamValues;
  std::vector<float> lastInputVoltages;

  struct UpdateParam {
    std::string oscAddress;
    int param;
    int input;
    int atten;
  };

  std::vector<UpdateParam> updateParams;

  struct PointCloudPortInfo {
    int id;
    bool connectionStatus;
    PointCloudPort *port;
    std::vector<PointCloudPort *> connections;
  };

  std::vector<PointCloudPortInfo> pointCloudInputs;
  std::vector<PointCloudPortInfo> pointCloudOutputs;

  std::map<std::string, int> listenerOutputs;
  std::queue<std::tuple<int, float>> listenerUpdates;

  int activateParam = -1;
  int activateInput = -1;
  int activateLight = -1;
  dsp::BooleanTrigger activateTrigger;
  int active = 1;

  bool initialised = false;

  UModule() {
    // URack initialisation for the plugin.
    // URack::Settings::initialised is contained in this plugin only
    if (!URack::Settings::initialised) {
      DEBUG("Initialising global URack instance");
      // Find the URack-Collection plugin, and tell *this* plugin
      // to point its URack::settings and URack::networkManager
      // to the same pointer as the URack-Collection plugin...
      // So that network threads etc. are shared between all
      // URack plugins.
      for (auto plugin : rack::plugin::plugins) {
        if (plugin->slug == "URack-Collection") {
          DEBUG("Found URack-Collection...joining instance.");
          void *handle = plugin->handle;

          auto settingsSym = dlsym(handle, "baseSettings");
          auto settingsPtr = reinterpret_cast<URack::Settings **>(settingsSym);
          URack::settings = *settingsPtr;

          void *managerSym = dlsym(handle, "baseManager");
          auto managerPtr =
              reinterpret_cast<URack::NetworkManager **>(managerSym);
          URack::networkManager = *managerPtr;

          void *instanceListSym = dlsym(handle, "moduleInstanceList");
          auto instanceListPtr =
              reinterpret_cast<std::map<std::string, URack::UModule *> **>(
                  instanceListSym);
          UModule::moduleInstances = *instanceListPtr;

          void *defaultHostsSym = dlsym(handle, "defaultHostSelections");
          auto defaultHostsPtr =
              reinterpret_cast<std::vector<int> **>(defaultHostsSym);
          URack::defaultHosts = *defaultHostsPtr;

          break;
        }
      }

      if (URack::settings == nullptr) {
        DEBUG(std::string("Unable to find URack-Collection.").c_str());
        // TODO make this less harsh than exiting...
        exit(1);
      }

      // load URack-wide settings
      if (!URack::settings->hasLoaded)
        URack::settings->load();

      // if no default hosts loaded, select localhost
      if (URack::defaultHosts->size() == 0)
        URack::defaultHosts->push_back(0);

      URack::Settings::initialised = true;
    }
  }

  ~UModule() { URack::settings->save(); }

  void configActivate(int param, int light = -1, int input = -1) {
    activateParam = param;
    activateLight = light;
    activateInput = input;
    configParam(activateParam, 0.f, 1.f, 1.f, "Activate");
  }

  void configUpdate(int param, int input, std::string oscAddress) {
    configUpdate(param, input, -99, oscAddress);
  }

  void configUpdate(int param, int input, int atten, std::string oscAddress) {
    configUpdate(oscAddress, param, input, atten);
  }

  void configBiUpdate(std::string oscAddress, int param, int input = -1,
                      int atten = -1, float defaultValue = 0.f) {
    configUpdate(oscAddress, param, input, atten, defaultValue, -5.f, 5.f);
  }

  void configUpdate(std::string oscAddress, int param, int input = -1,
                    int atten = -1, float defaultValue = 0.f,
                    float minValue = 0.f, float maxValue = 10.f) {
    if (param > -1)
      configParam(param, minValue, maxValue, defaultValue, oscAddress);
    if (atten > -1)
      configParam(atten, -1.f, 1.f, 0.f, oscAddress);
    UpdateParam updateParam = {oscAddress, param, input, atten};
    updateParams.push_back(updateParam);
  }

  void configListener(std::string oscAddress, int param) {
    listenerOutputs[oscAddress] = param;
  }

  void activateHost(std::string ip, int port = SENDPORT) {
    bool existingHost = false;
    // if host already exists in Dispatcher, assign that
    for (unsigned int i = 0;
         i < URack::networkManager->dispatcher->sockets.size(); i++) {
      auto socket = URack::networkManager->dispatcher->sockets[i];
      if (socket->ip == ip && socket->port == port) {
        activeHosts.push_back(i);
        existingHost = true;
        break;
      }
    }
    // if the socket doesn't exist, open a new one
    if (!existingHost) {
      activeHosts.push_back(
          URack::networkManager->dispatcher->connect_host(ip, port));
      URack::settings->save();
    }

    forceNetworkUpdate();

    // update the default host list so that each new module
    // automatically connects to the same hosts as the last one set
    URack::defaultHosts->clear();
    for (auto host : activeHosts)
      URack::defaultHosts->push_back(host);
  }

  void deactivateHost(std::string ip, int port = SENDPORT) {
    for (unsigned int i = 0;
         i < URack::networkManager->dispatcher->sockets.size(); i++) {
      auto socket = URack::networkManager->dispatcher->sockets[i];
      if (socket->ip == ip && socket->port == port) {
        activeHosts.erase(std::remove(activeHosts.begin(), activeHosts.end(),
                                      socket->hostNum),
                          activeHosts.end());
        break;
      }
    }
  }

  void setVoltage(int output, float value) {
    listenerUpdates.push({output, value});
  }

  void onAdd() override {
    if (activeHosts.size() == 0)
      for (int host : *URack::defaultHosts)
        activateHost(URack::networkManager->dispatcher->sockets[host]->ip);

    instanceAddress = "Instance/" + model->slug + "/" + std::to_string(id);
    moduleInstances->operator[](instanceAddress) = this;
    std::vector<OscArg> args = {model->slug.c_str(), id};
    URack::networkManager->dispatcher->send(activeHosts, "Add", args);

    // initialize last value arrays
    for (unsigned int i = 0; i < inputs.size(); i++)
      lastInputVoltages.push_back(-99);
    for (unsigned int i = 0; i < params.size(); i++)
      lastParamValues.push_back(-99);
  }

  void onRemove() override {
    std::vector<OscArg> args = {model->slug.c_str(), id};
    URack::networkManager->dispatcher->send(activeHosts, "Remove", args);
  }

  void onReset() override {
    std::vector<OscArg> args = {model->slug.c_str(), id};
    URack::networkManager->dispatcher->send(activeHosts, "Reset", args);
    forceNetworkUpdate();
  }

  void forceNetworkUpdate() {
    // force voltages to be re-loaded
    if (lastInputVoltages.size() > 0)
      for (unsigned int i = 0; i < inputs.size(); i++)
        lastInputVoltages[i] = -99;
    if (lastParamValues.size() > 0)
      for (unsigned int i = 0; i < params.size(); i++)
        lastParamValues[i] = -99;
    // re-send active status
    URack::networkManager->dispatcher->send(
        activeHosts, instanceAddress + "/Active", active);
  }

  void sendConnections() {
    // send connected ports (useful when loading URack Player after VCV)
    for (auto const &output : pointCloudOutputs) {
      for (auto const &connection : output.connections) {
        // Dispatch connect message
        auto module = dynamic_cast<UModule *>(connection->module);
        std::vector<OscArg> update = {output.port->oscAddress.c_str(),
                                      module->id,
                                      connection->oscAddress.c_str()};
        std::string address = instanceAddress + "/Connect";
        URack::networkManager->dispatcher->send(activeHosts, address, update);
      }
    }
  }

  virtual void start() {}

  void process(const ProcessArgs &args) override {
    if (!initialised) {
      // any initialisation that needs to happen on first process step
      // send module active status
      URack::networkManager->dispatcher->send(
          activeHosts, instanceAddress + "/Active", active);
      if (activateParam != -1)
        params[activateParam].setValue(active);
      // callback for any start() overriden in the module
      start();
      initialised = true;
    }

    if (activateParam != -1) {
      bool oldActive = active;
      // if no activate input or it's not connected,
      // use the button to determine active status
      if (activateInput == -1 || !inputs[activateInput].isConnected()) {
        bool activeButton = params[activateParam].getValue() > 0;
        if (activateTrigger.process(activeButton))
          active = active > 0 ? 0 : 1;
      } else if (activateInput != -1 && inputs[activateInput].isConnected()) {
        // if we have a connected activate input, ignore the button
        float activeVoltage = inputs[activateInput].getVoltage();
        active = activeVoltage > 0 ? 1 : 0;
      }
      if (oldActive != active)
        URack::networkManager->dispatcher->send(
            activeHosts, instanceAddress + "/Active", active);
    }
    if (activateLight != -1)
      lights[activateLight].setBrightness(active * 10.f);

    // set any param updates received on the OSC listener
    while (listenerUpdates.size() > 0) {
      auto outputUpdate = listenerUpdates.front();
      int outputNum = std::get<0>(outputUpdate);
      float voltage = std::get<1>(outputUpdate);
      outputs[outputNum].setVoltage(voltage);
      listenerUpdates.pop();
    }

    update(args);

    if (active)
      updateTimer += args.sampleTime;

    if (updateTimer >= OSC_UPDATE_PERIOD) {
      // check for changed values
      // and send updates if necessary
      for (unsigned int i = 0; i < updateParams.size(); i++) {
        int param = updateParams[i].param;
        int input = updateParams[i].input;
        int atten = updateParams[i].atten;

        float value = param < 0 ? 0 : params[param].getValue();
        float voltage = input < 0 ? 0 : inputs[input].getVoltage();
        voltage = atten < 0 ? voltage : voltage * params[atten].getValue();

        bool sendUpdate =
            std::abs(value - lastParamValues[i]) > EPSILON
                ? true
                : std::abs(voltage - lastInputVoltages[i]) > EPSILON;

        if (sendUpdate) {
          // send the param value + the (attenuated) voltage to Unity
          std::vector<OscArg> update = {value + voltage};
          std::string address =
              instanceAddress + "/" + updateParams[i].oscAddress;
          URack::networkManager->dispatcher->send(activeHosts, address, update);
          // store the values for the next frame
          // so that we only send when there is a change
          lastParamValues[i] = value;
          lastInputVoltages[i] = voltage;
        }
      }

      // check for new point cloud port connections,
      // and send updates if necessary

      for (unsigned int i = 0; i < pointCloudOutputs.size(); i++) {
        auto output = pointCloudOutputs[i];
        auto cables = APP->scene->rack->getCablesOnPort(output.port);
        if (cables.size() > output.connections.size()) {
          // if we have connected new ports
          for (auto cableWidget : cables) {
            // iterate through each cable and add the target port to
            // this port's connection list if it isn't already in
            // there
            auto inputPort =
                dynamic_cast<PointCloudPort *>(cableWidget->inputPort);
            // if inputPort is null the cable is being dragged
            if (inputPort) {
              if (std::count(output.connections.begin(),
                             output.connections.end(), inputPort))
                continue;
              output.connections.push_back(inputPort);

              // Dispatch connect message
              auto module = dynamic_cast<UModule *>(inputPort->module);
              std::vector<OscArg> update = {output.port->oscAddress.c_str(),
                                            module->id,
                                            inputPort->oscAddress.c_str()};
              std::string address = instanceAddress + "/Connect";
              URack::networkManager->dispatcher->send(activeHosts, address,
                                                      update);
            }
          }
        } else if (cables.size() < output.connections.size()) {
          // if the connection is no longer connected by a cable,
          // remove it
          std::vector<PointCloudPort *> disconnectList;
          for (auto target : output.connections) {
            bool stillConnected = false;
            for (auto cableWidget : cables) {
              auto inputPort =
                  dynamic_cast<PointCloudPort *>(cableWidget->inputPort);
              if (inputPort == target) {
                stillConnected = true;
                break;
              }
            }
            if (!stillConnected)
              disconnectList.push_back(target);
          }

          for (auto target : disconnectList) {
            // remove the target from the output.connections list
            output.connections.erase(std::remove(output.connections.begin(),
                                                 output.connections.end(),
                                                 target),
                                     output.connections.end());
            // and dispatch a disconnect message
            auto targetModule = dynamic_cast<UModule *>(target->module);
            std::vector<OscArg> update = {output.port->oscAddress.c_str(),
                                          targetModule->id,
                                          target->oscAddress.c_str()};
            std::string address = instanceAddress + "/Disconnect";
            URack::networkManager->dispatcher->send(activeHosts, address,
                                                    update);
          }
        }
        pointCloudOutputs[i] = output;
      }
      updateTimer -= OSC_UPDATE_PERIOD;
    }
  }

  // override update instead of process in implimentation
  virtual void update(const ProcessArgs &args){};

  void dataFromJson(json_t *rootJ) override {
    json_t *activeHostsJ = json_object_get(rootJ, "activeHosts");
    if (activeHostsJ) {
      int i = 0;
      while (json_array_get(activeHostsJ, i) != NULL) {
        activeHosts.push_back(
            json_integer_value(json_array_get(activeHostsJ, i)));
        i++;
      }
    }
    json_t *activeJ = json_object_get(rootJ, "active");
    if (activeJ)
      active = json_integer_value(activeJ);
    json_t *childParams = json_object_get(rootJ, "childParams");
    if (childParams)
      onLoad(childParams);
  }

  json_t *dataToJson() override {
    json_t *rootJ = json_object();

    json_t *activeHostsJ = json_array();
    for (int host : activeHosts)
      json_array_append_new(activeHostsJ, json_integer(host));

    json_object_set_new(rootJ, "activeHosts", activeHostsJ);

    json_object_set_new(rootJ, "active", json_integer(active));
    json_object_set_new(rootJ, "childParams", onSave());
    return rootJ;
  }

  // override these instead of the regular Json methods above
  virtual void onLoad(json_t *roojJ){};
  virtual json_t *onSave() { return json_object(); };
};

struct UModuleWidget : ModuleWidget {
  void addPointCloudInput(math::Vec pos, UModule *module, int inputId,
                          std::string oscAddress) {
    auto port = createInputCentered<PointCloudPort>(pos, module, inputId);
    addInput(port);
    port->type = PointCloudPort::INPUT;
    port->oscAddress = oscAddress;
    if (module)
      module->pointCloudInputs.push_back(
          {inputId, false, port, std::vector<PointCloudPort *>{}});
  }

  void addPointCloudOutput(math::Vec pos, UModule *module, int outputId,
                           std::string oscAddress) {
    auto port = createOutputCentered<PointCloudPort>(pos, module, outputId);
    addOutput(port);
    port->type = PointCloudPort::OUTPUT;
    port->oscAddress = oscAddress;
    if (module)
      module->pointCloudOutputs.push_back(
          {outputId, false, port, std::vector<PointCloudPort *>{}});
  }

  struct HostMenuItem : MenuItem {
    UModule *module;
    SocketInfo *socketInfo;
    bool active;
    void onAction(const event::Action &e) override {
      if (!active)
        module->activateHost(socketInfo->ip);
      else
        module->deactivateHost(socketInfo->ip);
    }
  };

  struct AddHostItem : TextField {
    UModule *module;
    Menu *menu;

    AddHostItem() {
      box.size.x = 100;
      placeholder = "Add new host";
    }

    void onSelectKey(const event::SelectKey &e) override {
      if (e.action == GLFW_PRESS || e.action == GLFW_REPEAT) {
        if (e.key == GLFW_KEY_BACKSPACE) {
          setText(text.substr(0, text.size() - 1));
          e.consume(this);
        } else if (e.key == GLFW_KEY_ENTER) {
          struct sockaddr_in sa;
          if (inet_pton(AF_INET, text.c_str(), &(sa.sin_addr)) > 0) {
            // if the input is a valid IPv4
            module->activateHost(text.c_str());
            menu->hide();
          };
          e.consume(this);
        }
      }
    }
  };

  void appendContextMenu(Menu *menu) override {
    auto module = dynamic_cast<UModule *>(this->module);

    menu->addChild(new MenuEntry);
    menu->addChild(createMenuLabel("Connect hosts"));

    auto activeHosts = module->activeHosts;

    for (auto socketInfo : URack::networkManager->dispatcher->sockets) {
      auto hostItem = new HostMenuItem;
      hostItem->module = module;
      hostItem->socketInfo = socketInfo;
      hostItem->text = socketInfo->ip;
      if (std::count(activeHosts.begin(), activeHosts.end(),
                     socketInfo->hostNum)) {
        hostItem->active = true;
        hostItem->rightText = CHECKMARK(true);
      } else
        hostItem->active = false;
      menu->addChild(hostItem);
    }
    auto addHostItem = new AddHostItem;
    addHostItem->module = module;
    addHostItem->menu = menu;
    menu->addChild(addHostItem);
  }

  virtual void updateFields() {}
};

} // namespace URack
