#include "UModule.hpp"

struct Billboard : URack::UModule {
	enum ParamIds {
		FADE_PARAM,
		GROWTH_PARAM,
		LIFE_PARAM,
		SPAWN_RATE_PARAM,
		SIZE_PARAM,
		LIFE_ATTEN_PARAM,
		SPAWN_RATE_ATTEN_PARAM,
		SIZE_ATTEN_PARAM,
		FUZZ_PARAM,
		TURBULENCE_DELAY_PARAM,
		TURBULENCE_SCALE_PARAM,
		TURBULENCE_PARAM,
		GLOW_PARAM,
		TURBULENCE_ATTEN_PARAM,
		FORCE_Y_PARAM,
		FORCE_Z_PARAM,
		FORCE_X_PARAM,
		ACTIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		FADE_INPUT,
		GROWTH_INPUT,
		LIFE_INPUT,
		SPAWN_RATE_INPUT,
		SIZE_INPUT,
		FUZZ_INPUT,
		GLOW_INPUT,
		TURBULENCE_DELAY_INPUT,
		TURBULENCE_SCALE_INPUT,
		TURBULENCE_INPUT,
		ACTIVE_INPUT,
		POINT_CLOUD_INPUT,
		FORCE_X_INPUT,
		FORCE_Y_INPUT,
		FORCE_Z_INPUT,
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

	Billboard() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configBiUpdate("Fade", FADE_PARAM, FADE_INPUT);
		configBiUpdate("Growth", GROWTH_PARAM, GROWTH_INPUT);
		configUpdate("Life", LIFE_PARAM, LIFE_INPUT, LIFE_ATTEN_PARAM, 0.f);
		configUpdate("SpawnRate", SPAWN_RATE_PARAM, SPAWN_RATE_INPUT, SPAWN_RATE_ATTEN_PARAM, 0.f);
		configUpdate("Size", SIZE_PARAM, SIZE_INPUT, SIZE_ATTEN_PARAM, 0.f);
		configUpdate("Fuzz", FUZZ_PARAM, FUZZ_INPUT);
		configBiUpdate("TurbulenceDelay", TURBULENCE_DELAY_PARAM, TURBULENCE_DELAY_INPUT);
		configUpdate("TurbulenceScale", TURBULENCE_SCALE_PARAM, TURBULENCE_SCALE_INPUT);
		configUpdate("Turbulence", TURBULENCE_PARAM, TURBULENCE_INPUT, TURBULENCE_ATTEN_PARAM, 0.f);
		configUpdate("Glow", GLOW_PARAM, GLOW_INPUT);
		configBiUpdate("ForceY", FORCE_Y_PARAM, FORCE_Y_INPUT);
		configBiUpdate("ForceZ", FORCE_Z_PARAM, FORCE_Z_INPUT);
		configBiUpdate("ForceX", FORCE_X_PARAM, FORCE_X_INPUT);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct BillboardWidget : URack::UModuleWidget {
	BillboardWidget(Billboard* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Billboard.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(19.394, 28.045)), module, Billboard::FADE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(102.262, 28.045)), module, Billboard::GROWTH_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(38.21, 32.116)), module, Billboard::LIFE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(60.796, 32.116)), module, Billboard::SPAWN_RATE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(83.446, 32.116)), module, Billboard::SIZE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(32.352, 53.076)), module, Billboard::LIFE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(54.938, 53.076)), module, Billboard::SPAWN_RATE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(77.588, 53.076)), module, Billboard::SIZE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.857, 63.831)), module, Billboard::FUZZ_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(41.116, 67.732)), module, Billboard::TURBULENCE_DELAY_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(80.545, 68.654)), module, Billboard::TURBULENCE_SCALE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(60.873, 71.804)), module, Billboard::TURBULENCE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(24.162, 81.293)), module, Billboard::GLOW_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(55.014, 92.764)), module, Billboard::TURBULENCE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(25.581, 102.412)), module, Billboard::FORCE_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(40.59, 102.412)), module, Billboard::FORCE_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(10.571, 102.415)), module, Billboard::FORCE_X_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(54.931, 106.642)), module, Billboard::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(54.931, 106.642)), module, Billboard::ACTIVE_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(19.394, 45.949)), module, Billboard::FADE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(102.262, 45.949)), module, Billboard::GROWTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.041, 53.076)), module, Billboard::LIFE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(65.626, 53.076)), module, Billboard::SPAWN_RATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(88.276, 53.076)), module, Billboard::SIZE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.962, 60.126)), module, Billboard::FUZZ_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(11.267, 77.589)), module, Billboard::GLOW_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(41.116, 85.636)), module, Billboard::TURBULENCE_DELAY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(80.545, 86.558)), module, Billboard::TURBULENCE_SCALE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(65.703, 92.764)), module, Billboard::TURBULENCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(66.483, 106.642)), module, Billboard::ACTIVE_INPUT));
		addPointCloudInput(mm2px(Vec(98.317, 107.999)), module, Billboard::POINT_CLOUD_INPUT, "PointCloudInput");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.571, 120.319)), module, Billboard::FORCE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.589, 120.321)), module, Billboard::FORCE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(40.598, 120.321)), module, Billboard::FORCE_Z_INPUT));

		addPointCloudOutput(mm2px(Vec(112.515, 107.999)), module, Billboard::POINT_CLOUD_OUTPUT, "PointCloudOutput");
	}
};


Model* modelBillboard = createModel<Billboard, BillboardWidget>("Billboard");