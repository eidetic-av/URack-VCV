#include "UModule.hpp"
#include "dsp/digital.hpp"

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
		ACTIVE_PARAM,
		MARKER_ACTIVE_PARAM,
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
		ACTIVE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds { SINE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { ACTIVE_LIGHT, MARKER_ACTIVE_LIGHT, NUM_LIGHTS };

	bool markerActive;
	dsp::BooleanTrigger markerTrigger;

	Arena() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);

		configBiUpdate("CameraOriginX", CAMERA_ORIGIN_X_PARAM,
				CAMERA_ORIGIN_X_INPUT);
		configBiUpdate("CameraOriginY", CAMERA_ORIGIN_Y_PARAM,
				CAMERA_ORIGIN_Y_INPUT);
		configBiUpdate("CameraOriginZ", CAMERA_ORIGIN_Z_PARAM,
				CAMERA_ORIGIN_Z_INPUT);

		configUpdate("CameraDistance", CAMERA_DISTANCE_PARAM,
				CAMERA_DISTANCE_INPUT);
		configUpdate("CameraFocalLength", CAMERA_FOCAL_LENGTH_PARAM,
				CAMERA_FOCAL_LENGTH_INPUT);

		configBiUpdate("CameraOrbitX", CAMERA_ORBIT_X_PARAM,
				CAMERA_ORBIT_X_INPUT);
		configBiUpdate("CameraOrbitY", CAMERA_ORBIT_Y_PARAM,
				CAMERA_ORBIT_Y_INPUT);

		configBiUpdate("SkyColorPhase", SKY_COLOR_PHASE_PARAM,
				SKY_COLOR_PHASE_INPUT);
		configUpdate("SkyColorHarmony", SKY_COLOR_HARMONY_PARAM,
				SKY_COLOR_HARMONY_INPUT);
		configUpdate("SkyColorDiffusion", SKY_COLOR_DIFFUSION_PARAM,
				SKY_COLOR_DIFFUSION_INPUT);
		configUpdate("SkyExposure", SKY_COLOR_EXPOSURE_PARAM,
				SKY_COLOR_EXPOSURE_INPUT);
	}

	void update(const ProcessArgs &args) override {
		if (markerTrigger.process(params[MARKER_ACTIVE_PARAM].getValue() >
					0.f)) {
			markerActive ^= true;
			URack::Dispatcher::send(hostNum, instanceAddress + "/MarkerEnable",
					markerActive ? 10.f : 0.f);
		}
		lights[MARKER_ACTIVE_LIGHT].setBrightness(markerActive > 0.f ? 10.f
				: 0.f);
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

		addParam(createParamCentered<LEDBezel>(mm2px(Vec(30, 5)), module,
					Arena::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<YellowLight>>(
					mm2px(Vec(30, 5)), module, Arena::ACTIVE_LIGHT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50, 5)), module,
					Arena::ACTIVE_INPUT));

		addParam(createParamCentered<LEDBezel>(mm2px(Vec(80, 5)), module,
					Arena::MARKER_ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(
					mm2px(Vec(80, 5)), module, Arena::MARKER_ACTIVE_LIGHT));

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
