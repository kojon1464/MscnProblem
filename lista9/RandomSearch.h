#pragma once

#include "Array.h"
#include "MscnProblem.h"

class RandomSearch
{
public:
    RandomSearch(MscnProblem* problem, Exception& exception);
    ~RandomSearch();

    Exception getBestSolution(Array& bestSolution);
    Exception iterate();
private:
    MscnProblem* problem;
    Array bestSolution;
    double bestQuality;
    bool found;
};

