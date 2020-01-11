#include "pch.h"
#include "DiffEvol.h"

DiffEvol::DiffEvol(MscnProblem* problem, Exception& exception)
{
    if (problem == NULL)
    {
        exception.setOcurred(true);
    }
    this->problem = problem;
    population.initializate(*problem);
}

DiffEvol::~DiffEvol()
{
}

Exception DiffEvol::getBestSolution(Solution& bestSolution)
{
    population.getBestSolution(bestSolution);
}
