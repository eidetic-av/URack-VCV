#include "UModule.hpp"

struct Mirage2x : URack::UModule {
	enum ParamIds {
		WARP_DISTANCE_ATTEN_PARAM,
		TURBULENCE_AMOUNT_ATTEN_PARAM,
		TURBULENCE_FREQUENCY_ATTEN_PARAM,
		WARP_X_EULER_PARAM,
		WARP_DISTANCE_PARAM,
		TURBULENCE_AMOUNT_PARAM,
		TURBULENCE_FREQUENCY_PARAM,
		WARP_X_EULER_ATTEN_PARAM,
		WARP_SHARD_ATTEN_PARAM,
		TURBULENCE_X_PARAM,
		TURBULENCE_Y_PARAM,
		TURBULENCE_Z_PARAM,
		WARP_Y_EULER_PARAM,
		WARP_SHARD_PARAM,
		TURBULENCE_X_ATTEN_PARAM,
		TURBULENCE_Y_ATTEN_PARAM,
		TURBULENCE_Z_ATTEN_PARAM,
		WARP_Y_EULER_ATTEN_PARAM,
		TEXTURE_SELECT_A_ATTEN_PARAM,
		TEXTURE_SELECT_B_ATTEN_PARAM,
		GLOW_PARAM,
		TEXTURE_SELECT_A_PARAM,
		TEXTURE_SELECT_B_PARAM,
		ACTIVE_PARAM,
		TEXTURE_SCALE_PARAM,
		SIMULATION_SPEED_PARAM,
		TEXTURE_SCALE_ATTEN_PARAM,
		SIMULATION_SPEED_ATTEN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		WARP_DISTANCE_INPUT,
		TURBULENCE_AMOUNT_INPUT,
		TURBULENCE_FREQUENCY_INPUT,
		LENGTH_INPUT,
		WARP_SHARD_INPUT,
		TURBULENCE_X_INPUT,
		TURBULENCE_Y_INPUT,
		TURBULENCE_Z_INPUT,
		WARP_Y_EULER_INPUT,
		GLOW_INPUT,
		TEXTURE_SELECT_A_INPUT,
		TEXTURE_SELECT_B_INPUT,
		ACTIVE_INPUT,
		TEXTURE_SCALE_INPUT,
		SIMULATION_SPEED_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	Mirage2x() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configUpdate("WarpXEuler", WARP_X_EULER_PARAM);
		configUpdate("WarpDistance", WARP_DISTANCE_PARAM, WARP_DISTANCE_INPUT, WARP_DISTANCE_ATTEN_PARAM, 0.f);
		configUpdate("TurbulenceAmount", TURBULENCE_AMOUNT_PARAM, TURBULENCE_AMOUNT_INPUT, TURBULENCE_AMOUNT_ATTEN_PARAM, 0.f);
		configUpdate("TurbulenceFrequency", TURBULENCE_FREQUENCY_PARAM, TURBULENCE_FREQUENCY_INPUT, TURBULENCE_FREQUENCY_ATTEN_PARAM, 0.f);
		configUpdate("TurbulenceX", TURBULENCE_X_PARAM, TURBULENCE_X_INPUT, TURBULENCE_X_ATTEN_PARAM, 0.f);
		configUpdate("TurbulenceY", TURBULENCE_Y_PARAM, TURBULENCE_Y_INPUT, TURBULENCE_Y_ATTEN_PARAM, 0.f);
		configUpdate("TurbulenceZ", TURBULENCE_Z_PARAM, TURBULENCE_Z_INPUT, TURBULENCE_Z_ATTEN_PARAM, 0.f);
		configUpdate("WarpYEuler", WARP_Y_EULER_PARAM, WARP_Y_EULER_INPUT, WARP_Y_EULER_ATTEN_PARAM, 0.f);
		configUpdate("WarpShard", WARP_SHARD_PARAM, WARP_SHARD_INPUT, WARP_SHARD_ATTEN_PARAM, 0.f);
		configUpdate("Glow", GLOW_PARAM, GLOW_INPUT);
		configUpdate("TextureSelectA", TEXTURE_SELECT_A_PARAM, TEXTURE_SELECT_A_INPUT, TEXTURE_SELECT_A_ATTEN_PARAM, 0.f);
		configUpdate("TextureSelectB", TEXTURE_SELECT_B_PARAM, TEXTURE_SELECT_B_INPUT, TEXTURE_SELECT_B_ATTEN_PARAM, 0.f);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
		configUpdate("TextureScale", TEXTURE_SCALE_PARAM, TEXTURE_SCALE_INPUT, TEXTURE_SCALE_ATTEN_PARAM, 0.f);
		configUpdate("SimulationSpeed", SIMULATION_SPEED_PARAM, SIMULATION_SPEED_INPUT, SIMULATION_SPEED_ATTEN_PARAM, 0.f);
	}

