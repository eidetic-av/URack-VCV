#include "UModule.hpp"

struct GlowWorm : URack::UModule {
	enum ParamIds {
		ACTIVE_PARAM,
		WIDTH_ATTEN_PARAM,
		LENGTH_ATTEN_PARAM,
		CAP_TYPE_PARAM,
		GLOW_ATTEN_PARAM,
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

	GlowWorm() {
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


struct GlowWormWidget : URack::UModuleWidget {
	GlowWormWidget(GlowWorm* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/GlowWorm.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

                addParam(createParamCentered<LEDBezel>(mm2px(Vec(119.224, 26.496)), module, GlowWorm::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(119.224, 26.496)), module, GlowWorm::ACTIVE_LIGHT));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(73.871, 34.522)), module, GlowWorm::WIDTH_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(15.206, 35.423)), module, GlowWorm::LENGTH_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(94.637, 38.136)), module, GlowWorm::CAP_TYPE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(125.914, 38.342)), module, GlowWorm::GLOW_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(26.922, 41.813)), module, GlowWorm::LENGTH_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(62.156, 42.263)), module, GlowWorm::WIDTH_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(114.198, 46.083)), module, GlowWorm::GLOW_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(44.448, 46.55)), module, GlowWorm::TAIL_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(34.229, 73.274)), module, GlowWorm::HUE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(48.157, 73.274)), module, GlowWorm::SATURATION_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(62.064, 73.274)), module, GlowWorm::BRIGHTNESS_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(80.79, 73.274)), module, GlowWorm::HUE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(100.597, 73.274)), module, GlowWorm::SATURATION_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(120.063, 73.274)), module, GlowWorm::BRIGHTNESS_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(34.229, 90.985)), module, GlowWorm::TIP_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(48.157, 90.985)), module, GlowWorm::TIP_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(62.064, 90.985)), module, GlowWorm::TIP_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(80.79, 90.985)), module, GlowWorm::TIP_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(100.597, 90.985)), module, GlowWorm::TIP_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(120.063, 90.985)), module, GlowWorm::TIP_Z_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(34.229, 104.619)), module, GlowWorm::ORIGIN_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(48.157, 104.619)), module, GlowWorm::ORIGIN_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(62.064, 104.619)), module, GlowWorm::ORIGIN_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(80.79, 104.619)), module, GlowWorm::ORIGIN_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(100.597, 104.619)), module, GlowWorm::ORIGIN_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(120.063, 104.619)), module, GlowWorm::ORIGIN_Z_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(34.229, 117.991)), module, GlowWorm::ROTATION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(48.157, 117.991)), module, GlowWorm::ROTATION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(62.064, 117.991)), module, GlowWorm::ROTATION_Z_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(80.79, 117.991)), module, GlowWorm::ROTATION_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(100.597, 117.991)), module, GlowWorm::ROTATION_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(120.063, 117.991)), module, GlowWorm::ROTATION_Z_ATTEN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(130.263, 26.496)), module, GlowWorm::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(77.935, 43.049)), module, GlowWorm::WIDTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.142, 43.949)), module, GlowWorm::LENGTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.978, 46.869)), module, GlowWorm::GLOW_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(89.892, 73.274)), module, GlowWorm::HUE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(109.698, 73.274)), module, GlowWorm::SATURATION_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.164, 73.274)), module, GlowWorm::BRIGHTNESS_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(89.892, 90.985)), module, GlowWorm::TIP_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(109.698, 90.985)), module, GlowWorm::TIP_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.164, 90.985)), module, GlowWorm::TIP_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(89.892, 104.619)), module, GlowWorm::ORIGIN_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(109.698, 104.619)), module, GlowWorm::ORIGIN_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.164, 104.619)), module, GlowWorm::ORIGIN_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(89.892, 117.991)), module, GlowWorm::ROTATION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(109.698, 117.991)), module, GlowWorm::ROTATION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(129.164, 117.991)), module, GlowWorm::ROTATION_Z_INPUT));
	}
};


Model* modelGlowWorm = createModel<GlowWorm, GlowWormWidget>("GlowWorm");