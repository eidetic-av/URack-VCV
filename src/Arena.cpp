#include "UModule.hpp"

struct Arena : URack::UModule {
	enum ParamIds {
		CAMERA_ORIGIN_X_PARAM,
		CAMERA_ORIGIN_Y_PARAM,
		CAMERA_ORIGIN_Z_PARAM,
		CAMERA_DISTANCE_PARAM,
		CAMERA_ORBIT_X_PARAM,
		CAMERA_ORBIT_Y_PARAM,
		CAMERA_FOCAL_LENGTH_PARAM,
		CAMERA_EXPOSURE_PARAM,
		LIGHT_HUE_PARAM,
		LIGHT_SATURATION_PARAM,
		LIGHT_BRIGHTNESS_PARAM,
		LIGHT_DIRECTION_X_PARAM,
		LIGHT_DIRECTION_Y_PARAM,
		SKY_COLOR_PHASE_PARAM,
		SKY_COLOR_HARMONY_PARAM,
		SKY_COLOR_DIFFUSION_PARAM,
		SKY_EXPOSURE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CAMERA_ORIGIN_X_INPUT,
		CAMERA_ORIGIN_Y_INPUT,
		CAMERA_ORIGIN_Z_INPUT,
		CAMERA_DISTANCE_INPUT,
		CAMERA_ORBIT_X_INPUT,
		CAMERA_ORBIT_Y_INPUT,
		CAMERA_FOCAL_LENGTH_INPUT,
		CAMERA_EXPOSURE_INPUT,
		LIGHT_HUE_INPUT,
		LIGHT_SATURATION_INPUT,
		LIGHT_BRIGHTNESS_INPUT,
		LIGHT_DIRECTION_X_INPUT,
		LIGHT_DIRECTION_Y_INPUT,
		SKY_COLOR_PHASE_INPUT,
		SKY_COLOR_HARMONY_INPUT,
		SKY_COLOR_DIFFUSION_INPUT,
		SKY_EXPOSURE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { SINE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { BLINK_LIGHT, NUM_LIGHTS };

	Arena() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(CAMERA_ORIGIN_X_PARAM, -5.f, 5.f, 0.f, "");
		configParam(LIGHT_HUE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SKY_COLOR_PHASE_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SKY_COLOR_DIFFUSION_PARAM, 0.f, 10.f, 0.f, "");
		configParam(LIGHT_SATURATION_PARAM, 0.f, 10.f, 0.f, "");
		configParam(LIGHT_DIRECTION_X_PARAM, -5.f, 5.f, 0.f, "");
		configParam(CAMERA_ORIGIN_Y_PARAM, -5.f, 5.f, 0.f, "");
		configParam(CAMERA_ORBIT_X_PARAM, -5.f, 5.f, 0.f, "");
		configParam(CAMERA_ORBIT_Y_PARAM, -5.f, 5.f, 0.f, "");
		configParam(CAMERA_EXPOSURE_PARAM, 0.f, 10.f, 0.f, "");
		configParam(CAMERA_ORIGIN_Z_PARAM, -5.f, 5.f, 0.f, "");
		configParam(CAMERA_DISTANCE_PARAM, 0.f, 10.f, 0.f, "");
		configParam(CAMERA_FOCAL_LENGTH_PARAM, 0.f, 10.f, 0.f, "");
		configParam(LIGHT_BRIGHTNESS_PARAM, 0.f, 10.f, 0.f, "");
		configParam(LIGHT_DIRECTION_Y_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SKY_COLOR_HARMONY_PARAM, 0.f, 10.f, 0.f, "");
		configParam(SKY_EXPOSURE_PARAM, 0.f, 10.f, 0.f, "");
	}

