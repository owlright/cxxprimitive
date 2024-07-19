#ifndef raster_h
#define raster_h
#include <stdint.h>
#include <vector>
typedef struct {
    int x, y;
} Point;

typedef struct Scanline_ {
    int y = -1;
    int left = -1;
    int right = -1;
    uint32_t alpha;
} Scanline;

typedef struct {
    std::vector<Scanline> lines;
    int h;
} RasterizedLines;

void Display(RasterizedLines* obj);
#endif
