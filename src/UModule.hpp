#include "URack.hpp"
#include <tuple>

namespace URack {

struct UModule : Module {

	int host = 0;
	std::string instanceAddress;
	
	float epsilon = std::numeric_limits<float>::epsilon();

	float updateTimer = -1;
	std::vector<float> lastParamValues;
	std::vector<float> lastInputVoltages;
	std::vector<std::tuple<int, int, std::string>> updateParams;

	UModule() {
	}

	void configUpdate(int param, int input, std::string oscAddress)
	{
		updateParams.push_back(std::make_tuple(param, input, oscAddress));
	}

	void onAdd() override {
		instanceAddress = "instance/" + model->slug + "/" + std::to_string(id);
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "add", args, 2);

		// initialise last values arrays
		// and set to -99 to force an update
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages.push_back(-99);
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues.push_back(-99);
	}

	void onRemove() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "remove", args, 2);
	}

	void onReset() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, "reset", args, 2);
		for (unsigned int i = 0; i < inputs.size(); i++)
			lastInputVoltages[i] = -99;
		for (unsigned int i = 0; i < params.size(); i++)
			lastParamValues[i] = -99;
	}

	void process(const ProcessArgs & args) override {
		update(args);
		updateTimer += args.sampleTime;
		// only send updates over OSC every 1000Hz
		if (updateTimer >= 0.001f) {
			// check for changed values and send updates if necessary
			for (unsigned int i = 0; i < updateParams.size(); i++)
			{
				int param = std::get<0>(updateParams[i]);
				int input = std::get<1>(updateParams[i]);
				if (std::abs(params[param].getValue() - lastParamValues[i]) > epsilon
					|| std::abs(inputs[input].getVoltage() - lastInputVoltages[i]) > epsilon)
				{
					float value = params[param].getValue();
					float voltage = inputs[input].getVoltage();
					OscArg update [] = { value + voltage };
					std::string address = instanceAddress + "/" + std::get<2>(updateParams[i]);
					URack::Dispatcher::send(host, address, update, 1);
					lastParamValues[i] = value;
					lastInputVoltages[i] = voltage;
				}
			}
			updateTimer -= 0.001f;
		}
	}

	// override update instead of process in implimentation
	virtual void update(const ProcessArgs & args) { };
};

struct UModuleWidget : ModuleWidget {

	/* void addInput(PortWidget * input, std::string oscAddress) { */
	/* 	ModuleWidget::addInput(input); */
	/* 	// if this is a real module, not just being displayed in the browser */
	/* 	if (this->module != NULL) { */
	/* 		// create an osc address for the input */
	/* 		auto uModule = (UModule *) this->module; */
	/* 		uModule->inputAddressMap.insert({uModule->inputAddressMap.size(), oscAddress}); */
	/* 	} */
	/* } */

};

}
