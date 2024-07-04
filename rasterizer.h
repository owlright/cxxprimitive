#ifndef raster_h
#define raster_h
#include <stdint.h>

typedef struct {
    int x, y;
} Point;

typedef struct {
    int y, left, right;
    uint32_t alpha;
} Scanline;

typedef struct {
    Scanline* lines;
    int h;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RasterizedLines;

void DestroyRasterizedLines(RasterizedLines* obj);
void Display(RasterizedLines* obj);
#endif
