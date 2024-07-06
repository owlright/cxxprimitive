#include "stdio.h"
#include "triangle.h"
#include "log.h"

namespace primitive {
RasterizedLines Triangle::Rasterize() const
{
    log_info_no_args();
    RasterizedLines result;
    result.h = 3;
    return result;
}

}
