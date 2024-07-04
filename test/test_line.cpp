#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "shape.h"
#include "line.h"
using namespace primitive;
#define CASES_NUMBER 8
int main()
{
    int positions[CASES_NUMBER][4] = {
        { 1, 1, 11, 1 }, // slope = 0
        { 1, 1, 1, 11 }, // slope = inf
        { 1, 1, 5, 5 }, // slop = 1
        { 1, 5, 5, 1 }, // slope = -1
        { 0, 1, 6, 4 }, // slope = 0.5
        { 0, 3, 6, 0 }, // slope = -0.5
        { 0, 1, 3, 7 }, // slope = 2
        { 0, 7, 3, 1 }, // slope = -2
    };
    for (int i = 0; i < CASES_NUMBER; i++) {
        printf("case %d: (%d, %d) -> (%d, %d)\n", i + 1, positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
        Line t = Line(positions[i][0], positions[i][1], positions[i][2], positions[i][3]);
        RasterizedLines lines = t.Rasterize();
        for (int i = 0; i < lines.h; i++) {
            printf("x1: %-4d x2: %-4d y: %-4d\n", lines.lines[i].left, lines.lines[i].right, lines.lines[i].y);
        }
        DestroyRasterizedLines(&lines);
        printf("\n");
    }
    return 0;
}