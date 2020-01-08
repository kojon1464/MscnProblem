#include "pch.h"
#include "RandomSearch.h"
#include <iostream>

RandomSearch::RandomSearch(MscnProblem* problem, Exception& exception)
{
    if (problem == NULL)
    {
        exception.setOcurred(true);
    }

    this->problem = problem;
    bestSolution.setSize(problem->getRequiredSolutionLenght());
    bestQuality = 0;
    found = false;
}

RandomSearch::~RandomSearch()
{
    problem = NULL;
}

Exception RandomSearch::getBestSolution(Array& bestSolution)
{
    if (!found)
    {
        return Exception(true);
    }

    bestSolution = this->bestSolution;

    return Exception(false);
}

Exception RandomSearch::iterate()
{
    if (bestSolution.getSize() != problem->getRequiredSolutionLenght())
    {
        return Exception(true);
    }

    Random random;
    double min, max;
    Exception exception;
    Array tempSolution(bestSolution.getSize(), exception);

    for (int i = 0; i < bestSolution.getSize(); i++)
    {
        problem->getSolutionBounds(i, min, max);
        tempSolution[i] = random.getDouble(min, max);
    }

    bool result = false;
    problem->constraintsSatified(tempSolution.getInternalArray(), result);
    if (result)
    {
        double tempQuality = 0;
        problem->getQuality(tempSolution.getInternalArray(), tempQuality);
        if (tempQuality > bestQuality)
        {
            bestQuality = tempQuality;
            bestSolution = tempSolution;
            std::cout << "found better solution, quality:" << tempQuality << std::endl;
        }
    }

    return Exception(false);
}
