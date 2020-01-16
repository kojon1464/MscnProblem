#pragma once

#include "Specimen.h"
#include "Exception.h"
#include "Problem.h"
#include <vector>

class Population
{
public:
	static const int DEFAULT_POPULATION_SIZE;

	Population();
	Population(int populationSize, Exception& exception);
	Population(const Population& other);
	~Population();

	Population& operator=(const Population& other);
    Specimen* operator[](const int index);

	Exception initializate(Problem* problem, Solution* solution);
    Exception getBestSolution(Solution& solution);

	Exception getRandomSpecimen(Specimen*& specimen);

	int getPopulationSize();
private:
	std::vector<Specimen*> population;
	int populationSize;

	void copy(const Population& other);
	void remove();
};

