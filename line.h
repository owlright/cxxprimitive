#ifndef line_h
#define line_h
#include <stdlib.h>
#include "shape.h"
#include <vector>
namespace primitive {
using std::vector;
class Line : public IShape {
private:
    int x1, y1;
    int x2, y2;

public:
    explicit Line(int x1, int y1, int x2, int y2);
    explicit Line(int x[4]);
    explicit Line(std::vector<int> x);
    RasterizedLines Rasterize() const override;

private:
    void RasterizeLineLow(int x0, int y0, int x1, int y1, vector<Scanline>& lines) const;
    void RasterizeLineHigh(int x0, int y0, int x1, int y1, vector<Scanline>& lines) const;
};
}

#endif
