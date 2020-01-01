#include "UModule.hpp"

struct Arena : URack::UModule {
	enum ParamIds { PITCH_PARAM, NUM_PARAMS };
	enum InputIds { PITCH_INPUT, NUM_INPUTS };
	enum OutputIds { SINE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { BLINK_LIGHT, NUM_LIGHTS };

	Arena() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
	}

	void update(const ProcessArgs &args) override {}
};

struct ArenaWidget : URack::UModuleWidget {
	ArenaWidget(Arena *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/MyModule.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(
				createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
						RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 77.478)),
					module, Arena::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)),
					module, Arena::SINE_OUTPUT));

		// testPort = new PointCloudPortWidget();
		// testPort->setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/PJ301M.svg")));
		// addChild(testPort);
	}
};

Model *modelArena = createModel<Arena, ArenaWidget>("Arena");
