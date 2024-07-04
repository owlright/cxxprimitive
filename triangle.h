#ifndef triangle_h
#define triangle_h
#include <stdlib.h>
#include "shape.h"

namespace primitive {

class Triangle : public IShape {
public:
    RasterizedLines Rasterize() const override;

private:
    int x1, y1;
    int x2, y2;
    int x3, y3;
};

}

#endif
