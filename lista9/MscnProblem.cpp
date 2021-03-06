#include "pch.h"
#include "MscnProblem.h"
#include "Util.h"
#include "MscnSolution.h"
#include <iostream>

const std::string MscnProblem::SEPARATOR = ";";
const int MscnProblem::DEFAULT_FACILITIES_NUMBER = 3;

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

const double MscnProblem::COST_MIN_CONSTRAINT = 1;
const double MscnProblem::COST_MAX_CONSTRAINT = 10;
const double MscnProblem::FACILITY_COST_MIN_CONSTRAINT = 50;
const double MscnProblem::FACILITY_COST_MAX_CONSTRAINT = 200;
const double MscnProblem::PROFIT_MIN_CONSTRAINT = 2;
const double MscnProblem::PROFIT_MAX_CONSTRAINT = 100;
const double MscnProblem::FACILITY_LIMIT_MIN_CONSTRAINT = 100;
const double MscnProblem::FACILITY_LIMIT_MAX_CONSTRAINT = 7000;
const double MscnProblem::MIN_BOUNDARY_MIN_CONSTRAINT = 0; // Note that if you want to be sure that getQualityAndFix gives you
const double MscnProblem::MIN_BOUNDARY_MAX_CONSTRAINT = 0; // valid solution at 100% chance min boundary has to be 0
const double MscnProblem::MAX_BOUNDARY_MIN_CONSTRAINT = 180;
const double MscnProblem::MAX_BOUNDARY_MAX_CONSTRAINT = 220;

MscnProblem::MscnProblem()
{
    qualityInvokeCounter = 0;
	setSize(DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER, DEFAULT_FACILITIES_NUMBER);
}

MscnProblem::MscnProblem(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores, Exception& exception)
{
	exception = setSize(numberOfDeliverers, numberOfFactories, numberOfMagazines, numberOfStores);
}

MscnProblem::~MscnProblem()
{
}

bool MscnProblem::sameSize(Solution& solution)
{
	try 
	{
		MscnSolution& mscnSolution = dynamic_cast<MscnSolution&>(solution);
		return mscnSolution.getNumberOfDeliverers() == numberOfDeliverers &&
			mscnSolution.getNumberOfFactories() == numberOfFactories &&
			mscnSolution.getNumberOfMagazines() == numberOfMagazines &&
			mscnSolution.getNumberOfStores() == numberOfStores;
	}
	catch(const std::bad_cast& e)
	{
		return solution.getSolutionLenght() == getRequiredSolutionLenght();
	}
}

Exception MscnProblem::getQuality(double* solution, double& result)
{
    Matrix xd, xf, xm;

    if (solutionToMatrices(solution, xd, xf, xm).getOcurred())
    {
        return Exception(true);
    }
    return getQuality(xd, xf, xm, result);
}

Exception MscnProblem::getQuality(Solution& solution, double& result)
{
	if (!sameSize(solution))
	{
		return Exception(true);
	}
	try
	{
		MscnSolution& mscnSolution = dynamic_cast<MscnSolution&>(solution);
		return getQuality(mscnSolution.getDeliverersMatrix(), mscnSolution.getFactoriesMatrix(), mscnSolution.getMagazinesMatrix(), result);
	}
	catch (const std::bad_cast& e)
	{
		double* array;
		solution.toArray(array);
		return getQuality(array, result);
	}
}

Exception MscnProblem::getQualityAndFix(Solution& solution, double& result)
{
    if (!sameSize(solution))
    {
        return Exception(true);
    }
    try
    {
        MscnSolution& mscnSolution = dynamic_cast<MscnSolution&>(solution);
        fixSolution(mscnSolution.getDeliverersMatrix(), mscnSolution.getFactoriesMatrix(), mscnSolution.getMagazinesMatrix());
        return getQuality(mscnSolution.getDeliverersMatrix(), mscnSolution.getFactoriesMatrix(), mscnSolution.getMagazinesMatrix(), result);
    }
    catch (const std::bad_cast& e)
    {
        double* array;
        solution.toArray(array);
        Matrix xd, xf, xm;
        solutionToMatrices(array, xd, xf, xm);
        fixSolution(xd, xf, xm);
        Exception exception = getQuality(xd, xf, xm, result); //todo: zapisac wynik z macierzy do solution
        return exception;
    }
}

Exception MscnProblem::constraintsSatified(double* solution, bool& result)
{
    Matrix xd, xf, xm;
    if (solutionToMatrices(solution, xd, xf, xm).getOcurred())
    {
        result = false;
        return Exception(true);
    }
    return constraintsSatified(xd, xf, xm, result);
}

