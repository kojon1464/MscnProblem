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
private:
	Solution solution;
	MscnProblem* problem;
};

