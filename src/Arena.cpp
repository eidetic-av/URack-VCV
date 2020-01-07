#include "UModule.hpp"

struct Arena : URack::UModule {
	enum ParamIds { NUM_PARAMS };
	enum InputIds {
		CAMERA_POS_X_INPUT,
		CAMERA_POS_Y_INPUT,
		CAMERA_POS_Z_INPUT,
		CAMERA_ROT_X_INPUT,
		CAMERA_ROT_Y_INPUT,
		CAMERA_ROT_Z_INPUT,
		CAMERA_ORIGIN_X_INPUT,
		CAMERA_ORIGIN_Y_INPUT,
		CAMERA_ORIGIN_Z_INPUT,
		CAMERA_FOCAL_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { SINE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { BLINK_LIGHT, NUM_LIGHTS };

	Arena() { config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS); }

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

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 10)), module,
					Arena::CAMERA_POS_X_INPUT),
				"CameraPositionX");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 20)), module,
					Arena::CAMERA_POS_Y_INPUT),
				"CameraPositionY");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 30)), module,
					Arena::CAMERA_POS_Z_INPUT),
				"CameraPositionZ");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 45)), module,
					Arena::CAMERA_ROT_X_INPUT),
				"CameraRotationX");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 55)), module,
					Arena::CAMERA_ROT_Y_INPUT),
				"CameraRotationY");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 65)), module,
					Arena::CAMERA_ROT_Z_INPUT),
				"CameraRotationZ");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 80)), module,
					Arena::CAMERA_ORIGIN_X_INPUT),
				"CameraOriginX");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 90)), module,
					Arena::CAMERA_ORIGIN_Y_INPUT),
				"CameraOriginY");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 100)), module,
					Arena::CAMERA_ORIGIN_Z_INPUT),
				"CameraOriginZ");
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 115)), module,
					Arena::CAMERA_FOCAL_INPUT),
				"CameraFocalLength");
	}
};

Model *modelArena = createModel<Arena, ArenaWidget>("Arena");
