#include "Worker.h"
namespace primitive {

Worker::Worker(const Image* target)
{
    this->target = target;
    this->current = new Image(*target);
    this->buffer = new Image(*target);
    this->W = target->width;
    this->H = target->height;
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    this->rng.seed(seed);
}

// Move constructor
Worker::Worker(Worker&& other) noexcept
    : W(other.W)
    , H(other.H)
    , target(other.target)
    , current(other.current)
    , buffer(other.buffer)
    , lines(std::move(other.lines))
    , counter(other.counter)
    , score(other.score)
    , rng(std::move(other.rng))
{
    other.target = nullptr;
    other.current = nullptr;
    other.buffer = nullptr;
    other.counter = 0;
    other.score = 0;
}

}
