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

Exception Specimen::setClampedValue(int index, double value)
{
	if (problem->getRequiredSolutionLenght() != solution.getSolutionLenght() || index < 0 || index >= solution.getSolutionLenght())
	{
		return Exception(true);
	}
	double min, max;
	problem->getSolutionBounds(index, min, max);
	value = std::max(min, std::min(max, value));
	solution.setValue(index, value);
	return Exception(false);
}

Exception Specimen::getValue(int index, double& value)
{
	return solution.getValue(index, value);
}

int Specimen::getSolutionSize()
{
	return solution.getSolutionLenght();
}

Solution& Specimen::getSolution()
{
    return solution;
}
