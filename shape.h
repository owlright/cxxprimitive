#ifndef shape_h
#define shape_h

#include "rasterizer.h"
namespace primitive {

enum class ShapeType {
    Line,
    Triangle,
    Rectangle,
    Circle,
    Ellipse,
    Polyline,
    Polygon,
    Bezier,
    Arc,
    ShapeTypeCount
};

class IShape {
public:
    virtual RasterizedLines Rasterize() const = 0;
};

}

#endif