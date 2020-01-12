#pragma once

#include "Solution.h"
#include "MscnProblem.h"

class Specimen
{
public:
	Specimen(MscnProblem* problem, Exception& exception);
	~Specimen();

	Exception randomizeSolution();
	Exception getQuality(double& result);
	Exception constraintsSatified(bool& result);

	Exception setClampedValue(int index, double value);
	Exception getValue(int index, double& value);
	int getSolutionSize();

    Solution& getSolution();
private:
	Solution solution;
	MscnProblem* problem;
};

