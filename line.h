#ifndef line_h
#define line_h
#include <stdlib.h>
#include "image.h"
#include "shape.h"
#include <vector>
#include <ostream>
namespace primitive {
using std::vector;
class Line : public IShape {
private:
    int x1, y1;
    int x2, y2;
public:
    Color color;

public:
    Line()
    {
        x1 = y1 = x2 = y2 = -1;
        color.r = color.g = color.b = 0;
    }
    explicit Line(int x1, int y1, int x2, int y2);
    explicit Line(int x[4]);
    explicit Line(std::vector<int> x);
    void SetStart(int x, int y) { x1 = x; y1 = y; };
    void SetEnd(int x, int y) { x2 = x; y2 = y; };
    RasterizedLines Rasterize() const override;

public:
    int getX1() const { return x1; }
    int getY1() const { return y1; }
    int getX2() const { return x2; }
    int getY2() const { return y2; }

private:
    void RasterizeLineLow(int x0, int y0, int x1, int y1, vector<Scanline>& lines) const;
    void RasterizeLineHigh(int x0, int y0, int x1, int y1, vector<Scanline>& lines) const;

public:
    friend std::ostream& operator<<(std::ostream& os, const Line& line);
};


}

#endif
