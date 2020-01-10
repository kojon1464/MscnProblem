#include "pch.h"
#include "RandomSearch.h"
#include "Util.h"
#include <iostream>

RandomSearch::RandomSearch(MscnProblem* problem, Exception& exception)
{
    if (problem == NULL)
    {
        exception.setOcurred(true);
    }

    int d, f, m, s;
    d = problem->getNumberOfDeliverers();
    f = problem->getNumberOfFactories();
    m = problem->getNumberOfMagazines();
    s = problem->getNumberOfStores();

    tempSolution = Solution(d, f, m, s, exception);
    bestSolution = Solution(d, f, m, s, exception);

    this->problem = problem;
    bestQuality = 0;
    found = false;
}

RandomSearch::~RandomSearch()
{
    problem = NULL;
}

Exception RandomSearch::getBestSolution(Solution& bestSolution)
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
    util::generateRandomSolution(*problem, tempSolution);

    bool result;
    problem->constraintsSatified(tempSolution, result);

    if (result)
    {
        double tempQuality = 0;
        problem->getQuality(tempSolution, tempQuality);
        if (tempQuality > bestQuality)
        {
            bestQuality = tempQuality;
            bestSolution = tempSolution;
            found = true;
            std::cout << "found better solution, quality:" << tempQuality << std::endl;
        }
    }

    return Exception(false);
}
