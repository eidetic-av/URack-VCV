#include "UModule.hpp"

struct PlyPlayer : URack::UModule {
	enum ParamIds {
		POSITION_X_PARAM,
		POSITION_Y_PARAM,
		POSITION_Z_PARAM,
		ROTATION_X_PARAM,
		ROTATION_Y_PARAM,
		ROTATION_Z_PARAM,
		SCALE_ATTEN_PARAM,
		SCALE_PARAM,
		ACTIVE_PARAM,
		SPEED_ATTEN_PARAM,
		RUN_PARAM,
		SPEED_PARAM,
		RESET_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		POSITION_X_INPUT,
		POSITION_Y_INPUT,
		POSITION_Z_INPUT,
		ROTATION_X_INPUT,
		ROTATION_Y_INPUT,
		ROTATION_Z_INPUT,
		SCALE_INPUT,
		ACTIVE_INPUT,
		RUN_INPUT,
		SPEED_INPUT,
		RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		POINT_CLOUD_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	PlyPlayer() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configBiUpdate("PositionX", POSITION_X_PARAM, POSITION_X_INPUT);
		configBiUpdate("PositionY", POSITION_Y_PARAM, POSITION_Y_INPUT);
		configBiUpdate("PositionZ", POSITION_Z_PARAM, POSITION_Z_INPUT);
		configBiUpdate("RotationX", ROTATION_X_PARAM, ROTATION_X_INPUT);
		configBiUpdate("RotationY", ROTATION_Y_PARAM, ROTATION_Y_INPUT);
		configBiUpdate("RotationZ", ROTATION_Z_PARAM, ROTATION_Z_INPUT);
		configUpdate("Scale", SCALE_PARAM, SCALE_INPUT, SCALE_ATTEN_PARAM, 0.f);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
		configUpdate("Run", RUN_PARAM, RUN_INPUT);
		configUpdate("Speed", SPEED_PARAM, SPEED_INPUT, SPEED_ATTEN_PARAM, 0.f);
		configUpdate("Reset", RESET_PARAM, RESET_INPUT);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct PlyPlayerWidget : URack::UModuleWidget {
	PlyPlayerWidget(PlyPlayer* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/PlyPlayer.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(29.445, 25.496)), module, PlyPlayer::POSITION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(43.373, 25.496)), module, PlyPlayer::POSITION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(57.281, 25.496)), module, PlyPlayer::POSITION_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(8.438, 47.143)), module, PlyPlayer::ROTATION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(22.346, 47.143)), module, PlyPlayer::ROTATION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(36.274, 47.143)), module, PlyPlayer::ROTATION_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(51.341, 70.112)), module, PlyPlayer::SCALE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(42.801, 79.148)), module, PlyPlayer::SCALE_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(16.566, 84.062)), module, PlyPlayer::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(16.566, 84.062)), module, PlyPlayer::ACTIVE_LIGHT));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(51.015, 96.4)), module, PlyPlayer::SPEED_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(19.422, 100.522)), module, PlyPlayer::RUN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(42.475, 105.435)), module, PlyPlayer::SPEED_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(22.754, 116.333)), module, PlyPlayer::RESET_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(29.445, 36.124)), module, PlyPlayer::POSITION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.373, 36.124)), module, PlyPlayer::POSITION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(57.281, 36.124)), module, PlyPlayer::POSITION_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.438, 57.771)), module, PlyPlayer::ROTATION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.346, 57.771)), module, PlyPlayer::ROTATION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(36.274, 57.771)), module, PlyPlayer::ROTATION_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(56.464, 79.168)), module, PlyPlayer::SCALE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.527, 84.063)), module, PlyPlayer::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.383, 100.522)), module, PlyPlayer::RUN_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(56.137, 105.455)), module, PlyPlayer::SPEED_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.715, 116.567)), module, PlyPlayer::RESET_INPUT));

		addPointCloudOutput(mm2px(Vec(56.035, 120.973)), module, PlyPlayer::POINT_CLOUD_OUTPUT, "PointCloudOutput");
	}
};


Model* modelPlyPlayer = createModel<PlyPlayer, PlyPlayerWidget>("PlyPlayer");