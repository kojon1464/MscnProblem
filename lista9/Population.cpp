#include "pch.h"
#include "Population.h"
#include "pch.h"
#include <stddef.h>

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
