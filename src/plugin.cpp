#include "plugin.hpp"

#include "URack.hpp"

Plugin *pluginInstance;

void init(Plugin *p) {
    pluginInstance = p;

    // Add modules here
    p->addModel(modelLiveScan3D);
    p->addModel(modelBounds);
    p->addModel(modelABBox);
    p->addModel(modelPointCounter);
    p->addModel(modelHighestPoint);
    p->addModel(modelDrone);
    p->addModel(modelHarmony);
    p->addModel(modelBillboard);
    p->addModel(modelInsider);
    p->addModel(modelDayTripper);
    p->addModel(modelRibbon);

    // load plugin-wide settings
    URack::Settings::load();

    // if we didn't load any pre-existing host configurations, create one at
    // LOCALHOST
    if (URack::Dispatcher::sockets.size() == 0)
        URack::Dispatcher::connect_host();

    // create a listener to process incoming messages
    URack::Listener::create();
}
