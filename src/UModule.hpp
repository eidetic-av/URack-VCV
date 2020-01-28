#include "URack.hpp"
#include "dsp/digital.hpp"
#include "plugin.hpp"
#include "settings.hpp"

namespace URack {

struct UModule : Module {
	const char* hostIp = LOCALHOST;
	int hostPort = PORT;
	int hostNum = 0;

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
		PointCloudPort* port;
	};

	std::vector<PointCloudPortInfo> pointCloudInputs;
	std::vector<PointCloudPortInfo> pointCloudOutputs;

	bool initialised = false;

	int activateParam = -1;
	int activateInput = -1;
	int activateLight = -1;
	dsp::BooleanTrigger activateTrigger;
	bool active;
	bool lastUpdateActive;

	UModule() {}

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
		if (atten > -1) configParam(atten, -1.f, 1.f, 0.f, oscAddress);
		UpdateParam updateParam = {oscAddress, param, input, atten};
		updateParams.push_back(updateParam);
	}

	void setHost(const char* ip, int port = PORT) {
		int host = -1;
		// if host already exists in Dispatcher, assign that
		for (unsigned int i = 0; i < Dispatcher::sockets.size(); i++) {
			auto socket = Dispatcher::sockets[i];
			if (socket.ip == ip && socket.port == port) {
				host = i;
				break;
			}
		}
		// otherwise create a new one at the specified endpoint
		if (hostNum == -1) hostNum = Dispatcher::create(ip, port);
		// and set the UModule properties
		hostNum = host;
		hostIp = ip;
		hostPort = port;
	}

	void onAdd() override {
		instanceAddress = "Instance/" + model->slug + "/" + std::to_string(id);
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(hostNum, "Add", args);

		// initialise last values arrays
		// and set to -99 to force an update
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages.push_back(-99);
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues.push_back(-99);
	}

	void onRemove() override {
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(hostNum, "Remove", args);
	}

	void onReset() override {
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(hostNum, "Reset", args);
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages[i] = -99;
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues[i] = -99;
	}

	void process(const ProcessArgs& args) override {
		if (activateParam > -1) {
			if (activateTrigger.process(params[activateParam].getValue() > 0.f))
				active ^= true;
			if (activateInput > -1)
				if (inputs[activateInput].getVoltage() != 0)
					active = inputs[activateInput].getVoltage() > 0.f;
			if (activateLight > -1)
				lights[activateLight].setBrightness(active ? 10.f : 0.f);
			if (lastUpdateActive != active) {
				// send active status on change
				Dispatcher::send(hostNum, instanceAddress + "/Active",
						active ? 1 : 0);
				// force parameter updates
				for (unsigned int i = 0; i < inputs.size(); i++)
					lastInputVoltages[i] = -99;
				for (unsigned int i = 0; i < params.size(); i++)
					lastParamValues[i] = -99;
			}
			lastUpdateActive = active;
		}

		// for initialisation that must happen after the module is
		// fully loaded
		if (!initialised) {
			Dispatcher::send(hostNum, instanceAddress + "/Active",
					active ? 1 : 0);
			initialised = true;
		}

		if (!active) return;

		update(args);
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
				voltage =
					atten < 0 ? voltage : voltage * params[atten].getValue();

				bool sendUpdate =
					std::abs(value - lastParamValues[i]) > EPSILON
					? true
					: std::abs(voltage - lastInputVoltages[i]) > EPSILON;

				if (sendUpdate) {
					// send the param value + the (attenuated) voltage to Unity
					std::vector<OscArg> update = {value + voltage};
					std::string address =
						instanceAddress + "/" + updateParams[i].oscAddress;
					URack::Dispatcher::send(hostNum, address, update);
					// store the values for the next frame
					// so that we only send when there is a change
					lastParamValues[i] = value;
					lastInputVoltages[i] = voltage;
				}
			}
			// check for new point cloud port connections,
			// and send updates if necessary
			if (pointCloudOutputs.size() > 0) {
				for (unsigned int i = 0; i < pointCloudOutputs.size(); i++) {
					auto connection = pointCloudOutputs[i];
					int id = connection.id;
					bool connectionStatus = connection.connectionStatus;
					auto output = outputs[id];
					bool connected = output.isConnected();
					if (connectionStatus != connected) {
						// connection status changed, update Port Info
						pointCloudOutputs[i].connectionStatus = connected;
						// try get the cable
						PointCloudPort* port = connection.port;
						auto cw = APP->scene->rack->getTopCable(port);
						if (cw) {
							// if cable exists, it means we created a connection
							auto inputPort =
								dynamic_cast<PointCloudPort*>(cw->inputPort);
							auto module =
								dynamic_cast<UModule*>(inputPort->module);
							// Dispatch connect message
							std::vector<OscArg> update = {
								port->oscAddress.c_str(), module->id,
								inputPort->oscAddress.c_str()};
							std::string address = instanceAddress + "/Connect";
							URack::Dispatcher::send(hostNum, address, update);

							// Todo: Need to figure out how to replace regular
							// cables that are loaded on serialization with
							// PointCloudCable
							/* auto pccw = */
							/* 	dynamic_cast<PointCloudCableWidget*>(cw); */
							/* if (!pccw) */
							/* 	PointCloudPort::replaceRegularCable(port, */
							/* 			inputPort); */

						} else {
							// if cable doesn't exist it means we removed a
							// connection
							// Dispatch disconnect message
							std::vector<OscArg> update = {
								port->oscAddress.c_str()};
							std::string address =
								instanceAddress + "/Disconnect";
							URack::Dispatcher::send(hostNum, address, update);
						}
					}
				}
			}
			updateTimer -= OSC_UPDATE_PERIOD;
		}
	}

	// override update instead of process in implimentation
	virtual void update(const ProcessArgs& args){};
};

struct UModuleWidget : ModuleWidget {
	void addPointCloudInput(math::Vec pos, UModule* module, int inputId,
			std::string oscAddress) {
		auto port = createInputCentered<PointCloudPort>(pos, module, inputId);
		addInput(port);
		port->type = PointCloudPort::INPUT;
		port->oscAddress = oscAddress;
		if (module) module->pointCloudInputs.push_back({inputId, false, port});
	}

	void addPointCloudOutput(math::Vec pos, UModule* module, int outputId,
			std::string oscAddress) {
		auto port = createOutputCentered<PointCloudPort>(pos, module, outputId);
		addOutput(port);
		port->type = PointCloudPort::OUTPUT;
		port->oscAddress = oscAddress;
		if (module)
			module->pointCloudOutputs.push_back({outputId, false, port});
	}
};

}  // namespace URack
