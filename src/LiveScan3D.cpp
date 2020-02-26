#include "UModule.hpp"

struct LiveScan3D : URack::UModule {
	enum ParamIds {
		MIN_LOP_X_PARAM,
		MAX_LOP_X_PARAM,
		LOCATION_X_PARAM,
		MIN_LOP_Y_PARAM,
		MAX_LOP_Y_PARAM,
		LOCATION_Y_PARAM,
		MIN_LOP_Z_PARAM,
		MAX_LOP_Z_PARAM,
		LOCATION_Z_PARAM,
		ROTATION_X_PARAM,
		SCALING_X_PARAM,
		ACTIVE_PARAM,
		ROTATION_Y_PARAM,
		SCALING_Y_PARAM,
		ROTATION_Z_PARAM,
		SCALING_Z_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		MIN_LOP_X_INPUT,
		MAX_LOP_X_INPUT,
		LOCATION_X_INPUT,
		MIN_LOP_Y_INPUT,
		MAX_LOP_Y_INPUT,
		LOCATION_Y_INPUT,
		MIN_LOP_Z_INPUT,
		MAX_LOP_Z_INPUT,
		LOCATION_Z_INPUT,
		ROTATION_X_INPUT,
		SCALING_X_INPUT,
		ACTIVE_INPUT,
		ROTATION_Y_INPUT,
		SCALING_Y_INPUT,
		ROTATION_Z_INPUT,
		SCALING_Z_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		POINT_CLOUD_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		ACTIVE_LIGHT,
		NUM_LIGHTS
	};

	LiveScan3D() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configBiUpdate("MinLopX", MIN_LOP_X_PARAM, MIN_LOP_X_INPUT, -1, -5);
		configBiUpdate("MaxLopX", MAX_LOP_X_PARAM, MAX_LOP_X_INPUT, -1, 5);
		configBiUpdate("LocationX", LOCATION_X_PARAM, LOCATION_X_INPUT);
		configBiUpdate("MinLopY", MIN_LOP_Y_PARAM, MIN_LOP_Y_INPUT, -1, -5);
		configBiUpdate("MaxLopY", MAX_LOP_Y_PARAM, MAX_LOP_Y_INPUT, -1, 5);
		configBiUpdate("LocationY", LOCATION_Y_PARAM, LOCATION_Y_INPUT);
		configBiUpdate("MinLopZ", MIN_LOP_Z_PARAM, MIN_LOP_Z_INPUT, -1, -5);
		configBiUpdate("MaxLopZ", MAX_LOP_Z_PARAM, MAX_LOP_Z_INPUT, -1, 5);
		configBiUpdate("LocationZ", LOCATION_Z_PARAM, LOCATION_Z_INPUT);
		configBiUpdate("RotationX", ROTATION_X_PARAM, ROTATION_X_INPUT);
		configBiUpdate("ScalingX", SCALING_X_PARAM, SCALING_X_INPUT, -1, 1);
		configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
		configBiUpdate("RotationY", ROTATION_Y_PARAM, ROTATION_Y_INPUT);
		configBiUpdate("ScalingY", SCALING_Y_PARAM, SCALING_Y_INPUT, -1, 1);
		configBiUpdate("RotationZ", ROTATION_Z_PARAM, ROTATION_Z_INPUT);
		configBiUpdate("ScalingZ", SCALING_Z_PARAM, SCALING_Z_INPUT, -1, 1);
	}

	void update(const ProcessArgs& args) override {
	}
};

struct LiveScan3DWidget : URack::UModuleWidget {
	LiveScan3DWidget(LiveScan3D* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LiveScan3D.svg")));

		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.392, 32.135)), module, LiveScan3D::MIN_LOP_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 32.142)), module, LiveScan3D::MAX_LOP_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(69.97, 32.142)), module, LiveScan3D::LOCATION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.392, 45.863)), module, LiveScan3D::MIN_LOP_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 45.871)), module, LiveScan3D::MAX_LOP_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(69.97, 45.871)), module, LiveScan3D::LOCATION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.386, 59.6)), module, LiveScan3D::MIN_LOP_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 59.6)), module, LiveScan3D::MAX_LOP_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(69.97, 59.6)), module, LiveScan3D::LOCATION_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.392, 80.231)), module, LiveScan3D::ROTATION_X_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 80.239)), module, LiveScan3D::SCALING_X_PARAM));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(69.589, 83.967)), module, LiveScan3D::ACTIVE_PARAM));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(69.589, 83.967)), module, LiveScan3D::ACTIVE_LIGHT));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.392, 93.96)), module, LiveScan3D::ROTATION_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 93.968)), module, LiveScan3D::SCALING_Y_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(21.392, 107.689)), module, LiveScan3D::ROTATION_Z_PARAM));
		addParam(createParamCentered<Davies1900hSmallWhiteKnob>(mm2px(Vec(42.695, 107.696)), module, LiveScan3D::SCALING_Z_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.196, 32.135)), module, LiveScan3D::MIN_LOP_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 32.142)), module, LiveScan3D::MAX_LOP_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(83.25, 32.142)), module, LiveScan3D::LOCATION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.196, 45.863)), module, LiveScan3D::MIN_LOP_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 45.871)), module, LiveScan3D::MAX_LOP_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(83.25, 45.871)), module, LiveScan3D::LOCATION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.19, 59.6)), module, LiveScan3D::MIN_LOP_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 59.6)), module, LiveScan3D::MAX_LOP_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(83.25, 59.6)), module, LiveScan3D::LOCATION_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.196, 80.231)), module, LiveScan3D::ROTATION_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 80.239)), module, LiveScan3D::SCALING_X_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(81.141, 83.967)), module, LiveScan3D::ACTIVE_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.196, 93.96)), module, LiveScan3D::ROTATION_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 93.968)), module, LiveScan3D::SCALING_Y_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.196, 107.689)), module, LiveScan3D::ROTATION_Z_INPUT));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.975, 107.696)), module, LiveScan3D::SCALING_Z_INPUT));

		addPointCloudOutput(mm2px(Vec(75.613, 102.992)), module, LiveScan3D::POINT_CLOUD_OUTPUT, "PointCloudOutput");
	}
};

Model* modelLiveScan3D = createModel<LiveScan3D, LiveScan3DWidget>("LiveScan3D");
