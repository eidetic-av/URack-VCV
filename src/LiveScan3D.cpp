#include "UModule.hpp"

struct LiveScan3D : URack::UModule {
	enum ParamIds {
		MAX_BOUNDS_X_INPUT_SCALE_PARAM,
		MAX_BOUNDS_X_PARAM,
		MIN_BOUNDS_X_INPUT_SCALE_PARAM,
		MIN_BOUNDS_X_PARAM,
		MAX_BOUNDS_Y_INPUT_SCALE_PARAM,
		MAX_BOUNDS_Y_PARAM,
		MIN_BOUNDS_Y_INPUT_SCALE_PARAM,
		MIN_BOUNDS_Y_PARAM,
		MAX_BOUNDS_Z_INPUT_SCALE_PARAM,
		MAX_BOUNDS_Z_PARAM,
		MIN_BOUNDS_Z_INPUT_SCALE_PARAM,
		MIN_BOUNDS_Z_PARAM,
		TRANSLATE_X_INPUT_SCALE_PARAM,
		TRANSLATE_X_PARAM,
		SCALE_X_PARAM,
		SCALE_X_INPUT_SCALE_PARAM,
		TRANSLATE_Y_INPUT_SCALE_PARAM,
		TRANSLATE_Y_PARAM,
		SCALE_Y_PARAM,
		SCALE_Y_INPUT_SCALE_PARAM,
		TRANSLATE_Z_INPUT_SCALE_PARAM,
		TRANSLATE_Z_PARAM,
		SCALE_Z_PARAM,
		SCALE_Z_INPUT_SCALE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		MAX_BOUNDS_X_INPUT,
		MIN_BOUNDS_X_INPUT,
		MAX_BOUNDS_Y_INPUT,
		MIN_BOUNDS_Y_INPUT,
		MAX_BOUNDS_Z_INPUT,
		MIN_BOUNDS_Z_INPUT,
		SCALE_X_INPUT,
		TRANSLATE_X_INPUT,
		SCALE_Y_INPUT,
		TRANSLATE_Y_INPUT,
		SCALE_Z_INPUT,
		TRANSLATE_Z_INPUT,
		POINT_CLOUD_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { POINT_CLOUD_OUTPUT, NEW_FRAME_OUTPUT, NUM_OUTPUTS };
	enum LightIds { NUM_LIGHTS };

