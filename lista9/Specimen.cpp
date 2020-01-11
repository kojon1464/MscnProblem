#include "pch.h"
#include "Specimen.h"
#include "Util.h"

Specimen::Specimen(MscnProblem* problem, Exception& exception)
{
	if (problem == NULL)
	{
		exception.setOcurred(true);
	}
	this->problem = problem;
	solution.setSize(problem->getNumberOfDeliverers(), problem->getNumberOfFactories(), problem->getNumberOfMagazines(), problem->getNumberOfStores());
}

Specimen::~Specimen()
{
}

Exception Specimen::randomizeSolution()
{
	return util::generateRandomSolution(*problem, solution);
}

Exception Specimen::getQuality(double& result)
{
	return problem->getQuality(solution, result);
}

Exception Specimen::constraintsSatified(bool& result)
{
	return problem->constraintsSatified(solution, result);
}

Solution& Specimen::getSolution()
{
    return solution;
}
