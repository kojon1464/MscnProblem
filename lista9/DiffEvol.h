#pragma once

#include "Population.h"
#include "MscnProblem.h"
#include "Solution.h"

class DiffEvol
{
public:
	static const double CROSS_PROBABILITY;
	static const double DIFFERENCE_WEIGHT;

    DiffEvol(MscnProblem* problem, Exception& exception);
    ~DiffEvol();

    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();
private:
    MscnProblem* problem;
    Population population;

	bool differentSpecimens(Specimen* specimen1, Specimen* specimen2, Specimen* specimen3, Specimen* specimen4);
};

