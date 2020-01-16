#pragma once

#include "Array.h"
#include "Exception.h"
#include <string>

class Solution
{
public:
	virtual Exception readFormFile(std::string path) = 0;
	virtual Exception writeToFile(std::string path) = 0;

	virtual Exception loadFromArray(Array& array) = 0;
	virtual Exception toArray(double*& array) = 0;

	virtual int getSolutionLenght() = 0;

	virtual Exception setValue(int index, double value) = 0;
	virtual Exception getValue(int index, double& value) = 0;

	virtual Solution* clone() = 0;
};

