#include "pch.h"
#include "MscnProblem.h"
#include "Util.h"
#include <iostream>

const std::string MscnProblem::SEPARATOR = ";";
const int MscnProblem::DEFAULT_FACILITIES_NUMBER = 10;
const std::string MscnProblem::DELIVERERES = "D";
const std::string MscnProblem::FACTORIES = "F";
const std::string MscnProblem::MAGAZINES = "M";
const std::string MscnProblem::STORES = "S";
const std::string MscnProblem::LIMIT_ARRAY_DELIVERERS = "sd";
const std::string MscnProblem::LIMIT_ARRAY_FACTORIES = "sf";
const std::string MscnProblem::LIMIT_ARRAY_MAGAZINES = "sm";
const std::string MscnProblem::LIMIT_ARRAY_STORES = "ss";
const std::string MscnProblem::COST_MATRIX_DELIVERERS = "cd";
const std::string MscnProblem::COST_MATRIX_FACTORIES = "cf";
const std::string MscnProblem::COST_MATRIX_MAGAZINES = "cm";
const std::string MscnProblem::COST_ARRAY_DELIVERERS = "ud";
const std::string MscnProblem::COST_ARRAY_FACTORIES = "uf";
const std::string MscnProblem::COST_ARRAY_MAGAZINES = "um";
const std::string MscnProblem::PRICE_ARRAY_STORE = "p";
const std::string MscnProblem::MIN_MAX_MATRIXES_DELIVERESR = "xdminmax";
const std::string MscnProblem::MIN_MAX_MATRIXES_FACTORIES = "xfminmax";
const std::string MscnProblem::MIN_MAX_MATRIXES_MAGAZINES = "xmminmax";

MscnProblem::MscnProblem()
{
    setNumberOfDeliverers(DEFAULT_FACILITIES_NUMBER);
    setNumberOfFactories(DEFAULT_FACILITIES_NUMBER);
    setNumberOfMagazines(DEFAULT_FACILITIES_NUMBER);
    setNumberOfStores(DEFAULT_FACILITIES_NUMBER);
}

MscnProblem::~MscnProblem()
{
}

Exception MscnProblem::getQuality(double* solution, double& result)
{
    Matrix xd, xf, xm;
    Exception except = solutionToMatricesAndCheckNonNative(solution, xd, xf, xm);

    if (except.getOcurred())
    {
        return except;
    }

    result = getIncome(xm) - getCost(xd, xf, xm) - getFixedCost(xd, xf, xm);
    return Exception(false);
}

Exception MscnProblem::constraintsSatified(double* solution, bool& result)
{
    result = true;
    Matrix xd, xf, xm;
    Exception except = solutionToMatricesAndCheckNonNative(solution, xd, xf, xm);

    if (except.getOcurred())
    {
        result = false;
        return except;
    }
    
    bool xdBound, xfBound, xmBound;
    xd.isInBounds(xdmin, xdmax, xdBound);
    xf.isInBounds(xfmin, xfmax, xfBound);
    xm.isInBounds(xmmin, xmmax, xmBound);

    if (!xdBound || !xfBound || !xmBound)
    {
        result = false;
    }

    double sum = 0;
    for (int i = 0; i < numberOfDeliverers; i++)
    {
        xd.getRowSum(i, sum);
        if (sum > sd[i])
        {
            result = false;
        }
    }

    for (int i = 0; i < numberOfFactories; i++)
    {
        xf.getRowSum(i, sum);
        if (sum > sf[i])
        {
            result = false;
        }
    }

    for (int i = 0; i < numberOfMagazines; i++)
    {
        xm.getRowSum(i, sum);
        if (sum > sm[i])
        {
            result = false;
        }
    }

    for (int i = 0; i < numberOfStores; i++)
    {
        xf.getColumnSum(i, sum);
        if (sum > ss[i])
        {
            result = false;
        }
    }

    double sum1 = 0;
    for (int i = 0; i < numberOfFactories; i++)
    {
        xd.getColumnSum(i, sum);
        xf.getRowSum(i, sum1);
        if (sum < sum1)
        {
            result = false;
        }
    }

    for (int i = 0; i < numberOfMagazines; i++)
    {
        xf.getColumnSum(i, sum);
        xm.getRowSum(i, sum1);
        if (sum < sum1)
        {
            result = false;
        }
    }
    return Exception(false);
}

