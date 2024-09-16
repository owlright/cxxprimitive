#pragma once
#include <vector>
#include <random>
#include <chrono>
#include <future>
#include "image.h"
#include "rasterizer.h"
#include "state.h"
#include "shape.h"

namespace primitive {

struct Heatmap {
    int W, H;
    std::vector<uint64_t> counts;
};

struct Worker {

private:
    ThreadPool* pool;

public:
    int W, H;
    const Image* target { nullptr };
    Image* current { nullptr };
    Image* buffer { nullptr };
    std::vector<Scanline> lines;
    int counter { 0 };
    double score { 0 };
    std::mt19937 rng;
    std::shared_ptr<ThreadPool> pool;
    void init(const Image&, double score);

    std::future<State> run(ShapeType shape, int a, int n, int age, int m);

    Worker(const Image* target, std::shared_ptr<ThreadPool> pool);
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
