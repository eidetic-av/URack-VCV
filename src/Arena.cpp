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
		SKY_COLOR_EXPOSURE_PARAM,
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
		SKY_COLOR_EXPOSURE_INPUT,
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
		configParam(CAMERA_FOCAL_LENGTH_PARAM, 0.f, 10.f, 5.f, "");
		configParam(LIGHT_BRIGHTNESS_PARAM, 0.f, 10.f, 0.f, "");
		configParam(LIGHT_DIRECTION_Y_PARAM, -5.f, 5.f, 0.f, "");
		configParam(SKY_COLOR_HARMONY_PARAM, 0.f, 10.f, 0.f, "");
		configParam(SKY_COLOR_EXPOSURE_PARAM, 0.f, 10.f, 0.f, "");

		configUpdate(CAMERA_ORIGIN_X_PARAM, CAMERA_ORIGIN_X_INPUT,
				"CameraOriginX");
		configUpdate(CAMERA_ORIGIN_Y_PARAM, CAMERA_ORIGIN_Y_INPUT,
				"CameraOriginY");
		configUpdate(CAMERA_ORIGIN_Z_PARAM, CAMERA_ORIGIN_Z_INPUT,
				"CameraOriginZ");
		configUpdate(CAMERA_DISTANCE_PARAM, CAMERA_DISTANCE_INPUT,
				"CameraDistance");
		configUpdate(CAMERA_ORBIT_X_PARAM, CAMERA_ORBIT_X_INPUT,
				"CameraOrbitX");
		configUpdate(CAMERA_ORBIT_Y_PARAM, CAMERA_ORBIT_Y_INPUT,
				"CameraOrbitY");
		configUpdate(CAMERA_FOCAL_LENGTH_PARAM, CAMERA_FOCAL_LENGTH_INPUT,
				"CameraFocalLength");
		configUpdate(LIGHT_HUE_PARAM, LIGHT_HUE_INPUT, "LightHue");
		configUpdate(LIGHT_SATURATION_PARAM, LIGHT_SATURATION_INPUT,
				"LightSaturation");
		configUpdate(LIGHT_BRIGHTNESS_PARAM, LIGHT_BRIGHTNESS_INPUT,
				"LightBrightness");
		configUpdate(SKY_COLOR_HARMONY_PARAM, SKY_COLOR_HARMONY_INPUT,
				"SkyColorHarmony");
		configUpdate(SKY_COLOR_PHASE_PARAM, SKY_COLOR_PHASE_INPUT,
				"SkyColorPhase");
		configUpdate(SKY_COLOR_DIFFUSION_PARAM, SKY_COLOR_DIFFUSION_INPUT,
				"SkyColorDiffusion");
		configUpdate(SKY_COLOR_EXPOSURE_PARAM, SKY_COLOR_EXPOSURE_INPUT,
				"SkyExposure");
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
					mm2px(Vec(99.541, 19.501)), module,
					Arena::CAMERA_FOCAL_LENGTH_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(69.783, 20.166)), module,
					Arena::LIGHT_DIRECTION_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(52.962, 20.44)), module, Arena::LIGHT_DIRECTION_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(19.325, 20.683)), module, Arena::LIGHT_HUE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(99.435, 36.782)), module, Arena::CAMERA_DISTANCE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(19.22, 37.964)), module, Arena::LIGHT_SATURATION_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(99.592, 54.541)), module, Arena::CAMERA_EXPOSURE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(19.376, 55.724)), module, Arena::LIGHT_BRIGHTNESS_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(40.233, 61.437)), module, Arena::CAMERA_ORBIT_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(63.425, 78.708)), module, Arena::CAMERA_ORBIT_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(97.9, 86.18)), module, Arena::SKY_COLOR_PHASE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(83.404, 89.743)), module,
					Arena::SKY_COLOR_HARMONY_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(53.885, 101.655)), module, Arena::CAMERA_ORIGIN_Z_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(98.159, 101.685)), module,
					Arena::SKY_COLOR_EXPOSURE_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(19.779, 101.784)), module, Arena::CAMERA_ORIGIN_X_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(37.226, 101.784)), module, Arena::CAMERA_ORIGIN_Y_PARAM));
		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(83.156, 107.106)), module,
					Arena::SKY_COLOR_DIFFUSION_PARAM));

		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(110.542, 19.242)), module,
					Arena::CAMERA_FOCAL_LENGTH_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(33.823, 20.321)), module,
					Arena::LIGHT_DIRECTION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(8.102, 20.424)), module, Arena::LIGHT_HUE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(110.437, 36.523)), module, Arena::CAMERA_DISTANCE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(7.997, 37.705)), module, Arena::LIGHT_SATURATION_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(33.365, 37.985)), module,
					Arena::LIGHT_DIRECTION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(110.593, 54.283)), module, Arena::CAMERA_EXPOSURE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(8.153, 55.465)), module, Arena::LIGHT_BRIGHTNESS_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(110.662, 71.16)), module,
					Arena::SKY_COLOR_HARMONY_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(19.381, 78.44)), module, Arena::CAMERA_ORBIT_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(39.676, 78.678)), module, Arena::CAMERA_ORBIT_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(110.546, 86.21)), module, Arena::SKY_COLOR_PHASE_INPUT));
		addInput(
				createInputCentered<PJ301MPort>(mm2px(Vec(110.029, 101.6)), module,
					Arena::SKY_COLOR_EXPOSURE_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(19.719, 118.366)), module, Arena::CAMERA_ORIGIN_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(37.097, 118.625)), module, Arena::CAMERA_ORIGIN_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(53.946, 118.625)), module, Arena::CAMERA_ORIGIN_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(
					mm2px(Vec(110.029, 119.109)), module,
					Arena::SKY_COLOR_DIFFUSION_INPUT));
	}
};

Model *modelArena = createModel<Arena, ArenaWidget>("Arena");
