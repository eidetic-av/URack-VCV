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
		X_OUTPUT,
		Y_OUTPUT,
		Z_OUTPUT,
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

		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.923, 33.702)), module, Drone::PACE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 33.702)), module, Drone::STRAFE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 33.702)), module, Drone::DISTANCE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 54.662)), module, Drone::PACE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 54.662)), module, Drone::STRAFE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 54.662)), module, Drone::DISTANCE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(12.922, 73.97)), module, Drone::ORBIT_X_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(35.56, 73.97)), module, Drone::ORBIT_Y_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(58.197, 73.97)), module, Drone::ZOOM_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(7.064, 94.93)), module, Drone::ORBIT_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(29.701, 94.93)), module, Drone::ORBIT_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(52.339, 94.93)), module, Drone::ZOOM_ATTEN_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(9.411, 109.288)), module, Drone::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(9.411, 109.288)), module, Drone::ACTIVE_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 54.662)), module, Drone::PACE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 54.662)), module, Drone::STRAFE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 54.662)), module, Drone::DISTANCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(17.753, 94.93)), module, Drone::ORBIT_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.39, 94.93)), module, Drone::ORBIT_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(63.028, 94.93)), module, Drone::ZOOM_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(20.963, 109.288)), module, Drone::ACTIVE_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(35.69, 109.288)), module, Drone::X_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(49.359, 109.288)), module, Drone::Y_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(63.028, 109.288)), module, Drone::Z_OUTPUT));
	}
};


Model* modelDrone = createModel<Drone, DroneWidget>("Drone");