#include "UModule.hpp"

struct Billboard : URack::UModule {
	enum ParamIds {
		LIFE_PARAM,
		SPAWN_RATE_PARAM,
		SIZE_PARAM,
		FADE_PARAM,
		GROWTH_PARAM,
		LIFE_ATTEN_PARAM,
		SPAWN_RATE_ATTEN_PARAM,
		SIZE_ATTEN_PARAM,
		TURBULENCE_SCALE_PARAM,
		FORCE_Y_PARAM,
		FORCE_Z_PARAM,
		FORCE_X_PARAM,
		TURBULENCE_PARAM,
		TURBULENCE_DELAY_PARAM,
		TURBULENCE_ATTEN_PARAM,
		FUZZ_PARAM,
		ACTIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		FADE_INPUT,
		LIFE_INPUT,
		SPAWN_RATE_INPUT,
		SIZE_INPUT,
		GROWTH_INPUT,
		TURBULENCE_SCALE_INPUT,
		FORCE_X_INPUT,
		FORCE_Y_INPUT,
		FORCE_Z_INPUT,
		TURBULENCE_DELAY_INPUT,
		TURBULENCE_INPUT,
		ACTIVE_INPUT,
		POINT_CLOUD_INPUT,
		FUZZ_INPUT,
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
		configUpdate("Life", LIFE_PARAM, LIFE_INPUT, LIFE_ATTEN_PARAM, 3.f);
		configUpdate("SpawnRate", SPAWN_RATE_PARAM, SPAWN_RATE_INPUT, SPAWN_RATE_ATTEN_PARAM, 5.f);
		configUpdate("Size", SIZE_PARAM, SIZE_INPUT, SIZE_ATTEN_PARAM, 2.f);
		configBiUpdate("Fade", FADE_PARAM, FADE_INPUT);
		configBiUpdate("Growth", GROWTH_PARAM, GROWTH_INPUT);
		configUpdate("TurbulenceScale", TURBULENCE_SCALE_PARAM, TURBULENCE_SCALE_INPUT, -1, 2.f);
		configBiUpdate("ForceY", FORCE_Y_PARAM, FORCE_Y_INPUT);
		configBiUpdate("ForceZ", FORCE_Z_PARAM, FORCE_Z_INPUT);
		configBiUpdate("ForceX", FORCE_X_PARAM, FORCE_X_INPUT);
		configUpdate("Turbulence", TURBULENCE_PARAM, TURBULENCE_INPUT, TURBULENCE_ATTEN_PARAM, 0.f);
		configBiUpdate("TurbulenceDelay", TURBULENCE_DELAY_PARAM, TURBULENCE_DELAY_INPUT);
		configUpdate("Fuzz", FUZZ_PARAM, FUZZ_INPUT, -1, 2.f);
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
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(28.307, 35.291)), module, Billboard::LIFE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(50.893, 35.291)), module, Billboard::SPAWN_RATE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(73.543, 35.291)), module, Billboard::SIZE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(9.492, 38.347)), module, Billboard::FADE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(92.359, 38.347)), module, Billboard::GROWTH_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(22.449, 56.251)), module, Billboard::LIFE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(45.035, 56.251)), module, Billboard::SPAWN_RATE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(67.685, 56.251)), module, Billboard::SIZE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(23.427, 71.721)), module, Billboard::TURBULENCE_SCALE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(77.35, 71.721)), module, Billboard::FORCE_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(92.359, 71.721)), module, Billboard::FORCE_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(62.34, 71.723)), module, Billboard::FORCE_X_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(41.467, 76.566)), module, Billboard::TURBULENCE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(9.492, 79.622)), module, Billboard::TURBULENCE_DELAY_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(35.608, 97.526)), module, Billboard::TURBULENCE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(25.562, 106.164)), module, Billboard::FUZZ_PARAM));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(44.901, 108.759)), module, Billboard::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(44.901, 108.759)), module, Billboard::ACTIVE_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.492, 56.251)), module, Billboard::FADE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.138, 56.251)), module, Billboard::LIFE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.723, 56.251)), module, Billboard::SPAWN_RATE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(78.374, 56.251)), module, Billboard::SIZE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.359, 56.251)), module, Billboard::GROWTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(23.427, 89.625)), module, Billboard::TURBULENCE_SCALE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(62.34, 89.627)), module, Billboard::FORCE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(77.358, 89.63)), module, Billboard::FORCE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(92.368, 89.63)), module, Billboard::FORCE_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.492, 97.526)), module, Billboard::TURBULENCE_DELAY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(46.297, 97.526)), module, Billboard::TURBULENCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(56.453, 108.759)), module, Billboard::ACTIVE_INPUT));
		addPointCloudInput(mm2px(Vec(74.504, 108.759)), module, Billboard::POINT_CLOUD_INPUT, "PointCloudInput");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.667, 111.875)), module, Billboard::FUZZ_INPUT));

		addPointCloudOutput(mm2px(Vec(88.702, 108.759)), module, Billboard::POINT_CLOUD_OUTPUT, "PointCloudOutput");
	}
};

Model* modelBillboard = createModel<Billboard, BillboardWidget>("Billboard");
