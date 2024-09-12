#pragma once
#include "Worker.h"
#include "shape.h"

namespace primitive {

struct State {
    Worker* worker;
    IShape* shape;
    int alpha;
    bool mutateAlpha;
    double score;
};

}