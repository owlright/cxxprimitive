#include "drawingboard.h"
#include <cmath>
#include <future>

namespace primitive {

double differenceFull(const Image& a, const Image& b)
{
    auto w = a.width;
    auto h = a.height;
    uint64_t total = 0;
    int ar, ag, ab, aa, br, bg, bb, ba, dr, dg, db, da;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            ar = a.RGBAAt(x, y).r;
            ag = a.RGBAAt(x, y).g;
            ab = a.RGBAAt(x, y).b;
            aa = a.RGBAAt(x, y).a;
            br = b.RGBAAt(x, y).r;
            bg = b.RGBAAt(x, y).g;
            bb = b.RGBAAt(x, y).b;
            ba = b.RGBAAt(x, y).a;
            dr = ar - br;
            dg = ag - bg;
            db = ab - bb;
            da = aa - ba;
            total += uint64_t(dr * dr + dg * dg + db * db + da * da);
        }
    }
    return sqrt(double(total) / double(w * h * 4)) / 255;
}

int DrawingBoard::step(ShapeType shapeType, int alpha, int repeat)
{
    auto state = runWorkers(shapeType, alpha, 1000, 100, 16);
    this->add(state.shape, alpha);
    return 0;
}

DrawingBoard::DrawingBoard(const Image& target, Color background, int numWorkers)
{
    this->target = target;
    this->background = background;
    this->width = target.width;
    this->height = target.height;
    this->current = Image(background, width, height);
    for (int i = 0; i < width * height; i++) {
        current.data[i] = background.r;
        current.data[i + 1] = background.g;
        current.data[i + 2] = background.b;
    }
    this->score = differenceFull(target, current);
    printf("Initial score: %f\n", score);
    for (int i = 0; i < numWorkers; i++) {
        Worker w(&target);
        workers.push_back(std::move(w));
    }
}

State DrawingBoard::runWorkers(ShapeType shape, int a, int n, int age, int m)
{
    auto wn = this->workers.size();
    auto wm = m / wn;
    if (m % wn != 0) {
        wm++;
    }
    std::vector<std::future<State>> results(wn);
    for (int i = 0; i < wn; i++) {
        auto& worker = this->workers[i];
        worker.init(this->current, this->score);
        results[i] = worker.run(shape, a, n, age, wm);
    }
    double bestEnergy = INFINITY;
    State bestState;
    for (int i = 0; i < wn; i++) {
        auto& result = results[i];
        auto state = result.get();
        if (state.energy() < bestEnergy) {
            bestEnergy = state.energy();
            bestState = state;
        }
    }
    return bestState;
}

void DrawingBoard::DrawLine(const Line& line)
{
    auto lines = line.Rasterize();
    auto color = line.color;
    for (int i = 0; i < lines.h; i++) {
        auto l = lines.lines[i];
        for (int x = l.left; x <= l.right; x++) {
            current.data[l.y * width * 3 + x] = color.r;
            current.data[l.y * width * 3 + x + 1] = color.g;
            current.data[l.y * width * 3 + x + 2] = color.b;
            // 已经画过的线，从目标图像中减去
            // targetImage[l.y * width * 3 + x] -= color.r;
            // targetImage[l.y * width * 3 + x + 1] -= color.g;
            // targetImage[l.y * width * 3 + x + 2] -= color.b;
        }
    }
    this->lines.push_back(line);
}

void DrawingBoard::add(IShape* shape, int alpha)
{
    auto lines = shape->Rasterize();
}

} // namespace primitive
