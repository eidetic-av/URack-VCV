#include "UModule.hpp"

struct Drone : URack::UModule {
	enum ParamIds {
		PACE_PARAM,
		STRAFE_PARAM,
		DISTANCE_PARAM,
		PACE_ATTEN_PARAM,
		STRAFE_ATTEN_PARAM,
		DISTANCE_ATTEN_PARAM,
		ORBIT_X_PARAM,
		ORBIT_Y_PARAM,
		ZOOM_PARAM,
		ORBIT_X_ATTEN_PARAM,
		ORBIT_Y_ATTEN_PARAM,
		ZOOM_ATTEN_PARAM,
		ACTIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PACE_INPUT,
		STRAFE_INPUT,
		DISTANCE_INPUT,
		ORBIT_X_INPUT,
		ORBIT_Y_INPUT,
		ZOOM_INPUT,
		ACTIVE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		Y_OUTPUT,
		Z_OUTPUT,
		X_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	Drone() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configBiUpdate("Pace", PACE_PARAM, PACE_INPUT, PACE_ATTEN_PARAM, 0.f);
		configBiUpdate("Strafe", STRAFE_PARAM, STRAFE_INPUT, STRAFE_ATTEN_PARAM, 0.f);
		configUpdate("Distance", DISTANCE_PARAM, DISTANCE_INPUT, DISTANCE_ATTEN_PARAM, 0.f);
		configBiUpdate("OrbitX", ORBIT_X_PARAM, ORBIT_X_INPUT, ORBIT_X_ATTEN_PARAM, 0.f);
		configBiUpdate("OrbitY", ORBIT_Y_PARAM, ORBIT_Y_INPUT, ORBIT_Y_ATTEN_PARAM, 0.f);
		configUpdate("Zoom", ZOOM_PARAM, ZOOM_INPUT, ZOOM_ATTEN_PARAM, 0.f);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct DroneWidget : URack::UModuleWidget {
	DroneWidget(Drone* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Drone.svg")));

		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.923, 32.643)), module, Drone::PACE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 32.643)), module, Drone::STRAFE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 32.643)), module, Drone::DISTANCE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 53.604)), module, Drone::PACE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 53.604)), module, Drone::STRAFE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 53.604)), module, Drone::DISTANCE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.922, 72.382)), module, Drone::ORBIT_X_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 72.382)), module, Drone::ORBIT_Y_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 72.382)), module, Drone::ZOOM_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 93.342)), module, Drone::ORBIT_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 93.342)), module, Drone::ORBIT_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 93.342)), module, Drone::ZOOM_ATTEN_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(9.411, 107.172)), module, Drone::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(9.411, 107.172)), module, Drone::ACTIVE_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 53.604)), module, Drone::PACE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 53.604)), module, Drone::STRAFE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 53.604)), module, Drone::DISTANCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 93.342)), module, Drone::ORBIT_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 93.342)), module, Drone::ORBIT_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 93.342)), module, Drone::ZOOM_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(20.963, 107.172)), module, Drone::ACTIVE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(49.359, 107.171)), module, Drone::Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(63.028, 107.171)), module, Drone::Z_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.69, 107.172)), module, Drone::X_OUTPUT));
	}
};


Model* modelDrone = createModel<Drone, DroneWidget>("Drone");