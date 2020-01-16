#include "pch.h"
#include "RandomSearch.h"
#include "Util.h"
#include <iostream>

RandomSearch::RandomSearch(Problem* problem, Solution* solution, Exception& exception)
{
	if (problem == NULL || solution == NULL || solution->getSolutionLenght() != problem->getRequiredSolutionLenght())
    {
        exception.setOcurred(true);
    }

    bestSolution = solution->clone();
	tempSolution = solution->clone();

    this->problem = problem;
    bestQuality = 0;
    found = false;
}

RandomSearch::RandomSearch(const RandomSearch& other)
{
	copy(other);
}

RandomSearch::~RandomSearch()
{
	remove();
}

RandomSearch & RandomSearch::operator=(const RandomSearch & other)
{
	if (this == &other)
	{
		return *this;
	}
	remove();
	copy(other);
	return *this;
}

Exception RandomSearch::getBestQuality(double& bestQuality)
{
	return this->bestQuality;
}

Exception RandomSearch::getBestSolution(Solution& bestSolution)
{
    if (!found)
    {
        return Exception(true);
    }

    bestSolution = *(this->bestSolution);

    return Exception(false);
}

Exception RandomSearch::iterate()
{
    util::generateRandomSolution(*problem, *tempSolution);

    bool result;
    problem->constraintsSatified(*tempSolution, result);

    if (result)
    {
        double tempQuality = 0;
        problem->getQuality(*tempSolution, tempQuality);
        if (tempQuality > bestQuality)
        {
			Solution* temp = bestSolution;
			bestSolution = tempSolution;
			tempSolution = temp;
            bestQuality = tempQuality;
            found = true;
            std::cout << "found better solution, quality:" << tempQuality << std::endl;
        }
    }

    return Exception(false);
}

void RandomSearch::remove()
{
	problem = NULL;
	delete tempSolution;
	delete bestSolution;
}

void RandomSearch::copy(const RandomSearch& other)
{
	problem = other.problem;
	bestQuality = other.bestQuality;
	found = other.found;
	tempSolution = other.tempSolution->clone();
	bestSolution = other.bestSolution->clone();
}
