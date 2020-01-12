#include "pch.h"
#include "Solution.h"

class MscnProblem;

const int Solution::DEFAULT_FACILITIES_NUMBER = 10;
const std::string Solution::DELIVERERES = "D";
const std::string Solution::FACTORIES = "F";
const std::string Solution::MAGAZINES = "M";
const std::string Solution::STORES = "S";
const std::string Solution::MATRIX_DELIVERERS = "xd";
const std::string Solution::MATRIX_FACTORIES = "xf";
const std::string Solution::MATRIX_MAGAZINES = "xm";


Solution::Solution()
{
	setSize(DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER);
}

Solution::Solution(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores, Exception& exception)
{
	exception = setSize(numberOfDeliverers, numberOfFactories, numberOfMagazines, numberOfStores);
}

Solution::~Solution()
{
}

Exception Solution::readFormFile(std::string path)
{
    FILE* file = fopen(path.c_str(), "r+");
    if (file == NULL)
    {
        return Exception(true);
    }
    {
        bool fail1, fail2, fail3, fail4;
        int deliverers, factories, magazines, stores;
        fail1 = fscanf(file, (DELIVERERES + " %i ").c_str(), &deliverers) < 1;
        fail2 = fscanf(file, (FACTORIES + " %i ").c_str(), &factories) < 1;
        fail3 = fscanf(file, (MAGAZINES + " %i ").c_str(), &magazines) < 1;
        fail4 = fscanf(file, (STORES + " %i ").c_str(), &stores) < 1;

        if (fail1 || fail2 || fail3 || fail4)
        {
            fclose(file);
            return Exception(true);
        }

        setNumberOfDeliverers(deliverers);
        setNumberOfFactories(factories);
        setNumberOfMagazines(magazines);
        setNumberOfStores(stores);
    }
    {
        bool errorOcurred1, errorOcurred2, errorOcurred3;
        errorOcurred1 = xd.readMatrixFromFile(file, MATRIX_DELIVERERS).getOcurred();
        errorOcurred2 = xf.readMatrixFromFile(file, MATRIX_FACTORIES).getOcurred();
        errorOcurred3 = xm.readMatrixFromFile(file, MATRIX_MAGAZINES).getOcurred();

        if (errorOcurred1 || errorOcurred2 || errorOcurred3)
        {
            fclose(file);
            return Exception(true);
        }
    }

    fclose(file);
    return Exception(false);
}


Exception Solution::writeToFile(std::string path)
{
    FILE* file = fopen(path.c_str(), "w+");
    if (file == NULL)
    {
        return Exception(true);
    }

    {
        bool fail1, fail2, fail3, fail4;
        fail1 = fprintf(file, (DELIVERERES + " %d\n").c_str(), numberOfDeliverers) <= 0;
        fail2 = fprintf(file, (FACTORIES + " %d\n").c_str(), numberOfFactories) <= 0;
        fail3 = fprintf(file, (MAGAZINES + " %d\n").c_str(), numberOfMagazines) <= 0;
        fail4 = fprintf(file, (STORES + " %d\n").c_str(), numberOfStores) <= 0;

        if (fail1 || fail2 || fail3 || fail4)
        {
            fclose(file);
            return Exception(true);
        }
    }
    {
        bool errorOcurred1, errorOcurred2, errorOcurred3;
        errorOcurred1 = xd.writeMatrixToFile(file, MATRIX_DELIVERERS).getOcurred();
        errorOcurred2 = xf.writeMatrixToFile(file, MATRIX_FACTORIES).getOcurred();
        errorOcurred3 = xm.writeMatrixToFile(file, MATRIX_MAGAZINES).getOcurred();

        if (errorOcurred1 || errorOcurred2 || errorOcurred3)
        {
            fclose(file);
            return Exception(true);
        }
    }

    fclose(file);
    return Exception(false);
}

Exception Solution::loadFromArray(Array& array)
{
    if (array.getSize() != getSolutionLenght())
    {
        return Exception(true);
    }

    int offset = 0;
    Exception exception;
    Matrix xdTemp(numberOfDeliverers, numberOfFactories, array.getInternalArray(), exception);
    offset += numberOfDeliverers * numberOfFactories;
    Matrix xfTemp(numberOfFactories, numberOfMagazines, array.getInternalArray() + offset, exception);
    offset += numberOfFactories * numberOfMagazines;
    Matrix xmTemp(numberOfMagazines, numberOfStores, array.getInternalArray() + offset, exception);

    xd = xdTemp;
    xf = xfTemp;
    xm = xmTemp;

    if (exception.getOcurred())
    {
        return exception;
    }

    return Exception(false);
}

