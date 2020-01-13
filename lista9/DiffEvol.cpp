#include "pch.h"
#include "DiffEvol.h"
#include <iostream>

const double DEFAULT_CROSS_PROBABILITY = 0.5;
const double DEFAULT_DIFFERENCE_WEIGHT = 0.2;

DiffEvol::DiffEvol(MscnProblem* problem, Exception& exception)
{
    if (problem == NULL)
    {
        exception.setOcurred(true);
    }
    this->problem = problem;
    population.initializate(*problem);
}

Exception DiffEvol::getBestQuality(double& bestQuality)
{
    return Exception();
}

DiffEvol::~DiffEvol()
{
}

Exception DiffEvol::getBestSolution(Solution& bestSolution)
{
    return population.getBestSolution(bestSolution);
}

Exception DiffEvol::iterate()
{
	for (int i = 0; i < population.getPopulationSize(); i++)
	{
		Specimen* current = population[i];
		Specimen* base = NULL;
		Specimen* add1 = NULL;
		Specimen* add2 = NULL;
		do
		{
			population.getRandomSpecimen(base);
			population.getRandomSpecimen(add1);
			population.getRandomSpecimen(add2);
		} while (!differentSpecimens(current, base, add1, add2));

		Specimen* newSpecimen = new Specimen(*current);
		for (int j = 0; j < current->getSolutionSize(); j++) 
		{
			Random random;
			if (random.getDouble(0, 1) < crossProbability)
			{
				double baseValue, add1Value, add2Value;
				base->getValue(j, baseValue);
				add1->getValue(j, add1Value);
				add2->getValue(j, add2Value);
				newSpecimen->setClampedValue(j, baseValue + differenceWeight * (add1Value - add2Value));
			}
		}

		bool constraintsSatisfied;
		double oldQuality, newQuality;
		current->getQuality(oldQuality);
		newSpecimen->constraintsSatified(constraintsSatisfied);
		newSpecimen->getQuality(newQuality);
		if (constraintsSatisfied && newQuality > oldQuality)
		{
			*current = *newSpecimen;
			std::cout << "found better specimen, quality: " << newQuality << std::endl;
		}
		delete newSpecimen;
	}
	return Exception(false);
}

Exception DiffEvol::setCrossProbability(double crossProbability)
{
    if (crossProbability < 0 || crossProbability > 1)
    {
        return Exception(true);
    }
    this->crossProbability = crossProbability;
    return Exception(false);
}

Exception DiffEvol::setDifferenceWeight(double differenceWeight)
{
    this->differenceWeight = differenceWeight;
    return Exception(false);
}

double DiffEvol::getCrossProbability()
{
    return crossProbability;
}

double DiffEvol::getDifferenceWeight()
{
    return differenceWeight;
}

bool DiffEvol::differentSpecimens(Specimen* specimen1, Specimen* specimen2, Specimen* specimen3, Specimen* specimen4)
{
	if (specimen1 == specimen2 || specimen1 == specimen3 || specimen1 == specimen4 ||
		specimen2 == specimen4 || specimen2 == specimen4 ||
		specimen3 == specimen2)
	{
		return false;
	}
	return true;
}
