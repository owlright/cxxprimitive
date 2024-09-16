#pragma once
#include "Worker.h"
#include "shape.h"

namespace primitive {

struct State {
    Worker* worker {nullptr};
    IShape* shape {nullptr};
    int alpha;
    bool mutateAlpha;
    double score;
    double energy();
};

}