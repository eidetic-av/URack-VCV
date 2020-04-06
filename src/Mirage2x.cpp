#include "UModule.hpp"

struct Mirage2x : URack::UModule {
    enum ParamIds {
        ACTIVE_PARAM,
        GLOW_ATTEN_PARAM,
        SIMULATION_SPEED_ATTEN_PARAM,
        TURBULENCE_AMOUNT_ATTEN_PARAM,
        WARP_DISTANCE_ATTEN_PARAM,
        TURBULENCE_X_SCALE_PARAM,
        WARP_X_EULER_PARAM,
        GLOW_PARAM,
        SIMULATION_SPEED_PARAM,
        TURBULENCE_AMOUNT_PARAM,
        WARP_DISTANCE_PARAM,
        TURBULENCE_X_SCALE_ATTEN_PARAM,
        WARP_X_EULER_ATTEN_PARAM,
        PARTICLE_SIZE_ATTEN_PARAM,
        PARTICLE_SHAPE_ATTEN_PARAM,
        TURBULENCE_FREQUENCY_ATTEN_PARAM,
        WARP_SHARD_ATTEN_PARAM,
        PARTICLE_SIZE_PARAM,
        PARTICLE_SHAPE_PARAM,
        TURBULENCE_Y_SCALE_PARAM,
        WARP_Y_EULER_PARAM,
        TURBULENCE_FREQUENCY_PARAM,
        WARP_SHARD_PARAM,
        TURBULENCE_Y_SCALE_ATTEN_PARAM,
        WARP_Y_EULER_ATTEN_PARAM,
        POSITION_X_PARAM,
        POSITION_Y_PARAM,
        POSITION_Z_PARAM,
        TEXTURE_SELECT_A_ATTEN_PARAM,
        TEXTURE_SELECT_B_ATTEN_PARAM,
        ROTATION_X_PARAM,
        ROTATION_Y_PARAM,
        ROTATION_Z_PARAM,
        TEXTURE_SELECT_A_PARAM,
        TEXTURE_SELECT_B_PARAM,
        GAIN_RED_PARAM,
        GAIN_GREEN_PARAM,
        GAIN_BLUE_PARAM,
        TEXTURE_SCALE_PARAM,
        TEXTURE_SCALE_ATTEN_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        ACTIVE_INPUT,
        GLOW_INPUT,
        SIMULATION_SPEED_INPUT,
        TURBULENCE_AMOUNT_INPUT,
        WARP_DISTANCE_INPUT,
        TURBULENCE_X_SCALE_INPUT,
        LENGTH_INPUT,
        PARTICLE_SIZE_INPUT,
        PARTICLE_SHAPE_INPUT,
        TURBULENCE_FREQUENCY_INPUT,
        WARP_SHARD_INPUT,
        TURBULENCE_Y_SCALE_INPUT,
        WARP_Y_EULER_INPUT,
        WARP_X_EULER_INPUT,
        POSITION_X_INPUT,
        POSITION_Y_INPUT,
        POSITION_Z_INPUT,
        ROTATION_X_INPUT,
        ROTATION_Y_INPUT,
        ROTATION_Z_INPUT,
        TEXTURE_SELECT_A_INPUT,
        TEXTURE_SELECT_B_INPUT,
        GAIN_RED_INPUT,
        GAIN_GREEN_INPUT,
        GAIN_BLUE_INPUT,
        TEXTURE_SCALE_INPUT,
        NUM_INPUTS
    };
    enum OutputIds { NUM_OUTPUTS };
    enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

