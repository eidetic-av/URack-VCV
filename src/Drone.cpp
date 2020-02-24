#include "UModule.hpp"

struct Drone : URack::UModule {
	enum ParamIds {
		X_PARAM,
		Y_PARAM,
		Z_PARAM,
		X_ATTEN_PARAM,
		Y_ATTEN_PARAM,
		Z_ATTEN_PARAM,
		ORBIT_PARAM,
		ELEVATION_PARAM,
		DISTANCE_PARAM,
		ORBIT_ATTEN_PARAM,
		ELEVATION_ATTEN_PARAM,
		DISTANCE_ATTEN_PARAM,
		ACTIVE_PARAM,
		TARGET_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		X_INPUT,
		Y_INPUT,
		Z_INPUT,
		ORBIT_INPUT,
		ELEVATION_INPUT,
		DISTANCE_INPUT,
		ACTIVE_INPUT,
		TARGET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		TARGET_LIGHT,
		NUM_LIGHTS
	};

	bool target;
	rack::dsp::BooleanTrigger targetTrigger;

	Drone() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configBiUpdate("X", X_PARAM, X_INPUT, X_ATTEN_PARAM, 0.f);
		configBiUpdate("Y", Y_PARAM, Y_INPUT, Y_ATTEN_PARAM, 0.f);
		configBiUpdate("Z", Z_PARAM, Z_INPUT, Z_ATTEN_PARAM, 0.f);
		configBiUpdate("Orbit", ORBIT_PARAM, ORBIT_INPUT, ORBIT_ATTEN_PARAM, 0.f);
		configBiUpdate("Elevation", ELEVATION_PARAM, ELEVATION_INPUT, ELEVATION_ATTEN_PARAM, 0.f);
		configUpdate("Distance", DISTANCE_PARAM, DISTANCE_INPUT, DISTANCE_ATTEN_PARAM, 0.f);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
	}

	void start() override {
		URack::Dispatcher::send(hostNum, instanceAddress + "/Target", target);
		lights[TARGET_LIGHT].setBrightness(target * 10.f);
	}

	void update(const ProcessArgs& args) override {
		bool oldTarget = target;
		// use the button to determine target status
		if (!inputs[TARGET_INPUT].isConnected()) {
			bool targetButton = params[TARGET_PARAM].getValue() > 0;
			if (targetTrigger.process(targetButton))
				target = target > 0 ? 0 : 1;
		} else {
			// if we have a connected target input, ignore the button
			float targetVoltage = inputs[TARGET_INPUT].getVoltage();
			target = targetVoltage > 0 ? 1 : 0;
		}
		if (oldTarget != target) {
			lights[TARGET_LIGHT].setBrightness(target * 10.f);
			URack::Dispatcher::send(hostNum, instanceAddress + "/Target", target);
		}
	}

	void onLoad(json_t* rootJ) override {
		json_t* targetJ = json_object_get(rootJ, "target");
		if (targetJ) target = json_integer_value(targetJ);
	}

	json_t* onSave() override {
		json_t* rootJ = json_object();
		json_object_set_new(rootJ, "target", json_integer(target));
		return rootJ;
	}
};

struct DroneWidget : URack::UModuleWidget {
	DroneWidget(Drone* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Drone.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.923, 35.291)), module, Drone::X_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 35.291)), module, Drone::Y_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 35.291)), module, Drone::Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 56.251)), module, Drone::X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 56.251)), module, Drone::Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 56.251)), module, Drone::Z_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.922, 75.034)), module, Drone::ORBIT_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 75.034)), module, Drone::ELEVATION_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 75.034)), module, Drone::DISTANCE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 95.994)), module, Drone::ORBIT_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 95.994)), module, Drone::ELEVATION_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 95.994)), module, Drone::DISTANCE_ATTEN_PARAM));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(16.625, 108.759)), module, Drone::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(16.625, 108.759)), module, Drone::ACTIVE_LIGHT));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(42.437, 108.759)), module, Drone::TARGET_PARAM));
		addChild(createLightCentered<LEDBezelLight<BlueLight>>(mm2px(Vec(42.437, 108.759)), module, Drone::TARGET_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 56.251)), module, Drone::X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 56.251)), module, Drone::Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 56.251)), module, Drone::Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 95.994)), module, Drone::ORBIT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 95.994)), module, Drone::ELEVATION_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 95.994)), module, Drone::DISTANCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(28.177, 108.759)), module, Drone::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(53.989, 108.759)), module, Drone::TARGET_INPUT));
	}
};

Model* modelDrone = createModel<Drone, DroneWidget>("Drone");
