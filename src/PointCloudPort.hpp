#include "plugin.hpp"
#include <settings.hpp>
#include <componentlibrary.hpp>

namespace URack {

struct PointCloudCableWidget : app::CableWidget {

	static void drawCable(NVGcontext* vg, math::Vec pos1, math::Vec pos2, NVGcolor color, float thickness, float tension, float opacity) {
		NVGcolor colorShadow = nvgRGBAf(0, 0, 0, 0.10);
		NVGcolor colorOutline = nvgLerpRGBA(color, nvgRGBf(0.0, 0.0, 0.0), 0.5);

		// Cable
		if (opacity > 0.0) {
			nvgSave(vg);
			// This power scaling looks more linear than actual linear scaling
			nvgGlobalAlpha(vg, std::pow(opacity, 1.5));

			float dist = pos1.minus(pos2).norm();
			math::Vec slump;
			slump.y = (1.0 - tension) * (150.0 + 1.0 * dist);
			math::Vec pos3 = pos1.plus(pos2).div(2).plus(slump);

			// Adjust pos1 and pos2 to not draw over the plug
			pos1 = pos1.plus(pos3.minus(pos1).normalize().mult(9));
			pos2 = pos2.plus(pos3.minus(pos2).normalize().mult(9));

			nvgLineJoin(vg, NVG_ROUND);

			// Shadow
			math::Vec pos4 = pos3.plus(slump.mult(0.08));
			nvgBeginPath(vg);
			nvgMoveTo(vg, pos1.x, pos1.y);
			nvgQuadTo(vg, pos4.x, pos4.y, pos2.x, pos2.y);
			nvgStrokeColor(vg, colorShadow);
			nvgStrokeWidth(vg, thickness);
			nvgStroke(vg);

			// Cable outline
			nvgBeginPath(vg);
			nvgMoveTo(vg, pos1.x, pos1.y);
			nvgQuadTo(vg, pos3.x, pos3.y, pos2.x, pos2.y);
			nvgStrokeColor(vg, colorOutline);
			nvgStrokeWidth(vg, thickness);
			nvgStroke(vg);

			// Cable solid
			nvgStrokeColor(vg, color);
			nvgStrokeWidth(vg, thickness - 2);
			nvgStroke(vg);

			// Cable inner line
			nvgStrokeColor(vg, SCHEME_WHITE);
			nvgStrokeWidth(vg, thickness - 7);
			nvgStroke(vg);

			nvgRestore(vg);
		}
	}

	void draw(const DrawArgs& args) override {
		float opacity = settings::cableOpacity;
		float tension = settings::cableTension;
		float thickness = 9;

		if (isComplete()) {
			engine::Output* output = &cable->outputModule->outputs[cable->outputId];
			// Draw opaque if mouse is hovering over a connected port

			if (outputPort->hovered || inputPort->hovered) {
				opacity = 1.0;
			}
			else if (output->channels == 0) {
				// Draw translucent cable if not active (i.e. 0 channels)
				opacity *= 0.5;
			}
		}
		else {
			// Draw opaque if the cable is incomplete
			opacity = 1.0;
		}

		math::Vec outputPos = getOutputPos();
		math::Vec inputPos = getInputPos();
		drawCable(args.vg, outputPos, inputPos, color, thickness, tension, opacity);
	}
};

struct PointCloudPort : app::SvgPort {

	std::string oscAddress;

	PointCloudPort() {
		setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/PJ301M.svg")));
	}

	void draw(const DrawArgs& args) override {
		CableWidget* cw = APP->scene->rack->incompleteCable;
		if (cw) {
			if (!dynamic_cast<PointCloudCableWidget*>(cw))	
				nvgGlobalAlpha(args.vg, 0.5);
		}
		Widget::draw(args);
	}

	void onDragStart(const event::DragStart& e) override {
		if (e.button != GLFW_MOUSE_BUTTON_LEFT)
			return;

		PointCloudCableWidget* cw = NULL;

		cw = dynamic_cast<PointCloudCableWidget*>(APP->scene->rack->getTopCable(this));
		if (cw)
		{
			// history::CableRemove
			history::CableRemove* h = new history::CableRemove;
			h->setCable(cw);
			APP->history->push(h);

			//Disconnect already existing cable
			APP->scene->rack->removeCable(cw);
			if (type == OUTPUT)
				cw->setOutput(NULL);
			else
				cw->setInput(NULL);
		}

		// Create a new cable
		cw = new PointCloudCableWidget;
		if (type == OUTPUT)
			cw->setOutput(this);
		else
			cw->setInput(this);

		APP->scene->rack->setIncompleteCable(cw);
	};

	void onDragEnd(const event::DragEnd& e) override {
		if (e.button != GLFW_MOUSE_BUTTON_LEFT)
			return;

		CableWidget* cw = APP->scene->rack->releaseIncompleteCable();
		if (!cw)
			return;

		if (cw->isComplete()) {
		
			// If the input isn't a PointCloud port, release connection
			if (!dynamic_cast<PointCloudPort*>(cw->inputPort))
				return;

			APP->scene->rack->addCable(cw);

			// history::CableAdd
			history::CableAdd* h = new history::CableAdd;
			h->setCable(cw);
			APP->history->push(h);
		}
		else {
			delete cw;
		}
	}

	void onDragDrop(const event::DragDrop& e) override {
		if (e.button != GLFW_MOUSE_BUTTON_LEFT)
			return;

		// Reject ports if this is an input port and something is already plugged into it
		if (type == INPUT) {
			if (APP->scene->rack->getTopCable(this))
				return;
		}

		CableWidget* cw = APP->scene->rack->incompleteCable;
		if (cw) {
			// ignore if it's not a point cloud cable
			if (!dynamic_cast<PointCloudCableWidget*>(cw))
				return;

			cw->hoveredOutputPort = cw->hoveredInputPort = NULL;
			if (type == OUTPUT)
				cw->setOutput(this);
			else
				cw->setInput(this);
		}
	};
};

}
