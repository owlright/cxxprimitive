#include <stdlib.h>
#include "rasterizer.h"
#include <stdio.h>

void Display(RasterizedLines* obj)
{
    RasterizedLines lines = *obj;
    for (int i = 0; i < lines.h; i++) {
        printf("x1: %-4d x2: %-4d y: %-4d\n", lines.lines[i].left, lines.lines[i].right, lines.lines[i].y);
    }
}