    Mirage2x() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
        configUpdate("TurbulenceXScale", TURBULENCE_X_SCALE_PARAM,
                     TURBULENCE_X_SCALE_INPUT, TURBULENCE_X_SCALE_ATTEN_PARAM,
                     5.f);
        configBiUpdate("WarpXEuler", WARP_X_EULER_PARAM,
                       WARP_X_EULER_ATTEN_PARAM, WARP_X_EULER_INPUT, 0.f);
        configUpdate("Glow", GLOW_PARAM, GLOW_INPUT, GLOW_ATTEN_PARAM, 0.f);
        configUpdate("SimulationSpeed", SIMULATION_SPEED_PARAM,
                     SIMULATION_SPEED_INPUT, SIMULATION_SPEED_ATTEN_PARAM, 5.f);
        configUpdate("TurbulenceAmount", TURBULENCE_AMOUNT_PARAM,
                     TURBULENCE_AMOUNT_INPUT, TURBULENCE_AMOUNT_ATTEN_PARAM,
                     0.f);
        configBiUpdate("WarpDistance", WARP_DISTANCE_PARAM, WARP_DISTANCE_INPUT,
                       WARP_DISTANCE_ATTEN_PARAM, 0.f);
        configUpdate("ParticleSize", PARTICLE_SIZE_PARAM, PARTICLE_SIZE_INPUT,
                     PARTICLE_SIZE_ATTEN_PARAM, 5.f);
        configUpdate("ParticleShape", PARTICLE_SHAPE_PARAM,
                     PARTICLE_SHAPE_INPUT, PARTICLE_SHAPE_ATTEN_PARAM, 0.f);
        configUpdate("TurbulenceYScale", TURBULENCE_Y_SCALE_PARAM,
                     TURBULENCE_Y_SCALE_INPUT, TURBULENCE_Y_SCALE_ATTEN_PARAM,
                     5.f);
        configBiUpdate("WarpYEuler", WARP_Y_EULER_PARAM, WARP_Y_EULER_INPUT,
                       WARP_Y_EULER_ATTEN_PARAM, 0.f);
        configUpdate("TurbulenceFrequency", TURBULENCE_FREQUENCY_PARAM,
                     TURBULENCE_FREQUENCY_INPUT,
                     TURBULENCE_FREQUENCY_ATTEN_PARAM, 5.f);
        configUpdate("WarpShard", WARP_SHARD_PARAM, WARP_SHARD_INPUT,
                     WARP_SHARD_ATTEN_PARAM, 5.f);
        configBiUpdate("PositionX", POSITION_X_PARAM, POSITION_X_INPUT);
        configBiUpdate("PositionY", POSITION_Y_PARAM, POSITION_Y_INPUT, -1,
                       0.25f);
        configBiUpdate("PositionZ", POSITION_Z_PARAM, POSITION_Z_INPUT, -1,
                       3.f);
        configBiUpdate("RotationX", ROTATION_X_PARAM, ROTATION_X_INPUT);
        configBiUpdate("RotationY", ROTATION_Y_PARAM, ROTATION_Y_INPUT);
        configBiUpdate("RotationZ", ROTATION_Z_PARAM, ROTATION_Z_INPUT);
        configUpdate("TextureSelectA", TEXTURE_SELECT_A_PARAM,
                     TEXTURE_SELECT_A_INPUT, TEXTURE_SELECT_A_ATTEN_PARAM, 0.f);
        configUpdate("TextureSelectB", TEXTURE_SELECT_B_PARAM,
                     TEXTURE_SELECT_B_INPUT, TEXTURE_SELECT_B_ATTEN_PARAM,
                     10.f);
        configBiUpdate("GainRed", GAIN_RED_PARAM, GAIN_RED_INPUT);
        configBiUpdate("GainGreen", GAIN_GREEN_PARAM, GAIN_GREEN_INPUT);
        configBiUpdate("GainBlue", GAIN_BLUE_PARAM, GAIN_BLUE_INPUT);
        configUpdate("TextureScale", TEXTURE_SCALE_PARAM, TEXTURE_SCALE_INPUT,
                     TEXTURE_SCALE_ATTEN_PARAM, 5.f);
    }

    void update(const ProcessArgs &args) override {}
};

