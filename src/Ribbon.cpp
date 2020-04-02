#include "UModule.hpp"

struct Ribbon : URack::UModule {
	enum ParamIds {
		ACTIVE_PARAM,
		WIDTH_ATTEN_PARAM,
		LENGTH_ATTEN_PARAM,
		GLOW_ATTEN_PARAM,
		CAP_TYPE_PARAM,
		LENGTH_PARAM,
		WIDTH_PARAM,
		GLOW_PARAM,
		TAIL_PARAM,
		HUE_PARAM,
		SATURATION_PARAM,
		BRIGHTNESS_PARAM,
		HUE_ATTEN_PARAM,
		SATURATION_ATTEN_PARAM,
		BRIGHTNESS_ATTEN_PARAM,
		TIP_X_PARAM,
		TIP_Y_PARAM,
		TIP_Z_PARAM,
		TIP_X_ATTEN_PARAM,
		TIP_Y_ATTEN_PARAM,
		TIP_Z_ATTEN_PARAM,
		ORIGIN_X_PARAM,
		ORIGIN_Y_PARAM,
		ORIGIN_Z_PARAM,
		ORIGIN_X_ATTEN_PARAM,
		ORIGIN_Y_ATTEN_PARAM,
		ORIGIN_Z_ATTEN_PARAM,
		ROTATION_X_PARAM,
		ROTATION_Y_PARAM,
		ROTATION_Z_PARAM,
		ROTATION_X_ATTEN_PARAM,
		ROTATION_Y_ATTEN_PARAM,
		ROTATION_Z_ATTEN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ACTIVE_INPUT,
		WIDTH_INPUT,
		LENGTH_INPUT,
		GLOW_INPUT,
		HUE_INPUT,
		SATURATION_INPUT,
		BRIGHTNESS_INPUT,
		TIP_X_INPUT,
		TIP_Y_INPUT,
		TIP_Z_INPUT,
		ORIGIN_X_INPUT,
		ORIGIN_Y_INPUT,
		ORIGIN_Z_INPUT,
		ROTATION_X_INPUT,
		ROTATION_Y_INPUT,
		ROTATION_Z_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	Ribbon() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
		configBiUpdate("CapType", CAP_TYPE_PARAM);
		configUpdate("Length", LENGTH_PARAM, LENGTH_INPUT, LENGTH_ATTEN_PARAM, 0.f);
		configUpdate("Width", WIDTH_PARAM, WIDTH_INPUT, WIDTH_ATTEN_PARAM, 0.f);
		configUpdate("Glow", GLOW_PARAM, GLOW_INPUT, GLOW_ATTEN_PARAM, 0.f);
		configBiUpdate("Tail", TAIL_PARAM);
		configUpdate("Hue", HUE_PARAM, HUE_INPUT, HUE_ATTEN_PARAM, 0.f);
		configUpdate("Saturation", SATURATION_PARAM, SATURATION_INPUT, SATURATION_ATTEN_PARAM, 0.f);
		configUpdate("Brightness", BRIGHTNESS_PARAM, BRIGHTNESS_INPUT, BRIGHTNESS_ATTEN_PARAM, 0.f);
		configBiUpdate("TipX", TIP_X_PARAM, TIP_X_INPUT, TIP_X_ATTEN_PARAM, 0.f);
		configBiUpdate("TipY", TIP_Y_PARAM, TIP_Y_INPUT, TIP_Y_ATTEN_PARAM, 0.f);
		configBiUpdate("TipZ", TIP_Z_PARAM, TIP_Z_INPUT, TIP_Z_ATTEN_PARAM, 0.f);
		configBiUpdate("OriginX", ORIGIN_X_PARAM, ORIGIN_X_INPUT, ORIGIN_X_ATTEN_PARAM, 0.f);
		configBiUpdate("OriginY", ORIGIN_Y_PARAM, ORIGIN_Y_INPUT, ORIGIN_Y_ATTEN_PARAM, 0.f);
		configBiUpdate("OriginZ", ORIGIN_Z_PARAM, ORIGIN_Z_INPUT, ORIGIN_Z_ATTEN_PARAM, 0.f);
		configBiUpdate("RotationX", ROTATION_X_PARAM, ROTATION_X_INPUT, ROTATION_X_ATTEN_PARAM, 0.f);
		configBiUpdate("RotationY", ROTATION_Y_PARAM, ROTATION_Y_INPUT, ROTATION_Y_ATTEN_PARAM, 0.f);
		configBiUpdate("RotationZ", ROTATION_Z_PARAM, ROTATION_Z_INPUT, ROTATION_Z_ATTEN_PARAM, 0.f);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct RibbonWidget : URack::UModuleWidget {
	RibbonWidget(Ribbon* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Ribbon.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

                addParam(createParamCentered<LEDBezel>(mm2px(Vec(123.987, 13.267)), module, Ribbon::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(123.987, 13.267)), module, Ribbon::ACTIVE_LIGHT));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(77.86, 33.464)), module, Ribbon::WIDTH_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(19.195, 34.364)), module, Ribbon::LENGTH_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(129.903, 35.696)), module, Ribbon::GLOW_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(98.626, 37.078)), module, Ribbon::CAP_TYPE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(30.911, 40.754)), module, Ribbon::LENGTH_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(66.145, 41.204)), module, Ribbon::WIDTH_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(118.188, 43.437)), module, Ribbon::GLOW_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(48.438, 45.491)), module, Ribbon::TAIL_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(36.991, 72.725)), module, Ribbon::HUE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(50.919, 73.091)), module, Ribbon::SATURATION_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(64.827, 73.274)), module, Ribbon::BRIGHTNESS_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(83.553, 73.274)), module, Ribbon::HUE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(105.476, 73.274)), module, Ribbon::SATURATION_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(128.646, 73.274)), module, Ribbon::BRIGHTNESS_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(36.991, 90.436)), module, Ribbon::TIP_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(50.919, 90.802)), module, Ribbon::TIP_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(64.827, 90.985)), module, Ribbon::TIP_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(83.553, 90.985)), module, Ribbon::TIP_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(105.476, 90.985)), module, Ribbon::TIP_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(128.646, 90.985)), module, Ribbon::TIP_Z_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(36.991, 104.07)), module, Ribbon::ORIGIN_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(50.919, 104.436)), module, Ribbon::ORIGIN_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(64.827, 104.619)), module, Ribbon::ORIGIN_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(83.553, 104.619)), module, Ribbon::ORIGIN_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(105.476, 104.619)), module, Ribbon::ORIGIN_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(128.646, 104.619)), module, Ribbon::ORIGIN_Z_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(36.991, 117.442)), module, Ribbon::ROTATION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(50.919, 117.808)), module, Ribbon::ROTATION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(64.827, 117.991)), module, Ribbon::ROTATION_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(83.553, 117.991)), module, Ribbon::ROTATION_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(105.476, 117.991)), module, Ribbon::ROTATION_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(128.646, 117.991)), module, Ribbon::ROTATION_Z_ATTEN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(135.025, 13.267)), module, Ribbon::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(81.924, 41.99)), module, Ribbon::WIDTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.131, 42.891)), module, Ribbon::LENGTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(133.967, 44.223)), module, Ribbon::GLOW_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.654, 73.274)), module, Ribbon::HUE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.577, 73.274)), module, Ribbon::SATURATION_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(137.747, 73.274)), module, Ribbon::BRIGHTNESS_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.654, 90.985)), module, Ribbon::TIP_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.577, 90.985)), module, Ribbon::TIP_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(137.747, 90.985)), module, Ribbon::TIP_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.654, 104.619)), module, Ribbon::ORIGIN_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.577, 104.619)), module, Ribbon::ORIGIN_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(137.747, 104.619)), module, Ribbon::ORIGIN_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.654, 117.991)), module, Ribbon::ROTATION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(114.577, 117.991)), module, Ribbon::ROTATION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(137.747, 117.991)), module, Ribbon::ROTATION_Z_INPUT));
	}
};


Model* modelRibbon = createModel<Ribbon, RibbonWidget>("Ribbon");