Exception MscnProblem::writeToFile(std::string path)
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
		bool errorOcurred1, errorOcurred2, errorOcurred3, errorOcurred4;
		errorOcurred1 = sd.writeArrayToFile(file, LIMIT_ARRAY_DELIVERERS).getOcurred();
		errorOcurred2 = sf.writeArrayToFile(file, LIMIT_ARRAY_FACTORIES).getOcurred();
		errorOcurred3 = sm.writeArrayToFile(file, LIMIT_ARRAY_MAGAZINES).getOcurred();
		errorOcurred4 = ss.writeArrayToFile(file, LIMIT_ARRAY_STORES).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3 || errorOcurred4)
		{
			fclose(file);
			return Exception(true);
		}
	}
    {
        bool errorOcurred1, errorOcurred2, errorOcurred3;
        errorOcurred1 = cd.writeMatrixToFile(file, COST_MATRIX_DELIVERERS).getOcurred();
        errorOcurred2 = cf.writeMatrixToFile(file, COST_MATRIX_FACTORIES).getOcurred();
        errorOcurred3 = cm.writeMatrixToFile(file, COST_MATRIX_MAGAZINES).getOcurred();

        if (errorOcurred1 || errorOcurred2 || errorOcurred3)
        {
            fclose(file);
            return Exception(true);
        }
    }
    {
        bool errorOcurred1, errorOcurred2, errorOcurred3, errorOcurred4;
        errorOcurred1 = ud.writeArrayToFile(file, COST_ARRAY_DELIVERERS).getOcurred();
        errorOcurred2 = uf.writeArrayToFile(file, COST_ARRAY_FACTORIES).getOcurred();
        errorOcurred3 = um.writeArrayToFile(file, COST_ARRAY_MAGAZINES).getOcurred();
        errorOcurred4 = p.writeArrayToFile(file, PRICE_ARRAY_STORE).getOcurred();

        if (errorOcurred1 || errorOcurred2 || errorOcurred3 || errorOcurred4)
        {
            fclose(file);
            return Exception(true);
        }
    }
	{
		bool errorOcurred1, errorOcurred2, errorOcurred3;
		errorOcurred1 = writeMatrixBoundsToFile(file, xdmin, xdmax, MIN_MAX_MATRIXES_DELIVERESR).getOcurred();
		errorOcurred2 = writeMatrixBoundsToFile(file, xfmin, xfmax, MIN_MAX_MATRIXES_FACTORIES).getOcurred();
		errorOcurred3 = writeMatrixBoundsToFile(file, xmmin, xmmax, MIN_MAX_MATRIXES_MAGAZINES).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3)
		{
			fclose(file);
			return Exception(true);
		}
	}

    fclose(file);
    return Exception(false);
}

