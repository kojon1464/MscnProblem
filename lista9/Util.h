#pragma once

#include "Solution.h"
#include "Problem.h"
#include <string>

namespace util
{
    bool isTagEqual(FILE* file, std::string tag);
    Exception generateRandomSolution(Problem& problem, Solution& solution);
}