#include <assert.h>
#include <math.h>
#include "line.h"
#define LOG_LEVEL INFO
#include "log.h"
namespace primitive {

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

Line::Line(int x1, int y1, int x2, int y2)
{
    assert(x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0);
    assert(!(x1 == x2 && y1 == y2));
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

Line::Line(int x[4])
{
    assert(x[0] >= 0 && x[1] >= 0 && x[2] >= 0 && x[3] >= 0);
    assert(!(x[0] == x[2] && x[1] == x[3]));
    this->x1 = x[0];
    this->y1 = x[1];
    this->x2 = x[2];
    this->y2 = x[3];
}

Line::Line(vector<int> x)
{
    assert(x[0] >= 0 && x[1] >= 0 && x[2] >= 0 && x[3] >= 0);
    assert(!(x[0] == x[2] && x[1] == x[3]));
    this->x1 = x[0];
    this->y1 = x[1];
    this->x2 = x[2];
    this->y2 = x[3];
}

RasterizedLines Line::Rasterize() const
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    log_debug("dx: %d, dy: %d", dx, dy);

    RasterizedLines result;
    // Scanline* lines = (Scanline*)calloc(abs(dy) + 1, sizeof(Scanline));
    std::vector<Scanline> lines(abs(dy) + 1);

    // ! Special case for vertical lines and horizontal lines
    if (dx == 0) { // x1 == x2
        for (int j = 0; j <= dy; j++) {
            lines[j].y = y1 + j;
            lines[j].left = x1;
            lines[j].right = x1;
        }
        result.lines = lines;
        result.h = abs(dy) + 1;
        return result;
    }
    if (dy == 0) { // y1 == y2
        lines[0].y = y1;
        lines[0].left = x1;
        lines[0].right = x2;
        result.lines = lines;
        result.h = 1;
        return result;
    }
    if (dx == abs(dy)) {
        for (int i = 0; i <= dx; i++) {
            int j = dy > 0 ? i : -i;
            lines[i].y = y1 + j;
            lines[i].left = x1 + i;
            lines[i].right = x1 + i;
        }
        result.lines = lines;
        result.h = abs(dy) + 1;
        return result;
    }

    // Bresenham's line algorithm
    if (abs(dy) < abs(dx)) { // |slope| < 1
        if (x1 > x2) {
            RasterizeLineLow(x2, y2, x1, y1, lines);
        } else {
            RasterizeLineLow(x1, y1, x2, y2, lines);
        }
    } else {
        if (y1 > y2) {
            RasterizeLineHigh(x2, y2, x1, y1, lines);
        } else {
            RasterizeLineHigh(x1, y1, x2, y2, lines);
        }
    }
    result.lines = lines;
    result.h = abs(dy) + 1;
    return result;
}

// ! 以下实现将grid的左上角坐标视作grid自身坐标
// plow line slope between -1 and 1
void Line::RasterizeLineLow(int x0, int y0, int x1, int y1, std::vector<Scanline>& lines) const
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;
    int line_number = 0;
    for (int x = x0; x <= x1; x++) {
        line_number = abs(y - y0);
        if (lines[line_number].left == -1) { // the first point in the line
            lines[line_number].left = x;
            lines[line_number].y = y;
        }
        lines[line_number].right = x; // update x2 anyway
        if (D > 0) {
            y = y + yi;
            D = D + 2 * dy - 2 * dx;
        } else {
            D = D + 2 * dy;
        }
    }
}

// high line slope greater than 1 or less than -1
void Line::RasterizeLineHigh(int x0, int y0, int x1, int y1, std::vector<Scanline>& lines) const
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = 2 * dx - dy;
    int x = x0;
    int line_number = 0;
    for (int y = y0; y <= y1; y++) {
        line_number = y - y0 > 0 ? y - y0 : y0 - y; // maybe abs() is ok
        if (lines[line_number].left == -1) { // the first point in the line
            lines[line_number].left = x;
            lines[line_number].y = y;
        }
        lines[line_number].right = x; // update x2 anyway
        if (D > 0) {
            x = x + xi;
            D = D + 2 * dx - 2 * dy;
        } else {
            D = D + 2 * dx;
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
    os << "Line: (" << line.x1 << ", " << line.y1 << ") to (" << line.x2 << ", " << line.y2 << ") with " << line.color;
    return os;
}

}