Exception MscnProblem::readFormFile(std::string path)
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
		bool errorOcurred1, errorOcurred2, errorOcurred3, errorOcurred4;
		errorOcurred1 = sd.readArrayFromFile(file, LIMIT_ARRAY_DELIVERERS).getOcurred();
		errorOcurred2 = sf.readArrayFromFile(file, LIMIT_ARRAY_FACTORIES).getOcurred();
		errorOcurred3 = sm.readArrayFromFile(file, LIMIT_ARRAY_MAGAZINES).getOcurred();
		errorOcurred4 = ss.readArrayFromFile(file, LIMIT_ARRAY_STORES).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3 || errorOcurred4)
		{
			fclose(file);
			return Exception(true);
		}
	}
	{
		bool errorOcurred1, errorOcurred2, errorOcurred3;
		errorOcurred1 = cd.readMatrixFromFile(file, COST_MATRIX_DELIVERERS).getOcurred();
		errorOcurred2 = cf.readMatrixFromFile(file, COST_MATRIX_FACTORIES).getOcurred();
		errorOcurred3 = cm.readMatrixFromFile(file, COST_MATRIX_MAGAZINES).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3)
		{
			fclose(file);
			return Exception(true);
		}
	}
	{
		bool errorOcurred1, errorOcurred2, errorOcurred3, errorOcurred4;
		errorOcurred1 = ud.readArrayFromFile(file, COST_ARRAY_DELIVERERS).getOcurred();
		errorOcurred2 = uf.readArrayFromFile(file, COST_ARRAY_FACTORIES).getOcurred();
		errorOcurred3 = um.readArrayFromFile(file, COST_ARRAY_MAGAZINES).getOcurred();
		errorOcurred4 = p.readArrayFromFile(file, PRICE_ARRAY_STORE).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3 || errorOcurred4)
		{
			fclose(file);
			return Exception(true);
		}
	}
	{
		bool errorOcurred1, errorOcurred2, errorOcurred3;
		errorOcurred1 = readMatrixBoundsFromFile(file, xdmin, xdmax, MIN_MAX_MATRIXES_DELIVERESR).getOcurred();
		errorOcurred2 = readMatrixBoundsFromFile(file, xfmin, xfmax, MIN_MAX_MATRIXES_FACTORIES).getOcurred();
		errorOcurred3 = readMatrixBoundsFromFile(file, xmmin, xmmax, MIN_MAX_MATRIXES_MAGAZINES).getOcurred();

		if (errorOcurred1 || errorOcurred2 || errorOcurred3)
		{
			fclose(file);
			return Exception(true);
		}
	}

	fclose(file);
	return Exception(false);
}

Exception MscnProblem::getSolutionBounds(int index, double& min, double& max)
{
	int solutionLenght = numberOfDeliverers * numberOfFactories +
		numberOfFactories * numberOfMagazines +
		numberOfMagazines * numberOfStores;
	int xdLength = numberOfDeliverers * numberOfFactories;
	int xfLength = numberOfFactories * numberOfMagazines;

	if (index < 0 || index >= solutionLenght)
	{
		return Exception(true);
	}

	if (index < xdLength)
	{
		min = xdmin[index / xdmin.getSizeX()][index % xdmin.getSizeX()];
		max = xdmax[index / xdmin.getSizeX()][index % xdmin.getSizeX()];
		return Exception(false);
	}
	if (index < xdLength + xfLength)
	{
		int correctedIndex = index - xdLength;
		min = xfmin[correctedIndex / xfmin.getSizeX()][correctedIndex % xfmin.getSizeX()];
		max = xfmax[correctedIndex / xfmin.getSizeX()][correctedIndex % xfmin.getSizeX()];
		return Exception(false);
	}
	int correctedIndex = index - (xdLength + xfLength);
	min = xmmin[correctedIndex / xmmin.getSizeX()][correctedIndex % xmmin.getSizeX()];
	max = xmmax[correctedIndex / xmmin.getSizeX()][correctedIndex % xmmin.getSizeX()];

	return Exception(false);
}

Exception MscnProblem::setNumberOfDeliverers(int numberOfDeliverers)
{
    if (numberOfDeliverers <= 0)
    {
        return Exception(true);
    }

    this->numberOfDeliverers = numberOfDeliverers;

    cd.setSizeY(numberOfDeliverers);
    xdmin.setSizeY(numberOfDeliverers);
    xdmax.setSizeY(numberOfDeliverers);

    sd.setSize(numberOfDeliverers);
    ud.setSize(numberOfDeliverers);

    return Exception(false);
}

Exception MscnProblem::setNumberOfFactories(int numberOfFactories)
{
    if (numberOfFactories <= 0)
    {
        return Exception(true);
    }

    this->numberOfFactories = numberOfFactories;

    cd.setSizeX(numberOfFactories);
    xdmin.setSizeX(numberOfFactories);
    xdmax.setSizeX(numberOfFactories);

    cf.setSizeY(numberOfFactories);
    xfmin.setSizeY(numberOfFactories);
    xfmax.setSizeY(numberOfFactories);

    uf.setSize(numberOfFactories);
    sf.setSize(numberOfFactories);

    return Exception(false);
}

