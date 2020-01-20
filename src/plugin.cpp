#include "plugin.hpp"

#include "URack.hpp"

Plugin* pluginInstance;

void init(Plugin* p) {
	pluginInstance = p;

	// Add modules here
	p->addModel(modelArena);
	p->addModel(modelLiveScan3D);
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when
	// your module is created to reduce startup times of Rack.
	URack::Dispatcher::create();
}

