#pragma once

#include "Array.h"
#include "Solution.h"
#include "MscnProblem.h"

class RandomSearch
{
public:
    RandomSearch(MscnProblem* problem, Exception& exception);
    ~RandomSearch();

    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();
private:
    MscnProblem* problem;
    Solution tempSolution;
    Solution bestSolution;
    double bestQuality;
    bool found;
};

