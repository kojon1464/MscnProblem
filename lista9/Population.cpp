#include "pch.h"
#include "Population.h"

const int Population::DEFAULT_POPULATION_SIZE = 20;

Population::Population()
{
	populationSize = DEFAULT_POPULATION_SIZE;
}

Population::Population(int populationSize, Exception& exception)
{
	if (populationSize <= 0)
	{
		exception.setOcurred(true);
	}

	this->populationSize = populationSize;
}

Population::Population(const Population& other)
{
	copy(other);
}

Population::~Population()
{
	remove();
}

Population& Population::operator=(const Population& other)
{
	if (&other == this)
	{
		return *this;
	}

	remove();
	copy(other);
	return *this;
}

Specimen* Population::operator[](const int index)
{
    if (index < 0 || index >= populationSize)
    {
        throw Exception(true);
    }
    return population[index];
}

Exception Population::initializate(MscnProblem& problem)
{
	for (int i = 0; i < populationSize; i++)
	{
		Exception exception;
		Specimen* tempSpecimen = new Specimen(&problem, exception);
		bool constraintsSatified = false;
		do 
		{
			tempSpecimen->randomizeSolution();
			tempSpecimen->constraintsSatified(constraintsSatified);
		} while (!constraintsSatified);
		population.push_back(tempSpecimen);
	}
    return Exception(false);
}

Exception Population::getBestSpecimen(solution& solutiion)
{
    if (population.size() != populationSize)
    {
        return Exception(true);
    }
    Specimen* best;
    double bestQuality = -DBL_MAX;
    for (int i = 0; i < population.size(); i++)
    {
        double tempQuality;
        population[i]->getQuality(tempQuality);
        if (tempQuality > bestQuality)
        {
            best = population[i];
            bestQuality = tempQuality;
        }
    }
    solution = best->getSolution();
    return Exception(false);
}

void Population::copy(const Population& other)
{
	this->populationSize = other.populationSize;
	for (int i = 0; i < populationSize; i++)
	{
		population.push_back(new Specimen(*(other.population[i])));
	}
}

void Population::remove()
{
	for (int i = 0; i < population.size(); i++)
	{
		delete population[i];
	}
	population.clear();
}
