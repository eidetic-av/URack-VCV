#pragma once
#include "widget/SvgWidget.hpp"


struct PointCloudPort {

};

struct PointCloudInput : PointCloudPort { };
struct PointCloudOutput : PointCloudPort { };

struct PointCloudPortWidget : rack::widget::SvgWidget {

};
