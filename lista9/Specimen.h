#pragma once

#include "Solution.h"
#include "Problem.h"

class Specimen
{
public:
	Specimen(Problem* problem, Solution* solution, Exception& exception);
	Specimen(const Specimen& other);
	~Specimen();

	Specimen& operator=(const Specimen& other);

	Exception randomizeSolution();
	Exception getQuality(double& result);
	Exception constraintsSatified(bool& result);

	Exception setClampedValue(int index, double value);
	Exception getValue(int index, double& value);
	int getSolutionSize();

    Solution& getSolution();
private:
	Solution* solution;
	Problem* problem;

	void remove();
	void copy(const Specimen& other);
};