	LiveScan3D() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(MAX_BOUNDS_X_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MAX_BOUNDS_X_PARAM, -5.f, 5.f, 5.f, "");
		configParam(MIN_BOUNDS_X_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MIN_BOUNDS_X_PARAM, -5.f, 5.f, -5.f, "");
		configParam(MAX_BOUNDS_Y_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MAX_BOUNDS_Y_PARAM, -5.f, 5.f, 5.f, "");
		configParam(MIN_BOUNDS_Y_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MIN_BOUNDS_Y_PARAM, -5.f, 5.f, -5.f, "");
		configParam(MAX_BOUNDS_Z_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MAX_BOUNDS_Z_PARAM, -5.f, 5.f, 5.f, "");
		configParam(MIN_BOUNDS_Z_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(MIN_BOUNDS_Z_PARAM, -5.f, 5.f, -5.f, "");
		configParam(TRANSLATE_X_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(TRANSLATE_X_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SCALE_X_PARAM, -5.f, 5.f, 1.f, "");
		configParam(SCALE_X_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(TRANSLATE_Y_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(TRANSLATE_Y_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SCALE_Y_PARAM, -5.f, 5.f, 1.f, "");
		configParam(SCALE_Y_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(TRANSLATE_Z_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(TRANSLATE_Z_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SCALE_Z_PARAM, -5.f, 5.f, 1.f, "");
		configParam(SCALE_Z_INPUT_SCALE_PARAM, -5.f, 5.f, 0.f, "");

		configUpdate(MIN_BOUNDS_X_PARAM, MIN_BOUNDS_X_INPUT,
				MIN_BOUNDS_X_INPUT_SCALE_PARAM, "MinX");
	}

	void process(const ProcessArgs& args) override {}
};

struct LiveScan3DWidget : URack::UModuleWidget {
	LiveScan3DWidget(LiveScan3D* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/LiveScan3D.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(
				createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
						RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.65, 24.792)), module,
					LiveScan3D::MAX_BOUNDS_X_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.535, 24.792)), module,
					LiveScan3D::MAX_BOUNDS_X_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(16.92, 24.798)), module,
					LiveScan3D::MIN_BOUNDS_X_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.035, 24.798)), module,
					LiveScan3D::MIN_BOUNDS_X_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.587, 39.361)), module,
					LiveScan3D::MAX_BOUNDS_Y_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.472, 39.361)), module,
					LiveScan3D::MAX_BOUNDS_Y_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(16.983, 39.367)), module,
					LiveScan3D::MIN_BOUNDS_Y_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.098, 39.367)), module,
					LiveScan3D::MIN_BOUNDS_Y_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.487, 53.93)), module,
					LiveScan3D::MAX_BOUNDS_Z_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.372, 53.93)), module, LiveScan3D::MAX_BOUNDS_Z_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(17.083, 53.936)), module,
					LiveScan3D::MIN_BOUNDS_Z_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.198, 53.936)), module,
					LiveScan3D::MIN_BOUNDS_Z_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(16.92, 76.327)), module,
					LiveScan3D::TRANSLATE_X_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.035, 76.327)), module, LiveScan3D::TRANSLATE_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.571, 76.327)), module, LiveScan3D::SCALE_X_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.686, 76.327)), module,
					LiveScan3D::SCALE_X_INPUT_SCALE_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(16.983, 90.895)), module,
					LiveScan3D::TRANSLATE_Y_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.098, 90.895)), module, LiveScan3D::TRANSLATE_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.508, 90.895)), module, LiveScan3D::SCALE_Y_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.623, 90.895)), module,
					LiveScan3D::SCALE_Y_INPUT_SCALE_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(17.083, 105.464)), module,
					LiveScan3D::TRANSLATE_Z_INPUT_SCALE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(29.198, 105.464)), module,
					LiveScan3D::TRANSLATE_Z_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.408, 105.464)), module, LiveScan3D::SCALE_Z_PARAM));
		addParam(createParamCentered<Trimpot>(
					mm2px(Vec(64.523, 105.464)), module,
					LiveScan3D::SCALE_Z_INPUT_SCALE_PARAM));

		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(74.4, 24.792)), module, LiveScan3D::MAX_BOUNDS_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.17, 24.798)), module, LiveScan3D::MIN_BOUNDS_X_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(74.337, 39.361)), module,
					LiveScan3D::MAX_BOUNDS_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.233, 39.367)), module, LiveScan3D::MIN_BOUNDS_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(74.237, 53.93)), module, LiveScan3D::MAX_BOUNDS_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.333, 53.936)), module, LiveScan3D::MIN_BOUNDS_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(74.436, 76.327)), module, LiveScan3D::SCALE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.17, 76.327)), module, LiveScan3D::TRANSLATE_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(74.373, 90.895)), module, LiveScan3D::SCALE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.233, 90.895)), module, LiveScan3D::TRANSLATE_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(74.242, 105.464)), module, LiveScan3D::SCALE_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.333, 105.464)), module, LiveScan3D::TRANSLATE_Z_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(12.553, 288.464)), module, LiveScan3D::NEW_FRAME_OUTPUT));

		addPointCloudOutput(createOutputCentered<URack::PointCloudPort>(
					mm2px(Vec(10, 119.964)), module, LiveScan3D::POINT_CLOUD_OUTPUT));

		addPointCloudInput(createOutputCentered<URack::PointCloudPort>(
					mm2px(Vec(30, 119.964)), module, LiveScan3D::POINT_CLOUD_INPUT));
	}
};

Model* modelLiveScan3D =
createModel<LiveScan3D, LiveScan3DWidget>("LiveScan3D");
