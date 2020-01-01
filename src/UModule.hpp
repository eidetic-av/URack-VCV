#include "plugin.hpp"
#include "URack.hpp"

namespace URack {

struct UModule : Module {

	int host = 0;
	std::string instanceAddress;

	float updateTimer = -1;
	std::vector<float> lastInputValues;

	UModule() {
		lastInputValues.resize(sizeof(inputs));
	}

	void onAdd() override {	
		instanceAddress = INSTANCE_ROOT + model->slug + "/" + std::to_string(id);
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, ADD_ADDRESS, args, 2);
	}

	void onRemove() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, REMOVE_ADDRESS, args, 2);
	}

	void onReset() override {
		OscArg args [] = { model->slug.c_str(), id };
		URack::Dispatcher::send(host, RESET_ADDRESS, args, 2);
	}

	void process(const ProcessArgs & args) override {
		update(args);
		
		updateTimer += args.sampleTime;
		// only send updates over OSC every 100Hz
		if (updateTimer >= 0.01f) {
			// check for changed values and send updates if necessary
			for (int i = 0; i < sizeof(inputs); i++)
			{
				if (inputs[i].getVoltage() != lastInputValues[i])
				{
					OscArg update [] = { i, inputs[i].getVoltage() };	
					URack::Dispatcher::send(host, instanceAddress, update, 2);
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

};

}
