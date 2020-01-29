#include "UModule.hpp"

struct LineRenderer : URack::UModule {
	enum ParamIds {
		LIFETIME_PARAM,
		SPAWN_RATE_PARAM,
		POINT_COUNT_PARAM,
		LIFETIME_ATTEN_PARAM,
		SPAWN_RATE_ATTEN_PARAM,
		POINT_COUNT_ATTEN_PARAM,
		FUZZ_PARAM,
		SIZE_PARAM,
		MIN_BOUNDS_Z_INPUT_SCALE_PARAM,
		FUZZ_ATTEN_PARAM,
		TURBULENCE_PARAM,
		SIZE_ATTEN_PARAM,
		FORCE_X_PARAM,
		TURBULENCE_ATTEN_PARAM,
		DELAY_PARAM,
		ACTIVE_PARAM,
		FORCE_X_ATTEN_PARAM,
		DELAY_ATTEN_PARAM,
		FORCE_Y_PARAM,
		FORCE_Z_PARAM,
		FORCE_Y_ATTEN_PARAM,
		SCROLL_ATTEN_PARAM,
		FORCE_Z_ATTEN_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		POINT_CLOUD_INPUT,
		LIFETIME_INPUT,
		SPAWN_RATE_INPUT,
		POINT_COUNT_INPUT,
		FUZZ_INPUT,
		SIZE_INPUT,
		TURBULENCE_INPUT,
		ACTIVE_INPUT,
		FORCE_X_INPUT,
		DELAY_INPUT,
		FORCE_Y_INPUT,
		SCROLL_INPUT,
		FORCE_Z_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { NUM_OUTPUTS };
	enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

	LineRenderer() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
		configUpdate("SpawnRateMultiplier", SPAWN_RATE_PARAM, SPAWN_RATE_INPUT,
				SPAWN_RATE_ATTEN_PARAM, 5.f);
		configUpdate("Size", SIZE_PARAM, SIZE_INPUT, SIZE_ATTEN_PARAM, 2.5f);
		configUpdate("PointCountMultiplier", POINT_COUNT_PARAM,
				POINT_COUNT_INPUT, POINT_COUNT_ATTEN_PARAM, 5.f);
		configUpdate("Lifetime", LIFETIME_PARAM, LIFETIME_INPUT,
				LIFETIME_ATTEN_PARAM, 5.f);
		configUpdate("Noise", FUZZ_PARAM, FUZZ_INPUT, FUZZ_ATTEN_PARAM, 1.5f);
		configUpdate("Turbulence", TURBULENCE_PARAM, TURBULENCE_INPUT,
				TURBULENCE_ATTEN_PARAM);
		configUpdate("TurbulenceScroll", SCROLL_ATTEN_PARAM, SCROLL_INPUT);
		configUpdate("TurbulenceDelay", DELAY_PARAM, DELAY_INPUT,
				DELAY_ATTEN_PARAM);
		configBiUpdate("TurbulenceScaleX", FORCE_X_PARAM, FORCE_X_INPUT,
				FORCE_X_ATTEN_PARAM);
		configBiUpdate("TurbulenceScaleY", FORCE_Y_PARAM, FORCE_Y_INPUT,
				FORCE_Y_ATTEN_PARAM);
		configBiUpdate("TurbulenceScaleZ", FORCE_Z_PARAM, FORCE_Z_INPUT,
				FORCE_Z_ATTEN_PARAM);
	}

	void update(const ProcessArgs& args) override {}
};

struct LineRendererWidget : URack::UModuleWidget {
	LineRendererWidget(LineRenderer* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/Renderer.svg")));

		addParam(createParamCentered<LEDBezel>(
					mm2px(Vec(33.338, 88.469)), module, LineRenderer::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<YellowLight>>(
					mm2px(Vec(33.338, 88.469)), module, LineRenderer::ACTIVE_LIGHT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(43.218, 88.469)), module, LineRenderer::ACTIVE_INPUT));

		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(57.655, 10.683)), module, LineRenderer::LIFETIME_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(35.646, 10.958)), module,
					LineRenderer::SPAWN_RATE_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(13.312, 23.383)), module,
					LineRenderer::POINT_COUNT_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(52.782, 27.539)), module,
					LineRenderer::LIFETIME_ATTEN_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(30.773, 27.814)), module,
					LineRenderer::SPAWN_RATE_ATTEN_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(8.439, 40.239)), module,
					LineRenderer::POINT_COUNT_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(35.098, 40.849)), module, LineRenderer::FUZZ_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(57.871, 41.378)), module, LineRenderer::SIZE_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(30.224, 57.705)),
					module,
					LineRenderer::FUZZ_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(16.215, 58.03)), module, LineRenderer::TURBULENCE_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(52.998, 58.234)),
					module,
					LineRenderer::SIZE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(60.689, 73.715)), module, LineRenderer::FORCE_X_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(11.341, 74.886)), module,
					LineRenderer::TURBULENCE_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(15.75, 86.521)), module, LineRenderer::DELAY_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(55.816, 90.571)), module,
					LineRenderer::FORCE_X_ATTEN_PARAM));
		addParam(createParamCentered<Trimpot>(mm2px(Vec(10.876, 103.377)),
					module,
					LineRenderer::DELAY_ATTEN_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(38.915, 103.536)), module, LineRenderer::FORCE_Y_PARAM));
		addParam(createParamCentered<Davies1900hWhiteKnob>(
					mm2px(Vec(60.272, 103.817)), module, LineRenderer::FORCE_Z_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(34.042, 120.392)), module,
					LineRenderer::FORCE_Y_ATTEN_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(10.595, 120.438)), module,
					LineRenderer::SCROLL_ATTEN_PARAM));
		addParam(
				createParamCentered<Trimpot>(mm2px(Vec(55.399, 120.673)), module,
					LineRenderer::FORCE_Z_ATTEN_PARAM));

		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(61.501, 27.539)), module, LineRenderer::LIFETIME_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(39.491, 27.814)), module,
					LineRenderer::SPAWN_RATE_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(17.157, 40.239)), module,
					LineRenderer::POINT_COUNT_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(38.943, 57.705)), module, LineRenderer::FUZZ_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(61.717, 58.234)), module, LineRenderer::SIZE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(20.06, 74.886)), module, LineRenderer::TURBULENCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(64.535, 90.571)), module, LineRenderer::FORCE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(19.595, 103.377)), module, LineRenderer::DELAY_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(42.76, 120.392)), module, LineRenderer::FORCE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(19.314, 120.438)), module, LineRenderer::SCROLL_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(64.117, 120.673)), module, LineRenderer::FORCE_Z_INPUT));

		addPointCloudInput(mm2px(Vec(7.567, 8.31)), module,
				LineRenderer::POINT_CLOUD_INPUT, "PointCloud");
	}
};

Model* modelLineRenderer =
createModel<LineRenderer, LineRendererWidget>("LineRenderer");
