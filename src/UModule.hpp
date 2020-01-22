#include "URack.hpp"

namespace URack {

struct UModule : Module {
	int host = 0;
	std::string instanceAddress;

	float epsilon = std::numeric_limits<float>::epsilon();

	float updateTimer = -1;
	std::vector<float> lastParamValues;
	std::vector<float> lastInputVoltages;

	struct UpdateParam {
		int param;
		int input;
		int inputScaleParam;
		std::string oscAddress;
	};

	std::vector<UpdateParam> updateParams;

	struct PointCloudPortInto {
		int id;
		bool connectionStatus;
		PointCloudPort* port;
	};

	std::vector<PointCloudPortInto> pointCloudInputs;
	std::vector<PointCloudPortInto> pointCloudOutputs;

	UModule() {}

	void configUpdate(int param, int input, std::string oscAddress) {
		configUpdate(param, input, -99, oscAddress);
	}

	void configUpdate(int param, int input, int inputScaleParam,
			std::string oscAddress) {
		UpdateParam updateParam = {param, input, inputScaleParam, oscAddress};
		updateParams.push_back(updateParam);
	}

	void onAdd() override {
		instanceAddress = "instance/" + model->slug + "/" + std::to_string(id);
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(host, "add", args);

		// initialise last values arrays
		// and set to -99 to force an update
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages.push_back(-99);
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues.push_back(-99);
	}

	void onRemove() override {
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(host, "remove", args);
	}

	void onReset() override {
		std::vector<OscArg> args = {model->slug.c_str(), id};
		URack::Dispatcher::send(host, "reset", args);
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages[i] = -99;
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues[i] = -99;
	}

	void process(const ProcessArgs& args) override {
		update(args);
		updateTimer += args.sampleTime;
		// only send updates over OSC every 1000Hz
		if (updateTimer >= 0.001f) {
			// check for changed values and send updates if necessary
			for (unsigned int i = 0; i < updateParams.size(); i++) {
				int param = updateParams[i].param;
				int input = updateParams[i].input;
				int inputScaleParam = updateParams[i].inputScaleParam;
				if (std::abs(params[param].getValue() - lastParamValues[i]) >
						epsilon ||
						std::abs(inputs[input].getVoltage() -
							lastInputVoltages[i]) > epsilon) {
					float value = params[param].getValue();
					float voltage = inputs[input].getVoltage();
					// if an input scale param is defined (i.e. not -99)
					// then attenuate the input voltage by its value
					if (inputScaleParam != -99)
						voltage = voltage * params[inputScaleParam].getValue();
					// send the param value + the (attenuated) voltage to Unity
					std::vector<OscArg> update = {value + voltage};
					std::string address =
						instanceAddress + "/" + updateParams[i].oscAddress;
					URack::Dispatcher::send(host, address, update);
					// store the values for the next frame
					// so that we only send when there is a change
					lastParamValues[i] = value;
					lastInputVoltages[i] = voltage;
				}
			}
			// check for new point cloud port connections,
			// and send updates if necessary
			if (pointCloudInputs.size() > 0) {
				for (unsigned int i = 0; i < pointCloudInputs.size(); i++) {
					auto connection = pointCloudInputs[i];
					int id = connection.id;
					bool connectionStatus = connection.connectionStatus;
					auto input = inputs[id];
					bool connected = input.isConnected();
					if (connectionStatus != connected) {
						pointCloudInputs[i].connectionStatus = connected;
					}
				}
			}
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
						CableWidget* cw = APP->scene->rack->getTopCable(port);
						if (cw) {
							// if cable exists, it means we created a connection
							auto inputPort =
								dynamic_cast<PointCloudPort*>(cw->inputPort);
							auto module =
								dynamic_cast<UModule*>(inputPort->module);
							// Dispatch connect message
							std::vector<OscArg> update = {
								port->oscAddress.c_str(),
								module->model->slug.c_str(), module->id,
								inputPort->oscAddress.c_str()};
							std::string address = instanceAddress + "/connect";
							URack::Dispatcher::send(host, address, update);
						} else {
							// if cable doesn't exist it means we removed a
							// connection
							// Dispatch disconnect message
							std::vector<OscArg> update = {
								port->oscAddress.c_str()};
							std::string address =
								instanceAddress + "/disconnect";
							URack::Dispatcher::send(host, address, update);
						}
					}
				}
			}

			updateTimer -= 0.001f;
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
