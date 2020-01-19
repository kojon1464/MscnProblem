#include "pch.h"
#include "MscnProblem.h"
#include "Solution.h"
#include "MscnSolution.h"
#include "Random.h"
#include "Timer.h"
#include "RandomSearch.h"
#include "DiffEvol.h"
#include <iostream>
#include <ctime>

int main()
{
	int D = 3;
	int F = 3;
	int M = 3;
	int S = 3;

	Exception exception;
    Problem* problem = new MscnProblem(D, F, M, S, exception);
    problem->generateInstance(20);
	Solution* solution = new MscnSolution(D, F, M, S, exception);

    int TIME_LIMIT = 10;
    int QUALITY_INVOKE_LIMIT = 1000000;

    Timer timer;

    Optimizer* search = new DiffEvol(problem, solution, exception);

    timer.start();
    while(timer.getTime() <= TIME_LIMIT && problem->getQualityInvokeCounter() <= QUALITY_INVOKE_LIMIT)
    {
        search->iterate();
    }

    Solution* bestSolution = new MscnSolution(D, F, M, S, exception);
    if (search->getBestSolution(bestSolution).getOcurred())
    {
        std::cout << "Nie znaleziono rozwiazania" << std::endl;
    }
    else
    {
        double quality;
        bool constraints;
        problem->getQuality(*bestSolution, quality);
        problem->constraintsSatified(*bestSolution, constraints);
        std::cout << "bestSolution quality: " << quality << " constarinst: " << constraints << std::endl;
		bestSolution->writeToFile("bestSolution.txt");
    }

	delete problem;
	delete solution;
	delete search;
	delete bestSolution;
}