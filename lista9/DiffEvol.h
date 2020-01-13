#pragma once

#include "Population.h"
#include "MscnProblem.h"
#include "Solution.h"

class DiffEvol
{
public:
    static const double DEFAULT_CROSS_PROBABILITY;
    static const double DEFAULT_DIFFERENCE_WEIGHT;

    DiffEvol(MscnProblem* problem, Exception& exception);
    Exception getBestQuality(double& bestQuality);
    ~DiffEvol();

    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();

    Exception setCrossProbability(double crossProbability);
    Exception setDifferenceWeight(double differenceWeight);

    double getCrossProbability();
    double getDifferenceWeight();
private:
    double crossProbability;
    double differenceWeight;

    MscnProblem* problem;
    Population population;

	bool differentSpecimens(Specimen* specimen1, Specimen* specimen2, Specimen* specimen3, Specimen* specimen4);
};

