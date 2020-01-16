#pragma once

#include "Population.h"
#include "Problem.h"
#include "Solution.h"
#include "Optimizer.h"

class DiffEvol : public Optimizer
{
public:
    static const double DEFAULT_CROSS_PROBABILITY;
    static const double DEFAULT_DIFFERENCE_WEIGHT;

    DiffEvol(Problem* problem, Solution* solution , Exception& exception);
    ~DiffEvol();

	Exception getBestQuality(double& bestQuality);
    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();

    Exception setCrossProbability(double crossProbability);
    Exception setDifferenceWeight(double differenceWeight);

    double getCrossProbability();
    double getDifferenceWeight();
private:
    double crossProbability;
    double differenceWeight;

    Problem* problem;
    Population population;

	bool differentSpecimens(Specimen* specimen1, Specimen* specimen2, Specimen* specimen3, Specimen* specimen4);
};

