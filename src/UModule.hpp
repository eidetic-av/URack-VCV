#include "URack.hpp"
#include <map>

namespace URack {

struct UModule : Module {

	int host = 0;
	std::string instanceAddress;
	std::map<int, std::string> inputAddressMap;

	float epsilon = std::numeric_limits<float>::epsilon();

	float updateTimer = -1;
	std::vector<float> lastInputValues;

	UModule() {
	}

	void onAdd() override {
		instanceAddress = "instance/" + model->slug + "/" + std::to_string(id);
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "add", args, 2);

		// initialise last input values array
		// and force an update
		lastInputValues.resize(inputs.size());
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputValues[i] = -99;
	}

	void onRemove() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "remove", args, 2);
	}

	void onReset() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "reset", args, 2);
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputValues[i] = -99;
	}

	void process(const ProcessArgs & args) override {
		update(args);

		updateTimer += args.sampleTime;
		// only send updates over OSC every 100Hz
		if (updateTimer >= 0.01f) {
			// check for changed values and send updates if necessary
			for (int i = 0; i < (int) inputs.size(); i++)
			{
				if (std::abs(inputs[i].getVoltage() - lastInputValues[i]) > epsilon)
				{
					OscArg update [] = { inputs[i].getVoltage() };
					std::string address = instanceAddress + "/" + inputAddressMap[i];
					URack::Dispatcher::send(host, address, update, 1);
					lastInputValues[i] = inputs[i].getVoltage();
				}
			}
			updateTimer -= 0.01f;
		}
	}

	// override update instead of process in implimentation
	virtual void update(const ProcessArgs & args) { };

};

struct UModuleWidget : ModuleWidget {
	void addInput(PortWidget * input, std::string oscAddress) {
		ModuleWidget::addInput(input);
		// if this is a real module, not just being displayed in the browser
		if (this->module != NULL) {
			// create an osc address for the input
			auto uModule = (UModule *) this->module;
			uModule->inputAddressMap.insert({uModule->inputAddressMap.size(), oscAddress});
		}
	}
};

}
