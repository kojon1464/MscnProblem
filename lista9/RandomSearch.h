#pragma once

#include "Array.h"
#include "Solution.h"
#include "Problem.h"
#include "Optimizer.h"

class RandomSearch : public Optimizer
{
public:
    RandomSearch(Problem* problem, Solution* solution, Exception& exception);
	RandomSearch(const RandomSearch& other);
    ~RandomSearch();

	RandomSearch& operator=(const RandomSearch& other);

	Exception getBestQuality(double& bestQuality);
    Exception getBestSolution(Solution& bestSolution);
    Exception iterate();
private:
    Problem* problem;
    Solution* tempSolution;
    Solution* bestSolution;
    double bestQuality;
    bool found;

	void remove();
	void copy(const RandomSearch& other);
};