Exception MscnProblem::setNumberOfMagazines(int numberOfMagazines)
{
    if (numberOfMagazines <= 0)
    {
        return Exception(true);
    }

    this->numberOfMagazines = numberOfMagazines;

    cf.setSizeX(numberOfMagazines);
    xfmin.setSizeX(numberOfMagazines);
    xfmax.setSizeX(numberOfMagazines);

    cm.setSizeY(numberOfMagazines);
    xmmin.setSizeY(numberOfMagazines);
    xmmax.setSizeY(numberOfMagazines);

    um.setSize(numberOfMagazines);
    sm.setSize(numberOfMagazines);

    return Exception(false);
}

Exception MscnProblem::setNumberOfStores(int numberOfStores)
{
    if (numberOfStores <= 0)
    {
        return Exception(true);
    }

    this->numberOfStores = numberOfStores;

    cm.setSizeX(numberOfStores);
    xmmin.setSizeX(numberOfStores);
    xmmax.setSizeX(numberOfStores);

    ss.setSize(numberOfStores);
    p.setSize(numberOfStores);

    return Exception(false);
}

Exception MscnProblem::setValueInCd(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, cd);
}

Exception MscnProblem::setValueInCf(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, cf);
}

Exception MscnProblem::setValueInCm(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, cm);
}

Exception MscnProblem::setValueInXdmin(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xdmin);
}

Exception MscnProblem::setValueInXfmin(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xfmin);
}

Exception MscnProblem::setValueInXmmin(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xmmin);
}

Exception MscnProblem::setValueInXdmax(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xdmax);
}

Exception MscnProblem::setValueInXfmax(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xfmax);
}

Exception MscnProblem::setValueInXmmax(int row, int cloumn, double value)
{
    return setNonNegativeValueMatrix(row, cloumn, value, xmmax);
}

Exception MscnProblem::setValueInSd(int index, double value)
{
    return setNonNegativeValueArray(index, value, sd);
}

Exception MscnProblem::setValueInSf(int index, double value)
{
    return setNonNegativeValueArray(index, value, sf);
}

Exception MscnProblem::setValueInSm(int index, double value)
{
    return setNonNegativeValueArray(index, value, sm);
}

Exception MscnProblem::setValueInSs(int index, double value)
{
    return setNonNegativeValueArray(index, value, ss);
}

Exception MscnProblem::setValueInUd(int index, double value)
{
    return setNonNegativeValueArray(index, value, ud);
}

Exception MscnProblem::setValueInUf(int index, double value)
{
    return setNonNegativeValueArray(index, value, uf);
}

Exception MscnProblem::setValueInUm(int index, double value)
{
    return setNonNegativeValueArray(index, value, um);
}

Exception MscnProblem::setValueInP(int index, double value)
{
    return setNonNegativeValueArray(index, value, p);
}

Exception MscnProblem::setNonNegativeValueMatrix(int row, int column, double value, Matrix& matrix)
{
    if (value < 0 || row < 0 || column < 0 || row >= matrix.getSizeY() || column >= matrix.getSizeX())
    {
        return Exception(true);
    }
    
    matrix[row][column] = value;
    return Exception(false);
}

Exception MscnProblem::setNonNegativeValueArray(int index, double value, Array& array)
{
    if (value < 0 || index < 0 || index >= array.getSize())
    {
        return Exception(true);
    }

    array[index] = value;
    return Exception(false);
}

