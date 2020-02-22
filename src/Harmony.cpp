#include "UModule.hpp"

struct Harmony : URack::UModule {
	enum ParamIds {
		COLOR_HARMONY_PARAM,
		PHASE_PARAM,
		COLOR_HARMONY_ATTEN_PARAM,
		PHASE_ATTEN_PARAM,
		DIFFUSION_PARAM,
		GLOW_PARAM,
		DIFFUSION_ATTEN_PARAM,
		GLOW_ATTEN_PARAM,
		ACTIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		COLOR_HARMONY_INPUT,
		PHASE_INPUT,
		DIFFUSION_INPUT,
		GLOW_INPUT,
		ACTIVE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	Harmony() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configUpdate("ColorHarmony", COLOR_HARMONY_PARAM, COLOR_HARMONY_INPUT, COLOR_HARMONY_ATTEN_PARAM, 0.f);
		configUpdate("Phase", PHASE_PARAM, PHASE_INPUT, PHASE_ATTEN_PARAM, 0.f);
		configUpdate("Diffusion", DIFFUSION_PARAM, DIFFUSION_INPUT, DIFFUSION_ATTEN_PARAM, 0.f);
		configUpdate("Glow", GLOW_PARAM, GLOW_INPUT, GLOW_ATTEN_PARAM, 0.f);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct HarmonyWidget : URack::UModuleWidget {
	HarmonyWidget(Harmony* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Harmony.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(14.081, 35.289)), module, Harmony::COLOR_HARMONY_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(36.719, 35.289)), module, Harmony::PHASE_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(8.223, 56.249)), module, Harmony::COLOR_HARMONY_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(30.86, 56.249)), module, Harmony::PHASE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(14.081, 75.028)), module, Harmony::DIFFUSION_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(36.719, 75.028)), module, Harmony::GLOW_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(8.223, 95.988)), module, Harmony::DIFFUSION_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(30.86, 95.988)), module, Harmony::GLOW_ATTEN_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(19.371, 108.759)), module, Harmony::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(19.371, 108.759)), module, Harmony::ACTIVE_LIGHT));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.911, 56.249)), module, Harmony::COLOR_HARMONY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(41.549, 56.249)), module, Harmony::PHASE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.911, 95.988)), module, Harmony::DIFFUSION_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(41.549, 95.988)), module, Harmony::GLOW_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(30.923, 108.759)), module, Harmony::ACTIVE_INPUT));
	}
};


Model* modelHarmony = createModel<Harmony, HarmonyWidget>("Harmony");