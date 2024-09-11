#pragma once
#include <vector>
#include <random>
#include <chrono>
#include "image.h"
#include "rasterizer.h"
namespace primitive {

struct Heatmap {
    int W, H;
    std::vector<uint64_t> counts;
};

struct Worker {
    int W, H;
    const Image* target {nullptr};
    Image* current {nullptr};
    Image* buffer {nullptr};
    std::vector<Scanline> lines;
    int counter { 0 };
    double score { 0 };
    std::mt19937 rng;
    Worker(const Image* target);
    // Copy constructor
    const Worker(const Worker& other) = delete;
    // Copy assignment operator
    const Worker& operator=(const Worker& other) = delete;
    // Move constructor
    Worker(Worker&& other) noexcept;
    // Move assignment operator
    Worker& operator=(Worker&& other) = delete;
};

}
