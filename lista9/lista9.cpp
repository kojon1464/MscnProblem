#include "pch.h"
#include "MscnProblem.h"
#include "Solution.h"
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

    MscnProblem problem;
    problem.generateInstance(20);
    Exception exception;

    int TIME_LIMIT = 10;
    int QUALITY_INVOKE_LIMIT = 1000;

    clock_t begin = clock();

    RandomSearch search(&problem, exception);

    while((clock() - begin)/CLOCKS_PER_SEC <= TIME_LIMIT && problem.getQualityInvokeCounter() <= QUALITY_INVOKE_LIMIT)
    {
        search.iterate();
    }

    Solution solution;
    if (search.getBestSolution(solution).getOcurred()) 
    {
        std::cout << "Nie znaleziono rozwiazania" << std::endl;
    }
    else
    {
        double quality;
        problem.getQuality(solution, quality);
        std::cout << "bestSolution quality: " << quality << std::endl;
        solution.writeToFile("bestSolution.txt");
    }
}