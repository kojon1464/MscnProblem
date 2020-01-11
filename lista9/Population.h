#pragma once

#include "Specimen.h"
#include "Exception.h"
#include "MscnProblem.h"
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

	Exception initializate(MscnProblem& problem);
private:
	std::vector<Specimen*> population;
	int populationSize;

	void copy(const Population& other);
	void remove();
};

