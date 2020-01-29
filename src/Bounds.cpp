#include "UModule.hpp"

struct Bounds : URack::UModule {
	enum ParamIds {
		MIN_BOUNDS_X_ATTEN_PARAM,
		MIN_BOUNDS_X_PARAM,
		MIN_BOUNDS_Y_ATTEN_PARAM,
		MIN_BOUNDS_Y_PARAM,
		MIN_BOUNDS_Z_ATTEN_PARAM,
		MIN_BOUNDS_Z_PARAM,
		MAX_BOUNDS_X_ATTEN_PARAM,
		MAX_BOUNDS_X_PARAM,
		MAX_BOUNDS_Y_ATTEN_PARAM,
		MAX_BOUNDS_Y_PARAM,
		MAX_BOUNDS_Z_ATTEN_PARAM,
		MAX_BOUNDS_Z_PARAM,
		ACTIVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		MIN_BOUNDS_X_INPUT,
		MIN_BOUNDS_Y_INPUT,
		MIN_BOUNDS_Z_INPUT,
		MAX_BOUNDS_X_INPUT,
		MAX_BOUNDS_Y_INPUT,
		MAX_BOUNDS_Z_INPUT,
		POINT_CLOUD_INPUT,
		ACTIVE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { POINTS_OUTSIDE_OUTPUT, POINTS_INSIDE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

	Bounds() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);

		configBiUpdate("MaxX", MAX_BOUNDS_X_PARAM, MAX_BOUNDS_X_INPUT,
				MAX_BOUNDS_X_ATTEN_PARAM, 5.f);
		configBiUpdate("MaxY", MAX_BOUNDS_Y_PARAM, MAX_BOUNDS_Y_INPUT,
				MAX_BOUNDS_Y_ATTEN_PARAM, 5.f);
		configBiUpdate("MaxZ", MAX_BOUNDS_Z_PARAM, MAX_BOUNDS_Z_INPUT,
				MAX_BOUNDS_Z_ATTEN_PARAM, 5.f);

		configBiUpdate("MinX", MIN_BOUNDS_X_PARAM, MIN_BOUNDS_X_INPUT,
				MIN_BOUNDS_X_ATTEN_PARAM, -5.f);
		configBiUpdate("MinY", MIN_BOUNDS_Y_PARAM, MIN_BOUNDS_Y_INPUT,
				MIN_BOUNDS_Y_ATTEN_PARAM, -5.f);
		configBiUpdate("MinZ", MIN_BOUNDS_Z_PARAM, MIN_BOUNDS_Z_INPUT,
				MIN_BOUNDS_Z_ATTEN_PARAM, -5.f);
	}
};

struct BoundsWidget : URack::UModuleWidget {
	BoundsWidget(Bounds* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/Bounds.svg")));

		addParam(createParamCentered<LEDBezel>(mm2px(Vec(16.92, 16.028)),
					module, Bounds::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<BlueLight>>(
					mm2px(Vec(16.92, 16.028)), module, Bounds::ACTIVE_LIGHT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(36, 16.0)), module,
					Bounds::ACTIVE_INPUT));

		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(16.92, 25.327)), module,
					Bounds::MIN_BOUNDS_X_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.035, 25.327)), module, Bounds::MIN_BOUNDS_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(16.983, 39.896)), module,
					Bounds::MIN_BOUNDS_Y_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.098, 39.896)), module, Bounds::MIN_BOUNDS_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(17.083, 54.465)), module,
					Bounds::MIN_BOUNDS_Z_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.198, 54.465)), module, Bounds::MIN_BOUNDS_Z_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(17.083, 75.256)), module,
					Bounds::MAX_BOUNDS_X_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.198, 75.256)), module, Bounds::MAX_BOUNDS_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(17.145, 89.825)), module,
					Bounds::MAX_BOUNDS_Y_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.26, 89.825)), module, Bounds::MAX_BOUNDS_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(17.245, 104.394)), module,
					Bounds::MAX_BOUNDS_Z_ATTEN_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.36, 104.394)), module, Bounds::MAX_BOUNDS_Z_PARAM));

		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.17, 25.327)), module, Bounds::MIN_BOUNDS_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.233, 39.896)), module, Bounds::MIN_BOUNDS_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.333, 54.465)), module, Bounds::MIN_BOUNDS_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.333, 75.256)), module, Bounds::MAX_BOUNDS_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.396, 89.825)), module, Bounds::MAX_BOUNDS_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.496, 104.394)), module, Bounds::MAX_BOUNDS_Z_INPUT));

		addPointCloudInput(mm2px(Vec(8.025, 120.652)), module,
				Bounds::POINT_CLOUD_INPUT, "PointCloud");

		addPointCloudOutput(mm2px(Vec(25.46, 120.652)), module,
				Bounds::POINTS_OUTSIDE_OUTPUT, "Outside");
		addPointCloudOutput(mm2px(Vec(39.368, 120.652)), module,
				Bounds::POINTS_INSIDE_OUTPUT, "Inside");
	};
};
Model* modelBounds = createModel<Bounds, BoundsWidget>("Bounds");
