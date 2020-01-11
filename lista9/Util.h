#pragma once

#include "Solution.h"
#include "MscnProblem.h"
#include <string>

namespace util
{
    bool isTagEqual(FILE* file, std::string tag);
    Exception generateRandomSolution(MscnProblem& problem, Solution& solution);
}