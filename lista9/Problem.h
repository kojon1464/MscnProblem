#pragma once
class Problem
{
public:
	virtual bool sameSize(Solution& solution) = 0;

	virtual Exception getQuality(double* solution, double& result) = 0;
	virtual Exception getQuality(Solution& solution, double& result) = 0;
    virtual Exception getQualityAndFix(Solution& solution, double& result) = 0;
	virtual Exception constraintsSatified(double* solution, bool& result) = 0;
	virtual Exception constraintsSatified(Solution& solution, bool& result) = 0;

	virtual Exception writeToFile(std::string path) = 0;
	virtual Exception readFormFile(std::string path) = 0;

	virtual void generateInstance(long unsigned int seed) = 0;

	virtual Exception getSolutionBounds(int index, double& min, double& max) = 0;
	virtual int getRequiredSolutionLenght() = 0;
	virtual int getQualityInvokeCounter() = 0;
};

