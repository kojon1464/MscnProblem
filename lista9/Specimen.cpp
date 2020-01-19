#include "pch.h"
#include "Specimen.h"
#include "Util.h"

Specimen::Specimen(Problem* problem, Solution* solution, Exception& exception)
{
	if (problem == NULL || solution == NULL || problem->getRequiredSolutionLenght() != solution->getSolutionLenght())
	{
		exception.setOcurred(true);
	}
	this->problem = problem;
	this->solution = solution->clone();
}

Specimen::Specimen(const Specimen& other)
{
	copy(other);
}

Specimen::~Specimen()
{
	remove();
}

Specimen& Specimen::operator=(const Specimen& other)
{
	if (&other == this) 
	{
		return *this;
	}
	remove();
	copy(other);
	return *this;
}

Exception Specimen::randomizeSolution()
{
	return util::generateRandomSolution(*problem, *solution);
}

Exception Specimen::getQuality(double& result)
{
	return problem->getQuality(*solution, result);
}

Exception Specimen::getQualityAndFix(double & result)
{
    return problem->getQualityAndFix(*solution, result);
}

Exception Specimen::constraintsSatified(bool& result)
{
	return problem->constraintsSatified(*solution, result);
}

Exception Specimen::setClampedValue(int index, double value)
{
	if (problem->getRequiredSolutionLenght() != solution->getSolutionLenght() || index < 0 || index >= solution->getSolutionLenght())
	{
		return Exception(true);
	}
	double min, max;
	problem->getSolutionBounds(index, min, max);
	value = std::max(min, std::min(max, value));
	solution->setValue(index, value);
	return Exception(false);
}

Exception Specimen::setValue(int index, double value)
{
    if (problem->getRequiredSolutionLenght() != solution->getSolutionLenght() || index < 0 || index >= solution->getSolutionLenght())
    {
        return Exception(true);
    }
    solution->setValue(index, value);
    return Exception(false);
}

Exception Specimen::getValue(int index, double& value)
{
	return solution->getValue(index, value);
}

int Specimen::getSolutionSize()
{
	return solution->getSolutionLenght();
}

Solution& Specimen::getSolution()
{
    return *solution;
}

void Specimen::remove()
{
	if (solution != NULL)
	{
		delete solution;
	}
	problem = NULL;
}

void Specimen::copy(const Specimen& other)
{
	solution = other.solution->clone();
	problem = other.problem;
}
