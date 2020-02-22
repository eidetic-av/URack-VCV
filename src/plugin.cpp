#include "plugin.hpp"

#include "URack.hpp"

Plugin* pluginInstance;

void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelArena);
	p->addModel(modelLiveScan3D);
	p->addModel(modelRenderer);
	p->addModel(modelLineRenderer);
	p->addModel(modelBounds);
	p->addModel(modelABBox);
	p->addModel(modelPointCounter);
	p->addModel(modelHighestPoint);
	p->addModel(modelDrone);
	p->addModel(modelHarmony);
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when
	// your module is created to reduce startup times of Rack.

	// load plugin-wide settings
	URack::Settings::load();

	// if we didn't load any pre-existing host configurations, create one at
	// LOCALHOST
	if (URack::Dispatcher::sockets.size() == 0) URack::Dispatcher::create();

	// create a listener to process incoming messages
	URack::Listener::create();
}
