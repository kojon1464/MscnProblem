#pragma once

#include "Population.h"
#include "MscnProblem.h"
#include "Solution.h"

class DiffEvol
{
public:
    DiffEvol(MscnProblem* problem, Exception& exception);
    ~DiffEvol();

    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();
private:
    MscnProblem* problem;
    Population population;
};

