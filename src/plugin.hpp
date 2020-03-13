#pragma once
#include <rack.hpp>

using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin *pluginInstance;

// Declare each Model, defined in each module source file
extern Model *modelLiveScan3D;
extern Model *modelBounds;
extern Model *modelABBox;
extern Model *modelPointCounter;
extern Model *modelHighestPoint;
extern Model *modelDrone;
extern Model *modelHarmony;
extern Model *modelBillboard;
extern Model *modelInsider;
