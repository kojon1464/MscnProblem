#include "pch.h"
#include "MscnProblem.h"
#include "Solution.h"
#include "MscnSolution.h"
#include "Random.h"
#include "RandomSearch.h"
#include "DiffEvol.h"
#include <iostream>
#include <ctime>

int main()
{
	/*MscnProblem problem;
	
    if (problem.readFormFile("text.txt").getOcurred())
    {
        std::cout << "blad";
        return -1;
    }
	double quality;
	//double solution[11] = {3677.99, 297.689, 402.775, 1700.9, 848.95, 225.568, 900.983, 460.027, 848.94, 225.152, 769.998};
    double* array;
    Solution solution;
    if (solution.readFormFile("solution.txt").getOcurred())
    {
        std::cout << "blad";
        return -1;
    }
    solution.toArray(array);
	bool satisfied;
	problem.getQuality(array, quality);
	problem.constraintsSatified(array, satisfied);
	std::cout << quality << std::endl;
	std::cout << satisfied << std::endl;
	problem.writeToFile("result.txt");
	double min, max;
	problem.getSolutionBounds(7, min, max);
	std::cout << min << std::endl;
	std::cout << max << std::endl;
    delete[] array;

    Random random(10);
    std::cout << random.getInt(-100, -5) << std::endl;
    std::cout << random.getInt(-100, -5) << std::endl;
    std::cout << random.getInt(-100, -5) << std::endl;*/

	int D = 3;
	int F = 3;
	int M = 3;
	int S = 3;

	Exception exception;
    Problem* problem = new MscnProblem(D, F, M, S, exception);
    problem->generateInstance(20);
	Solution* solution = new MscnSolution(D, F, M, S, exception);

	Solution* solution2 = new MscnSolution();
	*solution2 = *solution;

    int TIME_LIMIT = 3;
    int QUALITY_INVOKE_LIMIT = 1000000;

    clock_t begin = clock();

    Optimizer* search = new RandomSearch(problem, solution, exception);

    while((clock() - begin)/CLOCKS_PER_SEC <= TIME_LIMIT && problem->getQualityInvokeCounter() <= QUALITY_INVOKE_LIMIT)
    {
        search->iterate();
    }

    MscnSolution* bestSolution = new MscnSolution(D, F, M, S, exception);
    if (search->getBestSolution(*bestSolution).getOcurred())
    {
        std::cout << "Nie znaleziono rozwiazania" << std::endl;
    }
    else
    {
        double quality;
        problem->getQuality(*bestSolution, quality);
        std::cout << "bestSolution quality: " << quality << std::endl;
		bestSolution->writeToFile("bestSolution.txt");
    }

	delete problem;
	delete solution;
	delete search;
	delete bestSolution;
}