struct Mirage2xWidget : URack::UModuleWidget {
    Mirage2xWidget(Mirage2x *module) {
        setModule(module);
        setPanel(APP->window->loadSvg(
            asset::plugin(pluginInstance, "res/Mirage2x.svg")));

        addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(
            createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewBlack>(
            Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(
            createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
                                         RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        addParam(createParamCentered<LEDBezel>(mm2px(Vec(160.375, 10.44)),
                                               module, Mirage2x::ACTIVE_PARAM));
        addChild(createLightCentered<LEDBezelLight<RedLight>>(
            mm2px(Vec(160.375, 10.44)), module, Mirage2x::ACTIVE_LIGHT));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(136.802, 36.703)), module, Mirage2x::GLOW_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(167.35, 36.703)), module,
            Mirage2x::SIMULATION_SPEED_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(25.122, 37.758)), module,
            Mirage2x::TURBULENCE_AMOUNT_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(81.421, 37.758)), module,
            Mirage2x::WARP_DISTANCE_ATTEN_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(43.629, 41.51)), module,
            Mirage2x::TURBULENCE_X_SCALE_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(99.928, 41.51)), module, Mirage2x::WARP_X_EULER_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(125.086, 43.093)), module, Mirage2x::GLOW_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(155.635, 43.093)), module,
            Mirage2x::SIMULATION_SPEED_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(13.406, 44.148)), module,
            Mirage2x::TURBULENCE_AMOUNT_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(69.705, 44.148)), module, Mirage2x::WARP_DISTANCE_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(38.437, 50.956)), module,
            Mirage2x::TURBULENCE_X_SCALE_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(94.736, 50.956)), module,
            Mirage2x::WARP_X_EULER_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(136.802, 58.158)), module,
            Mirage2x::PARTICLE_SIZE_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(167.35, 58.158)), module,
            Mirage2x::PARTICLE_SHAPE_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(25.122, 61.859)), module,
            Mirage2x::TURBULENCE_FREQUENCY_ATTEN_PARAM));
        addParam(
            createParamCentered<TrimpotGray>(mm2px(Vec(81.421, 61.859)), module,
                                             Mirage2x::WARP_SHARD_ATTEN_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(125.086, 64.548)), module,
            Mirage2x::PARTICLE_SIZE_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(155.635, 64.548)), module,
            Mirage2x::PARTICLE_SHAPE_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(43.629, 65.611)), module,
            Mirage2x::TURBULENCE_Y_SCALE_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(99.928, 65.611)), module, Mirage2x::WARP_Y_EULER_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(13.406, 68.249)), module,
            Mirage2x::TURBULENCE_FREQUENCY_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(69.705, 68.249)), module, Mirage2x::WARP_SHARD_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(38.437, 75.056)), module,
            Mirage2x::TURBULENCE_Y_SCALE_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(94.736, 75.056)), module,
            Mirage2x::WARP_Y_EULER_ATTEN_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(122.705, 83.671)), module, Mirage2x::POSITION_X_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(142.297, 83.671)), module, Mirage2x::POSITION_Y_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(161.889, 83.671)), module, Mirage2x::POSITION_Z_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(15.61, 95.116)), module,
            Mirage2x::TEXTURE_SELECT_A_ATTEN_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(97.895, 95.116)), module,
            Mirage2x::TEXTURE_SELECT_B_ATTEN_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(122.561, 99.851)), module, Mirage2x::ROTATION_X_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(142.153, 99.851)), module, Mirage2x::ROTATION_Y_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(161.745, 99.851)), module, Mirage2x::ROTATION_Z_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(27.325, 101.506)), module,
            Mirage2x::TEXTURE_SELECT_A_PARAM));
        addParam(createParamCentered<Davies1900hWhiteKnob>(
            mm2px(Vec(86.18, 101.506)), module,
            Mirage2x::TEXTURE_SELECT_B_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(122.561, 116.173)), module, Mirage2x::GAIN_RED_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(142.153, 116.173)), module, Mirage2x::GAIN_GREEN_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(161.745, 116.173)), module, Mirage2x::GAIN_BLUE_PARAM));
        addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
            mm2px(Vec(35.848, 117.231)), module,
            Mirage2x::TEXTURE_SCALE_PARAM));
        addParam(createParamCentered<TrimpotGray>(
            mm2px(Vec(25.135, 117.537)), module,
            Mirage2x::TEXTURE_SCALE_ATTEN_PARAM));

        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(171.438, 10.44)), module, Mirage2x::ACTIVE_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(140.866, 45.23)),
                                                 module, Mirage2x::GLOW_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(171.414, 45.23)), module,
                                            Mirage2x::SIMULATION_SPEED_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(29.186, 46.285)), module,
                                            Mirage2x::TURBULENCE_AMOUNT_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(85.485, 46.285)), module, Mirage2x::WARP_DISTANCE_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(47.793, 50.956)), module,
            Mirage2x::TURBULENCE_X_SCALE_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(104.092, 50.956)), module, Mirage2x::LENGTH_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(140.866, 66.684)), module,
                                            Mirage2x::PARTICLE_SIZE_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(171.414, 66.684)), module,
                                            Mirage2x::PARTICLE_SHAPE_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(29.186, 70.385)), module,
            Mirage2x::TURBULENCE_FREQUENCY_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(85.485, 70.385)), module, Mirage2x::WARP_SHARD_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(47.793, 75.056)), module,
            Mirage2x::TURBULENCE_Y_SCALE_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(104.092, 75.056)), module, Mirage2x::WARP_Y_EULER_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(132.23, 83.671)), module, Mirage2x::POSITION_X_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(151.822, 83.671)), module, Mirage2x::POSITION_Y_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(171.414, 83.671)), module, Mirage2x::POSITION_Z_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(132.086, 99.851)), module, Mirage2x::ROTATION_X_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(151.678, 99.851)), module, Mirage2x::ROTATION_Y_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(171.27, 99.851)), module, Mirage2x::ROTATION_Z_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(11.546, 103.643)), module,
                                            Mirage2x::TEXTURE_SELECT_A_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(101.959, 103.643)), module,
            Mirage2x::TEXTURE_SELECT_B_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(132.086, 116.173)), module, Mirage2x::GAIN_RED_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(151.678, 116.173)), module, Mirage2x::GAIN_GREEN_INPUT));
        addInput(createInputCentered<PJ301MPort>(
            mm2px(Vec(171.27, 116.173)), module, Mirage2x::GAIN_BLUE_INPUT));
        addInput(
            createInputCentered<PJ301MPort>(mm2px(Vec(15.779, 117.537)), module,
                                            Mirage2x::TEXTURE_SCALE_INPUT));
    }
};

Model *modelMirage2x = createModel<Mirage2x, Mirage2xWidget>("Mirage2x");
