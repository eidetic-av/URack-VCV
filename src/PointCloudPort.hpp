#include "widget/SvgWidget.hpp"

namespace URack {

struct PointCloudPort {

};

struct PointCloudInput : PointCloudPort { };
struct PointCloudOutput : PointCloudPort { };

struct PointCloudPortWidget : rack::widget::SvgWidget {

};

}
