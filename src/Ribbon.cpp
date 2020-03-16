#include "UModule.hpp"

struct Ribbon : URack::UModule {
  enum ParamIds {
    LENGTH_PARAM,
    X_PARAM,
    Y_PARAM,
    Z_PARAM,
    TAIL_PARAM,
    X_ATTEN_PARAM,
    Y_ATTEN_PARAM,
    Z_ATTEN_PARAM,
    HUE_PARAM,
    SATURATION_PARAM,
    BRIGHTNESS_PARAM,
    WIDTH_PARAM,
    HUE_ATTEN_PARAM,
    SATURATION_ATTEN_PARAM,
    BRIGHTNESS_ATTEN_PARAM,
    GLOW_PARAM,
    ACTIVE_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    LENGTH_INPUT,
    TAIL_INPUT,
    X_INPUT,
    Y_INPUT,
    Z_INPUT,
    WIDTH_INPUT,
    HUE_INPUT,
    SATURATION_INPUT,
    BRIGHTNESS_INPUT,
    GLOW_INPUT,
    ACTIVE_INPUT,
    NUM_INPUTS
  };
  enum OutputIds { NUM_OUTPUTS };
  enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

  Ribbon() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configUpdate("Length", LENGTH_PARAM, LENGTH_INPUT);
    configBiUpdate("X", X_PARAM, X_INPUT, X_ATTEN_PARAM, 0.f);
    configBiUpdate("Y", Y_PARAM, Y_INPUT, Y_ATTEN_PARAM, 0.f);
    configBiUpdate("Z", Z_PARAM, Z_INPUT, Z_ATTEN_PARAM, 0.f);
    configBiUpdate("Tail", TAIL_PARAM, TAIL_INPUT);
    configUpdate("Hue", HUE_PARAM, HUE_INPUT, HUE_ATTEN_PARAM, 0.f);
    configUpdate("Saturation", SATURATION_PARAM, SATURATION_INPUT,
                 SATURATION_ATTEN_PARAM, 0.f);
    configUpdate("Brightness", BRIGHTNESS_PARAM, BRIGHTNESS_INPUT,
                 BRIGHTNESS_ATTEN_PARAM, 0.f);
    configUpdate("Width", WIDTH_PARAM, WIDTH_INPUT);
    configUpdate("Glow", GLOW_PARAM, GLOW_INPUT);
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

    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(81.536, 31.967)), module, Ribbon::LENGTH_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(13.886, 35.463)), module, Ribbon::X_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(37.53, 35.463)), module, Ribbon::Y_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(61.239, 35.463)), module, Ribbon::Z_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(81.536, 54.492)), module, Ribbon::TAIL_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(8.028, 56.423)), module,
                                              Ribbon::X_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(31.672, 56.423)),
                                              module, Ribbon::Y_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(55.38, 56.423)), module,
                                              Ribbon::Z_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(13.886, 76.566)), module, Ribbon::HUE_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(37.53, 76.566)), module, Ribbon::SATURATION_PARAM));
    addParam(createParamCentered<Davies1900hWhiteKnob>(
        mm2px(Vec(61.239, 76.566)), module, Ribbon::BRIGHTNESS_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(81.536, 77.401)), module, Ribbon::WIDTH_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(8.028, 97.526)), module,
                                              Ribbon::HUE_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(
        mm2px(Vec(31.672, 97.526)), module, Ribbon::SATURATION_ATTEN_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(55.38, 97.526)), module,
                                              Ribbon::BRIGHTNESS_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(81.536, 100.744)), module, Ribbon::GLOW_PARAM));
    addParam(createParamCentered<LEDBezel>(mm2px(Vec(44.901, 108.759)), module,
                                           Ribbon::ACTIVE_PARAM));
    addChild(createLightCentered<LEDBezelLight<RedLight>>(
        mm2px(Vec(44.901, 108.759)), module, Ribbon::ACTIVE_LIGHT));

    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(93.949, 31.967)), module,
                                             Ribbon::LENGTH_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(93.949, 54.492)), module,
                                             Ribbon::TAIL_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.717, 56.423)), module,
                                             Ribbon::X_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.361, 56.423)), module,
                                             Ribbon::Y_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(66.069, 56.423)), module,
                                             Ribbon::Z_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(93.949, 77.401)), module,
                                             Ribbon::WIDTH_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(18.717, 97.526)), module,
                                             Ribbon::HUE_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(42.361, 97.526)), module,
                                             Ribbon::SATURATION_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(66.069, 97.526)), module,
                                             Ribbon::BRIGHTNESS_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(93.949, 100.744)),
                                             module, Ribbon::GLOW_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(56.453, 108.759)),
                                             module, Ribbon::ACTIVE_INPUT));
  }
};

Model *modelRibbon = createModel<Ribbon, RibbonWidget>("Ribbon");
