#pragma once
#include "Worker.h"

namespace primitive {

struct State {
    Worker* worker;
    int alpha;
    bool mutateAlpha;
    double score;
};

}