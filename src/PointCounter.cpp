#include "UModule.hpp"

struct PointCounter : URack::UModule {
	enum ParamIds { NUM_PARAMS };
	enum InputIds { POINTS_INPUT, NUM_INPUTS };
	enum OutputIds { TEST_OUTPUT, COUNT_OUTPUT, POINTS_OUTPUT, NUM_OUTPUTS };
	enum LightIds { NUM_LIGHTS };

	PointCounter() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configListener("PointCount", COUNT_OUTPUT);
	}
};

struct PointCounterWidget : URack::UModuleWidget {
	PointCounterWidget(PointCounter* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/PointCounter.svg")));

		addPointCloudInput(mm2px(Vec(5.177, 50.713)), module,
				PointCounter::POINTS_INPUT, "PointCloud");

		addOutput(createOutputCentered<PJ301MPort>(
					mm2px(Vec(5.585, 73.107)), module, PointCounter::COUNT_OUTPUT));

		addPointCloudOutput(mm2px(Vec(4.869, 109.402)), module,
				PointCounter::POINTS_OUTPUT, "PointCloudThru");
	}
};

Model* modelPointCounter =
createModel<PointCounter, PointCounterWidget>("PointCounter");
