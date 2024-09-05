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

}
