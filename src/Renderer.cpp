#include "UModule.hpp"

struct Renderer : URack::UModule {
	enum ParamIds { NUM_PARAMS };
	enum InputIds { POINT_CLOUD_INPUT, NUM_INPUTS };
	enum OutputIds { NUM_OUTPUTS };
	enum LightIds { NUM_LIGHTS };

	Renderer() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }
};

struct RendererWidget : URack::UModuleWidget {
	RendererWidget(Renderer* module) {
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

		addPointCloudInput(mm2px(Vec(30, 119.964)), module,
				Renderer::POINT_CLOUD_INPUT, "PointCloudInput");
	}
};

Model* modelRenderer = createModel<Renderer, RendererWidget>("Renderer");
