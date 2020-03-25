#include "UModule.hpp"

struct Ribbon : URack::UModule {
  enum ParamIds {
    X_PARAM,
    Y_PARAM,
    Z_PARAM,
    LENGTH_PARAM,
    TAIL_PARAM,
    X_ATTEN_PARAM,
    Y_ATTEN_PARAM,
    Z_ATTEN_PARAM,
    WIDTH_PARAM,
    GLOW_PARAM,
    HUE_PARAM,
    SATURATION_PARAM,
    BRIGHTNESS_PARAM,
    ACTIVE_PARAM,
    HUE_ATTEN_PARAM,
    SATURATION_ATTEN_PARAM,
    BRIGHTNESS_ATTEN_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    LENGTH_INPUT,
    X_INPUT,
    Y_INPUT,
    Z_INPUT,
    ORIGIN_X_INPUT,
    ORIGIN_Y_INPUT,
    ORIGIN_Z_INPUT,
    ROTATE_X_INPUT,
    ROTATE_Y_INPUT,
    WIDTH_INPUT,
    GLOW_INPUT,
    ACTIVE_INPUT,
    HUE_INPUT,
    SATURATION_INPUT,
    BRIGHTNESS_INPUT,
    NUM_INPUTS
  };
  enum OutputIds { NUM_OUTPUTS };
  enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

  Ribbon() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configBiUpdate("X", X_PARAM, X_INPUT, X_ATTEN_PARAM, 0.f);
    configBiUpdate("Y", Y_PARAM, Y_INPUT, Y_ATTEN_PARAM, 0.f);
    configBiUpdate("Z", Z_PARAM, Z_INPUT, Z_ATTEN_PARAM, 0.f);
    configBiUpdate("OriginX", -1, ORIGIN_X_INPUT);
    configBiUpdate("OriginY", -1, ORIGIN_Y_INPUT);
    configBiUpdate("OriginZ", -1, ORIGIN_Z_INPUT);
    configBiUpdate("RotateX", -1, ROTATE_X_INPUT);
    configBiUpdate("RotateY", -1, ROTATE_Y_INPUT);
    configUpdate("Length", LENGTH_PARAM, LENGTH_INPUT);
    configBiUpdate("Tail", TAIL_PARAM);
    configUpdate("Width", WIDTH_PARAM, WIDTH_INPUT);
    configUpdate("Glow", GLOW_PARAM, GLOW_INPUT);
    configUpdate("Hue", HUE_PARAM, HUE_INPUT, HUE_ATTEN_PARAM, 0.f);
    configUpdate("Saturation", SATURATION_PARAM, SATURATION_INPUT,
                 SATURATION_ATTEN_PARAM, 0.f);
    configUpdate("Brightness", BRIGHTNESS_PARAM, BRIGHTNESS_INPUT,
                 BRIGHTNESS_ATTEN_PARAM, 0.f);
    configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
  }

  void update(const ProcessArgs &args) override {}
};

struct RibbonWidget : URack::UModuleWidget {
  RibbonWidget(Ribbon *module) {
    setModule(module);
    setPanel(
        APP->window->loadSvg(asset::plugin(pluginInstance, "res/Ribbon.svg")));

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(
        createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
                                          RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(45.566, 35.463)), module, Ribbon::X_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(69.274, 35.463)), module, Ribbon::Y_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(92.918, 35.463)), module, Ribbon::Z_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(25.269, 36.201)), module, Ribbon::LENGTH_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(9.394, 51.003)), module, Ribbon::TAIL_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(39.707, 56.423)),
                                              module, Ribbon::X_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(63.416, 56.423)),
                                              module, Ribbon::Y_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(87.06, 56.423)), module,
                                              Ribbon::Z_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(25.269, 62.665)), module, Ribbon::WIDTH_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(25.269, 87.832)), module, Ribbon::GLOW_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(45.418, 91.655)), module, Ribbon::HUE_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(69.062, 91.655)), module, Ribbon::SATURATION_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(92.771, 91.655)), module, Ribbon::BRIGHTNESS_PARAM));
    addParam(createParamCentered<LEDBezel>(mm2px(Vec(13.656, 108.759)), module,
                                           Ribbon::ACTIVE_PARAM));
    addChild(createLightCentered<LEDBezelLight<RedLight>>(
        mm2px(Vec(13.656, 108.759)), module, Ribbon::ACTIVE_LIGHT));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(39.56, 112.615)),
                                              module, Ribbon::HUE_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(
        mm2px(Vec(63.204, 112.615)), module, Ribbon::SATURATION_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(
        mm2px(Vec(86.912, 112.615)), module, Ribbon::BRIGHTNESS_ATTEN_PARAM));

    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.855, 36.201)), module,
                                             Ribbon::LENGTH_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.396, 56.423)), module,
                                             Ribbon::X_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(74.105, 56.423)), module,
                                             Ribbon::Y_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.748, 56.423)), module,
                                             Ribbon::Z_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.488, 66.797)), module,
                                             Ribbon::ORIGIN_X_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(55.457, 66.797)), module,
                                             Ribbon::ORIGIN_Y_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(68.425, 66.797)), module,
                                             Ribbon::ORIGIN_Z_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(81.393, 66.797)), module,
                                             Ribbon::ROTATE_X_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(94.361, 66.797)), module,
                                             Ribbon::ROTATE_Y_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.855, 70.175)), module,
                                             Ribbon::WIDTH_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(12.855, 87.832)), module,
                                             Ribbon::GLOW_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(25.208, 108.759)),
                                             module, Ribbon::ACTIVE_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(50.248, 112.615)),
                                             module, Ribbon::HUE_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(73.892, 112.615)),
                                             module, Ribbon::SATURATION_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(97.601, 112.615)),
                                             module, Ribbon::BRIGHTNESS_INPUT));
  }
};

Model *modelRibbon = createModel<Ribbon, RibbonWidget>("Ribbon");