Exception Solution::toArray(double *& array)
{
    int solutionLenght = getSolutionLenght();
    double* temp = new double[solutionLenght];
    int counter = 0;
    for (int i = 0; i < xd.getSizeY(); i++)
    {
        for (int j = 0; j < xd.getSizeX(); j++)
        {
            temp[counter++] = xd[i][j];
        }
    }
    for (int i = 0; i < xf.getSizeY(); i++)
    {
        for (int j = 0; j < xf.getSizeX(); j++)
        {
            temp[counter++] = xf[i][j];
        }
    }
    for (int i = 0; i < xm.getSizeY(); i++)
    {
        for (int j = 0; j < xm.getSizeX(); j++)
        {
            temp[counter++] = xm[i][j];
        }
    }
    array = temp;
    return Exception(false);
}

int Solution::getSolutionLenght()
{
    return numberOfDeliverers * numberOfFactories +
        numberOfFactories * numberOfMagazines +
        numberOfMagazines * numberOfStores;
}

Exception Solution::setSize(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores)
{
	if (numberOfDeliverers <= 0 || numberOfFactories <= 0 || numberOfMagazines <= 0 || numberOfStores <= 0)
	{
		return Exception(true);
	}

	setNumberOfDeliverers(numberOfDeliverers);
	setNumberOfFactories(numberOfFactories);
	setNumberOfMagazines(numberOfMagazines);
	setNumberOfStores(numberOfStores);

	return Exception(false);
}

Exception Solution::setNumberOfDeliverers(int numberOfDeliverers)
{
    if (numberOfDeliverers <= 0)
    {
        return Exception(true);
    }

    this->numberOfDeliverers = numberOfDeliverers;

    xd.setSizeY(numberOfDeliverers);

    return Exception(false);
}

Exception Solution::setNumberOfFactories(int numberOfFactories)
{
    if (numberOfFactories <= 0)
    {
        return Exception(true);
    }

    this->numberOfFactories = numberOfFactories;

    xd.setSizeX(numberOfFactories);
    xf.setSizeY(numberOfFactories);

    return Exception(false);
}

Exception Solution::setNumberOfMagazines(int numberOfMagazines)
{
    if (numberOfMagazines <= 0)
    {
        return Exception(true);
    }

    this->numberOfMagazines = numberOfMagazines;

    xf.setSizeX(numberOfMagazines);
    xm.setSizeY(numberOfMagazines);

    return Exception(false);
}

Exception Solution::setNumberOfStores(int numberOfStores)
{
    if (numberOfStores <= 0)
    {
        return Exception(true);
    }

    this->numberOfStores = numberOfStores;

    xm.setSizeX(numberOfStores);

    return Exception(false);
}

Exception Solution::setValue(int index, double value)
{
	int solutionLenght = getSolutionLenght();
	int xdLength = numberOfDeliverers * numberOfFactories;
	int xfLength = numberOfFactories * numberOfMagazines;

	if (index < 0 || index >= solutionLenght)
	{
		return Exception(true);
	}

	if (index < xdLength)
	{
		xd[index / xd.getSizeX()][index % xd.getSizeX()] = value;
		return Exception(false);
	}
	if (index < xdLength + xfLength)
	{
		int correctedIndex = index - xdLength;
		xf[correctedIndex / xf.getSizeX()][correctedIndex % xf.getSizeX()] = value;
		return Exception(false);
	}
	int correctedIndex = index - (xdLength + xfLength);
	xm[correctedIndex / xm.getSizeX()][correctedIndex % xm.getSizeX()] = value;

	return Exception(false);
}

Exception Solution::getValue(int index, double& value)
{
	int solutionLenght = getSolutionLenght();
	int xdLength = numberOfDeliverers * numberOfFactories;
	int xfLength = numberOfFactories * numberOfMagazines;

	if (index < 0 || index >= solutionLenght)
	{
		return Exception(true);
	}

	if (index < xdLength)
	{
		value = xd[index / xd.getSizeX()][index % xd.getSizeX()];
		return Exception(false);
	}
	if (index < xdLength + xfLength)
	{
		int correctedIndex = index - xdLength;
		value = xf[correctedIndex / xf.getSizeX()][correctedIndex % xf.getSizeX()];
		return Exception(false);
	}
	int correctedIndex = index - (xdLength + xfLength);
	value = xm[correctedIndex / xm.getSizeX()][correctedIndex % xm.getSizeX()];

	return Exception(false);
}

int Solution::getNumberOfDeliverers()
{
	return numberOfDeliverers;
}

int Solution::getNumberOfFactories()
{
	return numberOfFactories;
}

int Solution::getNumberOfMagazines()
{
	return numberOfMagazines;
}

int Solution::getNumberOfStores()
{
	return numberOfStores;
}

Matrix& Solution::getDeliverersMatrix()
{
    return xd;
}

Matrix& Solution::getFactoriesMatrix()
{
    return xf;
}

Matrix& Solution::getMagazinesMatrix()
{
    return xm;
}
