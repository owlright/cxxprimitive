#include "stdio.h"
#include "triangle.h"
#include "log.h"

namespace primitive {
RasterizedLines Triangle::Rasterize() const
{
    log_info_no_args();
    Scanline* lines = (Scanline*)calloc(3, sizeof(Scanline));
    RasterizedLines result;
    result.lines = lines;
    result.h = 3;
    return result;
}

}
