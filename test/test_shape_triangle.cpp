#include <stdint.h>
#include <stdlib.h>
#include "shape.h"
#include "triangle.h"
using namespace primitive;
int main()
{
    Triangle t;
    auto lines = t.Rasterize();
    DestroyRasterizedLines(&lines);
    return 0;
}