Exception MscnProblem::solutionToMatrices(double* solution, Matrix& xd, Matrix& xf, Matrix& xm)
{
    if (solution == NULL)
    {
        return Exception(true);
    }

    int offset = 0;
    Exception exception;
    Matrix xdTemp(numberOfDeliverers, numberOfFactories, solution, exception);
    offset += numberOfDeliverers * numberOfFactories;
    Matrix xfTemp(numberOfFactories, numberOfMagazines, solution + offset, exception);
    offset += numberOfFactories * numberOfMagazines;
    Matrix xmTemp(numberOfMagazines, numberOfStores, solution + offset, exception);

    xd = xdTemp;
    xf = xfTemp;
    xm = xmTemp;

    if (exception.getOcurred())
    {
        return exception;
    }
	return Exception(false);
}

Exception MscnProblem::solutionToMatricesAndCheckNonNative(double* solution, Matrix& xd, Matrix& xf, Matrix& xm)
{
    Exception e = solutionToMatrices(solution, xd, xf, xm);

    if (e.getOcurred())
    {
        return e;
    }

    if (!xd.allNumbersNonNegative() || !xf.allNumbersNonNegative() || !xm.allNumbersNonNegative())
    {
        return Exception(true);
    }
	return Exception(false);
}



Exception MscnProblem::writeMatrixBoundsToFile(FILE* file, Matrix& min, Matrix& max, std::string header)
{
    if (file == NULL || min.getSizeX() != max.getSizeX() || min.getSizeY() != min.getSizeY())
    {
        return Exception(true);
    }

    if (fprintf(file, (header + "\n").c_str()) <= 0)
    {
        return Exception(true);
    }

    for (int i = 0; i < min.getSizeY(); i++)
    {
        for (int j = 0; j < min.getSizeX(); j++)
        {
            if (fprintf(file, ("%lf" + SEPARATOR + "%lf" +SEPARATOR).c_str(), min[i][j], max[i][j]) <= 0)
            {
                return Exception(true);
            }
        }
    }

    if (fprintf(file, "\n") <= 0)
    {
        return Exception(true);
    }
    return Exception(false);
}

Exception MscnProblem::readMatrixBoundsFromFile(FILE* file, Matrix& min, Matrix& max, std::string header)
{
	if (file == NULL || min.getSizeX() != max.getSizeX() || min.getSizeY() != max.getSizeY())
	{
		return Exception(true);
	}

	if (!util::isTagEqual(file, header))
	{
		return Exception(true);
	}

	for (int i = 0; i < min.getSizeY(); i++)
	{
		for (int j = 0; j < min.getSizeX(); j++)
		{
			double number1, number2;
            char* c1;
            char* c2;
			if (fscanf(file, (" %lf%[" + SEPARATOR + "]%lf%[" + SEPARATOR+ "]").c_str(), &number1, &c1, &number2, &c2) < 4)
			{
				return Exception(true);
			}
			min[i][j] = number1;
			max[i][j] = number2;
		}
	}
	return Exception(false);
}

double MscnProblem::getIncome(Matrix& xm)
{
    double income = 0;
    for (int i = 0; i < numberOfMagazines; i++)
    {
        for (int j = 0; j < numberOfStores; j++)
        {
            income += p[j] * xm[i][j];
        }
    }
    return income;
}

double MscnProblem::getCost(Matrix& xd, Matrix& xf, Matrix& xm)
{
    double cost = 0;

    cost += xd * cd;
    cost += xf * cf;
    cost += xm * cm;

    return cost;
}

double MscnProblem::getFixedCost(Matrix& xd, Matrix& xf, Matrix& xm)
{
    double fixedCost = 0;

    addFacilityFixedCost(xd, ud, fixedCost);
    addFacilityFixedCost(xf, uf, fixedCost);
    addFacilityFixedCost(xm, um, fixedCost);

    return fixedCost;
}

Exception MscnProblem::addFacilityFixedCost(Matrix& matrix, Array& costArray, double& fixedCost)
{
    if (matrix.getSizeY() != costArray.getSize())
    {
        return Exception(true);
    }

    for (int i = 0; i < matrix.getSizeY(); i++)
    {
        double sum = 0;
        matrix.getRowSum(i, sum);
        if (sum > 0)
        {
            fixedCost += costArray[i];
        }
    }
    return Exception(false);
}
