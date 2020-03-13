#include "UModule.hpp"

struct Insider : URack::UModule {
  enum ParamIds {
    A_ATTEN_PARAM,
    A_PARAM,
    B_ATTEN_PARAM,
    B_PARAM,
    C_ATTEN_PARAM,
    C_PARAM,
    D_ATTEN_PARAM,
    D_PARAM,
    E_PARAM,
    F_PARAM,
    G_PARAM,
    H_PARAM,
    ACTIVE_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    A_INPUT,
    B_INPUT,
    C_INPUT,
    D_INPUT,
    E_INPUT,
    F_INPUT,
    G_INPUT,
    H_INPUT,
    POINT_CLOUD_1_INPUT,
    POINT_CLOUD_2_INPUT,
    ACTIVE_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    I_OUTPUT,
    J_OUTPUT,
    K_OUTPUT,
    L_OUTPUT,
    M_OUTPUT,
    POINT_CLOUD_1_OUTPUT,
    POINT_CLOUD_2_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds { ACTIVE_LIGHT, NUM_LIGHTS };

  Insider() {
    config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
    configBiUpdate("A", A_PARAM, A_INPUT, A_ATTEN_PARAM, 0.f);
    configBiUpdate("B", B_PARAM, B_INPUT, B_ATTEN_PARAM, 0.f);
    configBiUpdate("C", C_PARAM, C_INPUT, C_ATTEN_PARAM, 0.f);
    configBiUpdate("D", D_PARAM, D_INPUT, D_ATTEN_PARAM, 0.f);
    configBiUpdate("E", E_PARAM, E_INPUT);
    configBiUpdate("F", F_PARAM, F_INPUT);
    configBiUpdate("G", G_PARAM, G_INPUT);
    configBiUpdate("H", H_PARAM, H_INPUT);
    configListener("I", I_OUTPUT);
    configListener("J", J_OUTPUT);
    configListener("K", K_OUTPUT);
    configListener("L", L_OUTPUT);
    configListener("M", M_OUTPUT);
    configActivate(ACTIVE_PARAM, ACTIVE_LIGHT, ACTIVE_INPUT);
  }

  void update(const ProcessArgs &args) override {}
};

struct InsiderWidget : URack::UModuleWidget {
  InsiderWidget(Insider *module) {
    setModule(module);
    setPanel(
        APP->window->loadSvg(asset::plugin(pluginInstance, "res/Insider.svg")));

    addChild(createWidget<ScrewBlack>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(
        createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewBlack>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewBlack>(Vec(box.size.x - 2 * RACK_GRID_WIDTH,
                                          RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(17.532, 32.764)),
                                              module, Insider::A_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(28.852, 32.764)), module, Insider::A_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(62.76, 32.764)), module, Insider::E_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(17.532, 52.934)),
                                              module, Insider::B_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(28.852, 52.935)), module, Insider::B_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(62.76, 52.935)), module, Insider::F_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(17.532, 72.046)),
                                              module, Insider::C_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(28.852, 72.046)), module, Insider::C_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(62.76, 72.046)), module, Insider::G_PARAM));
    addParam(createParamCentered<TrimpotGray>(mm2px(Vec(17.532, 91.158)),
                                              module, Insider::D_ATTEN_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(28.852, 91.158)), module, Insider::D_PARAM));
    addParam(createParamCentered<Davies1900hSmallWhiteKnob>(
        mm2px(Vec(62.76, 91.158)), module, Insider::H_PARAM));
    addParam(createParamCentered<LEDBezel>(mm2px(Vec(44.771, 108.759)), module,
                                           Insider::ACTIVE_PARAM));
    addChild(createLightCentered<LEDBezelLight<RedLight>>(
        mm2px(Vec(44.771, 108.759)), module, Insider::ACTIVE_LIGHT));

    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.291, 32.764)), module,
                                             Insider::A_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.607, 32.764)), module,
                                             Insider::E_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.291, 52.935)), module,
                                             Insider::B_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.607, 52.935)), module,
                                             Insider::F_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.291, 72.046)), module,
                                             Insider::C_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.607, 72.046)), module,
                                             Insider::G_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.291, 91.158)), module,
                                             Insider::D_INPUT));
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(49.607, 91.158)), module,
                                             Insider::H_INPUT));
    addPointCloudInput(mm2px(Vec(15.615, 108.495)), module,
                       Insider::POINT_CLOUD_1_INPUT, "PointCloud1Input");
    addPointCloudInput(mm2px(Vec(72.198, 108.576)), module,
                       Insider::POINT_CLOUD_2_INPUT, "PointCloud2Input");
    addInput(createInputCentered<PJ301MPort>(mm2px(Vec(56.323, 108.759)),
                                             module, Insider::ACTIVE_INPUT));

    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.097, 36.468)),
                                               module, Insider::I_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.097, 49.23)),
                                               module, Insider::J_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.097, 61.992)),
                                               module, Insider::K_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.097, 74.754)),
                                               module, Insider::L_OUTPUT));
    addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(92.097, 87.516)),
                                               module, Insider::M_OUTPUT));
    addPointCloudOutput(mm2px(Vec(29.813, 108.495)), module,
                        Insider::POINT_CLOUD_1_OUTPUT, "PointCloud1Output");
    addPointCloudOutput(mm2px(Vec(86.396, 108.576)), module,
                        Insider::POINT_CLOUD_2_OUTPUT, "PointCloud2Output");
  }

  void appendContextMenu(Menu *menu) override {

    menu->addChild(new MenuEntry);
    menu->addChild(createMenuLabel("Script/graph name"));

    // TODO: This is copy/paste from UModule, needs to not be...
    auto module = dynamic_cast<URack::UModule *>(this->module);

    menu->addChild(new MenuEntry);
    menu->addChild(createMenuLabel("Host select"));

    for (auto socketInfo : URack::Dispatcher::sockets) {
      auto hostItem = new HostMenuItem;
      hostItem->module = module;
      hostItem->socketInfo = socketInfo;
      hostItem->text = socketInfo->ip;
      hostItem->rightText = CHECKMARK(socketInfo->hostNum == module->hostNum);
      menu->addChild(hostItem);
    }
    auto addHostItem = new AddHostItem;
    addHostItem->module = module;
    addHostItem->menu = menu;
    menu->addChild(addHostItem);
  }
};

Model *modelInsider = createModel<Insider, InsiderWidget>("Insider");
