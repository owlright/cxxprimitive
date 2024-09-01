#pragma once
#include <vector>
#include "image.h"
#include "rasterizer.h"
namespace primitive {

struct Heatmap {
    int W, H;
    std::vector<uint64_t> counts;
};

struct Worker {
    int W, H;
    Image* target {nullptr};
    Image* current {nullptr};
    Image* buffer {nullptr};
    std::vector<Scanline> lines;
    int counter { 0 };
    double score { 0 };
};

}