	void update(const ProcessArgs& args) override {
	}
};


struct Mirage2xWidget : URack::UModuleWidget {
	Mirage2xWidget(Mirage2x* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Mirage2x.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,	RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(97.013, 33.848)), module, Mirage2x::WARP_DISTANCE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(16.076, 35.965)), module, Mirage2x::TURBULENCE_AMOUNT_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(48.049, 36.639)), module, Mirage2x::TURBULENCE_FREQUENCY_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(115.52, 37.6)), module, Mirage2x::WARP_X_EULER_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(85.298, 40.238)), module, Mirage2x::WARP_DISTANCE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(27.791, 42.355)), module, Mirage2x::TURBULENCE_AMOUNT_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(59.764, 43.029)), module, Mirage2x::TURBULENCE_FREQUENCY_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(110.328, 47.045)), module, Mirage2x::WARP_X_EULER_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(97.013, 57.948)), module, Mirage2x::WARP_SHARD_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(16.382, 61.168)), module, Mirage2x::TURBULENCE_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(37.759, 61.168)), module, Mirage2x::TURBULENCE_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(58.757, 61.168)), module, Mirage2x::TURBULENCE_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(115.52, 61.7)), module, Mirage2x::WARP_Y_EULER_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(85.298, 64.338)), module, Mirage2x::WARP_SHARD_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(21.574, 70.613)), module, Mirage2x::TURBULENCE_X_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(42.951, 70.613)), module, Mirage2x::TURBULENCE_Y_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(63.949, 70.613)), module, Mirage2x::TURBULENCE_Z_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(110.328, 71.146)), module, Mirage2x::WARP_Y_EULER_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(14.552, 97.233)), module, Mirage2x::TEXTURE_SELECT_A_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(87.312, 97.233)), module, Mirage2x::TEXTURE_SELECT_B_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(109.82, 98.549)), module, Mirage2x::GLOW_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(26.267, 103.623)), module, Mirage2x::TEXTURE_SELECT_A_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(mm2px(Vec(75.597, 103.623)), module, Mirage2x::TEXTURE_SELECT_B_PARAM));
                addParam(createParamCentered<LEDBezel>(mm2px(Vec(109.889, 118.282)), module, Mirage2x::ACTIVE_PARAM));
                addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(109.889, 118.282)), module, Mirage2x::ACTIVE_LIGHT));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(30.557, 119.348)), module, Mirage2x::TEXTURE_SCALE_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(71.307, 119.348)), module, Mirage2x::SIMULATION_SPEED_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(19.843, 119.654)), module, Mirage2x::TEXTURE_SCALE_ATTEN_PARAM));
		addParam(createParamCentered<TrimpotGray>(mm2px(Vec(82.02, 119.654)), module, Mirage2x::SIMULATION_SPEED_ATTEN_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(101.077, 42.375)), module, Mirage2x::WARP_DISTANCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.012, 44.491)), module, Mirage2x::TURBULENCE_AMOUNT_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(43.985, 45.165)), module, Mirage2x::TURBULENCE_FREQUENCY_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(119.684, 47.045)), module, Mirage2x::LENGTH_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(101.077, 66.475)), module, Mirage2x::WARP_SHARD_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.219, 70.613)), module, Mirage2x::TURBULENCE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(33.595, 70.613)), module, Mirage2x::TURBULENCE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(54.593, 70.613)), module, Mirage2x::TURBULENCE_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(119.684, 71.146)), module, Mirage2x::WARP_Y_EULER_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(120.858, 98.549)), module, Mirage2x::GLOW_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.488, 105.759)), module, Mirage2x::TEXTURE_SELECT_A_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(91.376, 105.759)), module, Mirage2x::TEXTURE_SELECT_B_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(120.928, 118.282)), module, Mirage2x::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(10.488, 119.654)), module, Mirage2x::TEXTURE_SCALE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(91.376, 119.654)), module, Mirage2x::SIMULATION_SPEED_INPUT));
	}
};


Model* modelMirage2x = createModel<Mirage2x, Mirage2xWidget>("Mirage2x");