#include "../lib/oscpack/ip/UdpSocket.h"
#include "../lib/oscpack/osc/OscOutboundPacketStream.h"
#include "URack.hpp"
#include "plugin.hpp"

struct Arena : Module {
	enum ParamIds { PITCH_PARAM, NUM_PARAMS };
	enum InputIds { PITCH_INPUT, NUM_INPUTS };
	enum OutputIds { SINE_OUTPUT, NUM_OUTPUTS };
	enum LightIds { BLINK_LIGHT, NUM_LIGHTS };

	float phase = 0.f;
	float blinkPhase = 0.f;

	Arena() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(PITCH_PARAM, 0.f, 1.f, 0.f, "");
	}

	void onAdd() override { URack::Dispatcher::send(this->id); }

	~Arena() {}

	float lastPitch = 0.f;

	void process(const ProcessArgs &args) override {
		// Compute the frequency from the pitch parameter and input
		float pitch = params[PITCH_PARAM].getValue();
		pitch += inputs[PITCH_INPUT].getVoltage();
		pitch = clamp(pitch, -5.f, 5.f);

		lastPitch = pitch;

		// The default pitch is C4 = 261.6256f
		float freq = dsp::FREQ_C4 * std::pow(2.f, pitch);

		// Accumulate the phase
		phase += freq * args.sampleTime;
		if (phase >= 0.5f) phase -= 1.f;

		// Compute the sine output
		float sine = std::sin(2.f * M_PI * phase);
		// Audio signals are typically +/-5V
		// https://vcvrack.com/manual/VoltageStandards.html
		outputs[SINE_OUTPUT].setVoltage(5.f * sine);

		// Blink light at 1Hz
		blinkPhase += args.sampleTime;
		if (blinkPhase >= 1.f) blinkPhase -= 1.f;

		lights[BLINK_LIGHT].setBrightness(blinkPhase < 0.5f ? 1.f : 0.f);
	}
};

struct ArenaWidget : ModuleWidget {
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

		addParam(createParamCentered<RoundBlackKnob>(
					mm2px(Vec(15.24, 46.063)), module, Arena::PITCH_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(15.24, 77.478)),
					module, Arena::PITCH_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(15.24, 108.713)),
					module, Arena::SINE_OUTPUT));

		addChild(createLightCentered<MediumLight<RedLight>>(
					mm2px(Vec(15.24, 25.81)), module, Arena::BLINK_LIGHT));

		// testPort = new PointCloudPortWidget();
		// testPort->setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/PJ301M.svg")));
		// addChild(testPort);
	}
};

Model *modelArena = createModel<Arena, ArenaWidget>("Arena");