Exception MscnProblem::constraintsSatified(Solution& solution, bool& result)
{
	if (!sameSize(solution))
	{
		return Exception(true);
	}
	try
	{
		MscnSolution& mscnSolution = dynamic_cast<MscnSolution&>(solution);
		return constraintsSatified(mscnSolution.getDeliverersMatrix(), mscnSolution.getFactoriesMatrix(), mscnSolution.getMagazinesMatrix(), result);
	}
	catch (const std::bad_cast& e)
	{
		double* array;
		solution.toArray(array);
		return constraintsSatified(array, result);
	}
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

void MscnProblem::generateInstance(long unsigned int seed)
{
    Random random(seed);

    cd.fillRandomly(random, COST_MIN_CONSTRAINT, COST_MAX_CONSTRAINT);
    cf.fillRandomly(random, COST_MIN_CONSTRAINT, COST_MAX_CONSTRAINT);
    cm.fillRandomly(random, COST_MIN_CONSTRAINT, COST_MAX_CONSTRAINT);

    xdmin.fillRandomly(random, MIN_BOUNDARY_MIN_CONSTRAINT, MIN_BOUNDARY_MAX_CONSTRAINT);
    xfmin.fillRandomly(random, MIN_BOUNDARY_MIN_CONSTRAINT, MIN_BOUNDARY_MAX_CONSTRAINT);
    xmmin.fillRandomly(random, MIN_BOUNDARY_MIN_CONSTRAINT, MIN_BOUNDARY_MAX_CONSTRAINT);

    xdmax.fillRandomly(random, MAX_BOUNDARY_MIN_CONSTRAINT, MAX_BOUNDARY_MAX_CONSTRAINT);
    xfmax.fillRandomly(random, MAX_BOUNDARY_MIN_CONSTRAINT, MAX_BOUNDARY_MAX_CONSTRAINT);
    xmmax.fillRandomly(random, MAX_BOUNDARY_MIN_CONSTRAINT, MAX_BOUNDARY_MAX_CONSTRAINT);

    sd.fillRandomly(random, FACILITY_LIMIT_MIN_CONSTRAINT, FACILITY_LIMIT_MAX_CONSTRAINT);
    sf.fillRandomly(random, FACILITY_LIMIT_MIN_CONSTRAINT, FACILITY_LIMIT_MAX_CONSTRAINT);
    sm.fillRandomly(random, FACILITY_LIMIT_MIN_CONSTRAINT, FACILITY_LIMIT_MAX_CONSTRAINT);
    ss.fillRandomly(random, FACILITY_LIMIT_MIN_CONSTRAINT, FACILITY_LIMIT_MAX_CONSTRAINT);

    ud.fillRandomly(random, FACILITY_COST_MIN_CONSTRAINT, FACILITY_COST_MAX_CONSTRAINT);
    uf.fillRandomly(random, FACILITY_COST_MIN_CONSTRAINT, FACILITY_COST_MAX_CONSTRAINT);
    um.fillRandomly(random, FACILITY_COST_MIN_CONSTRAINT, FACILITY_COST_MAX_CONSTRAINT);
    p.fillRandomly(random, PROFIT_MIN_CONSTRAINT, PROFIT_MAX_CONSTRAINT);
}

Exception MscnProblem::getSolutionBounds(int index, double& min, double& max)
{
    int solutionLenght = getRequiredSolutionLenght();
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

int MscnProblem::getRequiredSolutionLenght()
{
    return numberOfDeliverers * numberOfFactories +
        numberOfFactories * numberOfMagazines +
        numberOfMagazines * numberOfStores;
}

int MscnProblem::getQualityInvokeCounter()
{
    return qualityInvokeCounter;
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

Exception MscnProblem::setSize(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores)
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

int MscnProblem::getNumberOfDeliverers()
{
    return numberOfDeliverers;
}

int MscnProblem::getNumberOfFactories()
{
    return numberOfFactories;
}

int MscnProblem::getNumberOfMagazines()
{
    return numberOfMagazines;
}

int MscnProblem::getNumberOfStores()
{
    return numberOfStores;
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

Exception MscnProblem::getQuality(Matrix &xd, Matrix &xf, Matrix& xm, double& result)
{
    qualityInvokeCounter++;
    Exception except = checkNonNativeNumbersInMatrices(xd, xf, xm);

    if (except.getOcurred())
    {
        return except;
    }

    result = getIncome(xm) - getCost(xd, xf, xm) - getFixedCost(xd, xf, xm);
    return Exception(false);
}

Exception MscnProblem::fixSolution(Matrix& xd, Matrix& xf, Matrix& xm)
{
    Exception exception1, exception2, exception3;
    exception1 = clampNonNegativeValueInMatrix(xd, xdmin, xdmax);
    exception2 = clampNonNegativeValueInMatrix(xf, xfmin, xfmax);
    exception3 = clampNonNegativeValueInMatrix(xm, xmmin, xmmax);

    if (exception1.getOcurred() || exception2.getOcurred() || exception3.getOcurred())
    {
        return Exception(true);
    }

    fixLimitForFacility(numberOfDeliverers, xd, sd);
    fixLimitForFacility(numberOfFactories, xf, sf);
    fixLimitForFacility(numberOfMagazines, xm, sm);
    fixLimitForStores(numberOfStores, xm, ss);

    fixFlows(numberOfFactories, xd, xf);
    fixFlows(numberOfMagazines, xf, xm);

    return Exception(false);
}

Exception MscnProblem::constraintsSatified(Matrix& xd, Matrix& xf, Matrix& xm, bool& result)
{
    result = true;

    Exception except = checkNonNativeNumbersInMatrices(xd, xf, xm);

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

    if (!checkLimitForFacility(numberOfDeliverers, xd, sd) ||
        !checkLimitForFacility(numberOfFactories, xf, sf) ||
        !checkLimitForFacility(numberOfMagazines, xm, sm))
    {
        result = false;
    }

    double sum = 0;
    for (int i = 0; i < numberOfStores; i++)
    {
        xm.getColumnSum(i, sum);
        if (sum > ss[i])
        {
            result = false;
        }
    }

    if (!checkFlows(numberOfFactories, xd, xf) ||
        !checkFlows(numberOfMagazines, xf, xm))
    {
        result = false;
    }

    return Exception(false);
}

Exception MscnProblem::clampNonNegativeValueInMatrix(Matrix& matrix, Matrix& min, Matrix& max)
{
    if (matrix.getSizeX() != min.getSizeX() || matrix.getSizeX() != max.getSizeX() ||
        matrix.getSizeY() != min.getSizeY() || matrix.getSizeY() != max.getSizeY())
    {
        return Exception(true);
    }

    const int zero = 0;
    for (int i = 0; i < matrix.getSizeY(); i++)
    {
        for (int j = 0; j < matrix.getSizeX(); j++)
        {
            matrix[i][j] = std::max(0.0, std::max(min[i][j], std::min(max[i][j], matrix[i][j])));
        }
    }

    return Exception(false);
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

Exception MscnProblem::checkNonNativeNumbersInMatrices(Matrix& xd, Matrix& xf, Matrix& xm)
{   
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

bool MscnProblem::checkLimitForFacility(int facilitiesNumber, Matrix& facilityMatrix, Array& facilityLimitAray)
{
    double sum = 0;
    for (int i = 0; i < facilitiesNumber; i++)
    {
        facilityMatrix.getRowSum(i, sum);
        if (sum > facilityLimitAray[i])
        {
            return false;
        }
    }
    return true;
}

void MscnProblem::fixLimitForFacility(int facilitiesNumber, Matrix& facilityMatrix, Array& facilityLimitAray)
{
    double sum = 0;
    for (int i = 0; i < facilitiesNumber; i++)
    {
        facilityMatrix.getRowSum(i, sum);
        if (sum > facilityLimitAray[i])
        {
            double multiplayer = std::max(0.0, std::min((facilityLimitAray[i] - 1)/sum, 1.0));
            for (int j = 0; j < facilityMatrix.getSizeX(); j++)
            {
                facilityMatrix[i][j] *= multiplayer;
            }
        }
    }
}

void MscnProblem::fixLimitForStores(int storesNumber, Matrix& magazineMatrix, Array& storeLimitAray)
{
    double sum = 0;
    for (int i = 0; i < storesNumber; i++)
    {
        magazineMatrix.getColumnSum(i, sum);
        if (sum > ss[i])
        {
            double multiplayer = std::max(0.0, std::min((storeLimitAray[i] - 1) / sum, 1.0));
            for (int j = 0; j < magazineMatrix.getSizeY(); j++)
            {
                magazineMatrix[j][i] *= multiplayer;
            }
        }
    }
}

bool MscnProblem::checkFlows(int facilitiesNumber, Matrix& delivererMatrix, Matrix& facilityMatrix)
{
    double deliveredSum = 0;
    double sendSum = 0;
    for (int i = 0; i < facilitiesNumber; i++)
    {
        delivererMatrix.getColumnSum(i, deliveredSum);
        facilityMatrix.getRowSum(i, sendSum);
        if (deliveredSum < sendSum)
        {
            return false;
        }
    }
    return true;
}

void MscnProblem::fixFlows(int facilitiesNumber, Matrix& delivererMatrix, Matrix& facilityMatrix)
{
    double deliveredSum = 0;
    double sendSum = 0;
    for (int i = 0; i < facilitiesNumber; i++)
    {
        delivererMatrix.getColumnSum(i, deliveredSum);
        facilityMatrix.getRowSum(i, sendSum);
        if (deliveredSum < sendSum)
        {
            double multiplayer = std::max(0.0, std::min((deliveredSum - 1) / sendSum, 1.0));
            for (int j = 0; j < facilityMatrix.getSizeX(); j++)
            {
                facilityMatrix[i][j] *= multiplayer;
            }
        }
    }
}

void MscnProblem::updateSolutionData(Solution& solution, Matrix& xd, Matrix& xf, Matrix& xm)
{
    int index = 0;
    updateSolutionDataHelper(solution, index, xd);
    updateSolutionDataHelper(solution, index, xf);
    updateSolutionDataHelper(solution, index, xm);
}

void MscnProblem::updateSolutionDataHelper(Solution& solution, int& index, Matrix& matrix)
{
    for (int i = 0; i < matrix.getSizeY(); i++)
    {
        for (int j = 0; j < matrix.getSizeX(); j++)
        {
            solution.setValue(index, matrix[i][j]);
            i++;
        }
    }
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
