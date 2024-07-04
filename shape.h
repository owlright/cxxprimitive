#ifndef shape_h
#define shape_h

#include "rasterizer.h"
namespace primitive {

class IShape {
public:
    virtual RasterizedLines Rasterize() const = 0;
};

}

#endif