	float updateTimer = -1;
	void process(const ProcessArgs &args) override {
		updateTimer += args.sampleTime;
		// only send updates over OSC every 100Hz
		if (updateTimer >= 0.01f) {
			sendUpdate(params[CAMERA_ORIGIN_X_PARAM].getValue() +
					inputs[CAMERA_ORIGIN_X_INPUT].getVoltage(),
					"CameraOriginX");
			sendUpdate(params[CAMERA_ORIGIN_Y_PARAM].getValue() +
					inputs[CAMERA_ORIGIN_Y_INPUT].getVoltage(),
					"CameraOriginY");
			sendUpdate(params[CAMERA_ORIGIN_Z_PARAM].getValue() +
					inputs[CAMERA_ORIGIN_Z_INPUT].getVoltage(),
					"CameraOriginZ");
			sendUpdate(params[CAMERA_DISTANCE_PARAM].getValue() +
					inputs[CAMERA_DISTANCE_INPUT].getVoltage(),
					"CameraDistance");
			sendUpdate(params[CAMERA_ORBIT_X_PARAM].getValue() +
					inputs[CAMERA_ORBIT_X_INPUT].getVoltage(),
					"CameraOrbitX");
			sendUpdate(params[CAMERA_ORBIT_Y_PARAM].getValue() +
					inputs[CAMERA_ORBIT_Y_INPUT].getVoltage(),
					"CameraOrbitY");
			sendUpdate(params[CAMERA_FOCAL_LENGTH_PARAM].getValue() +
					inputs[CAMERA_FOCAL_LENGTH_INPUT].getVoltage(),
					"CameraFocalLength");
			sendUpdate(params[LIGHT_HUE_PARAM].getValue() +
					inputs[LIGHT_HUE_INPUT].getVoltage(),
					"LightHue");
			sendUpdate(params[LIGHT_SATURATION_PARAM].getValue() +
					inputs[LIGHT_SATURATION_INPUT].getVoltage(),
					"LightSaturation");
			sendUpdate(params[LIGHT_BRIGHTNESS_PARAM].getValue() +
					inputs[LIGHT_BRIGHTNESS_INPUT].getVoltage(),
					"LightBrightness");
			sendUpdate(params[SKY_COLOR_HARMONY_PARAM].getValue() +
					inputs[SKY_COLOR_HARMONY_INPUT].getVoltage(),
					"SkyColorHarmony");
			sendUpdate(params[SKY_COLOR_PHASE_PARAM].getValue() +
					inputs[SKY_COLOR_PHASE_INPUT].getVoltage(),
					"SkyColorPhase");
			sendUpdate(params[SKY_COLOR_DIFFUSION_PARAM].getValue() +
					inputs[SKY_COLOR_DIFFUSION_INPUT].getVoltage(),
					"SkyColorDiffusion");
			sendUpdate(params[SKY_EXPOSURE_PARAM].getValue() +
					inputs[SKY_EXPOSURE_INPUT].getVoltage(),
					"SkyExposure");

			updateTimer -= 0.01f;
		}
	}
};

struct ArenaWidget : URack::UModuleWidget {
	ArenaWidget(Arena *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(
					asset::plugin(pluginInstance, "res/Arena.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(
					Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(
				createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
						RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(12.5, 29.837)), module, Arena::CAMERA_ORIGIN_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(28.349, 29.837)), module, Arena::CAMERA_ORBIT_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(60.49, 30.215)), module, Arena::LIGHT_HUE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(92.618, 45.339)), module, Arena::SKY_COLOR_PHASE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(108.466, 45.339)), module,
					Arena::SKY_COLOR_DIFFUSION_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(60.301, 45.722)), module, Arena::LIGHT_SATURATION_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(76.149, 45.722)), module,
					Arena::LIGHT_DIRECTION_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(12.122, 46.1)), module, Arena::CAMERA_ORIGIN_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(27.97, 46.1)), module, Arena::CAMERA_ORBIT_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(43.818, 46.1)), module, Arena::CAMERA_EXPOSURE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(11.933, 63.877)), module, Arena::CAMERA_ORIGIN_Z_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(27.781, 63.877)), module, Arena::CAMERA_DISTANCE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(43.629, 63.877)), module,
					Arena::CAMERA_FOCAL_LENGTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(60.928, 64.25)), module, Arena::LIGHT_BRIGHTNESS_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(76.776, 64.25)), module, Arena::LIGHT_DIRECTION_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(92.429, 64.345)), module,
					Arena::SKY_COLOR_HARMONY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(108.277, 64.345)), module, Arena::SKY_EXPOSURE_PARAM));

		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(11.768, 88.696)), module, Arena::CAMERA_ORIGIN_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(27.617, 88.696)), module, Arena::CAMERA_ORBIT_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(60.371, 88.696)), module, Arena::LIGHT_HUE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(11.768, 101.935)), module, Arena::CAMERA_ORIGIN_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(27.616, 101.935)), module, Arena::CAMERA_ORBIT_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(43.464, 101.935)), module, Arena::CAMERA_EXPOSURE_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(60.371, 101.935)), module,
					Arena::LIGHT_SATURATION_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(76.219, 101.935)), module,
					Arena::LIGHT_DIRECTION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(92.596, 101.935)), module, Arena::SKY_COLOR_PHASE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(108.444, 101.935)), module,
					Arena::SKY_COLOR_DIFFUSION_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(11.768, 114.799)), module, Arena::CAMERA_ORIGIN_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(27.616, 114.799)), module, Arena::CAMERA_DISTANCE_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(43.464, 114.799)), module,
					Arena::CAMERA_FOCAL_LENGTH_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(60.371, 114.799)), module,
					Arena::LIGHT_BRIGHTNESS_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(76.219, 114.799)), module,
					Arena::LIGHT_DIRECTION_Y_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(92.596, 114.799)), module,
					Arena::SKY_COLOR_HARMONY_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(108.444, 114.799)), module, Arena::SKY_EXPOSURE_INPUT));
	}
};

Model *modelArena = createModel<Arena, ArenaWidget>("Arena");
