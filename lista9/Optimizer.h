#pragma once

#include "Exception.h"
#include "Solution.h"

class Optimizer
{
public:
	virtual Exception getBestQuality(double& bestQuality) = 0;
	virtual Exception getBestSolution(Solution*& bestSolution) = 0;
	virtual Exception iterate() = 0;
};

