#pragma once

#include "Matrix.h"
#include "Array.h"
#include "Solution.h"
#include <string>

class MscnProblem
{
public:
    static const std::string SEPARATOR;
    static const int DEFAULT_FACILITIES_NUMBER;

    static const std::string DELIVERERES;
    static const std::string FACTORIES;
    static const std::string MAGAZINES;
    static const std::string STORES;

    static const std::string LIMIT_ARRAY_DELIVERERS;
    static const std::string LIMIT_ARRAY_FACTORIES;
    static const std::string LIMIT_ARRAY_MAGAZINES;
    static const std::string LIMIT_ARRAY_STORES;

    static const std::string COST_MATRIX_DELIVERERS;
    static const std::string COST_MATRIX_FACTORIES;
    static const std::string COST_MATRIX_MAGAZINES;

    static const std::string COST_ARRAY_DELIVERERS;
    static const std::string COST_ARRAY_FACTORIES;
    static const std::string COST_ARRAY_MAGAZINES;
    static const std::string PRICE_ARRAY_STORE;

    static const std::string MIN_MAX_MATRIXES_DELIVERESR;
    static const std::string MIN_MAX_MATRIXES_FACTORIES;
    static const std::string MIN_MAX_MATRIXES_MAGAZINES;

    static const double COST_MIN_CONSTRAINT;
    static const double COST_MAX_CONSTRAINT;
    static const double FACILITY_COST_MIN_CONSTRAINT;
    static const double FACILITY_COST_MAX_CONSTRAINT;
    static const double PROFIT_MIN_CONSTRAINT;
    static const double PROFIT_MAX_CONSTRAINT;
    static const double FACILITY_LIMIT_MIN_CONSTRAINT;
    static const double FACILITY_LIMIT_MAX_CONSTRAINT;
    static const double MIN_BOUNDARY_MIN_CONSTRAINT;
    static const double MIN_BOUNDARY_MAX_CONSTRAINT;
    static const double MAX_BOUNDARY_MIN_CONSTRAINT;
    static const double MAX_BOUNDARY_MAX_CONSTRAINT;

    MscnProblem();
    ~MscnProblem();

	bool sameSize(Solution& solution);

    Exception getQuality(double* solution, double& result);
    Exception getQuality(Solution& solution, double& result);
    Exception constraintsSatified(double* solution, bool& result);
    Exception constraintsSatified(Solution& solution, bool& result);

    Exception writeToFile(std::string path);
	Exception readFormFile(std::string path);

    void generateInstance(long unsigned int seed);

	Exception getSolutionBounds(int index, double& min, double& max);
    int getRequiredSolutionLenght();

    Exception setNumberOfDeliverers(int numberOfDeliverers);
    Exception setNumberOfFactories(int numberOfFactories);
    Exception setNumberOfMagazines(int numberOfMagazines);
    Exception setNumberOfStores(int numberOfStores);

    int getNumberOfDeliverers();
    int getNumberOfFactories();
    int getNumberOfMagazines();
    int getNumberOfStores();

    Exception setValueInCd(int row, int cloumn, double value);
    Exception setValueInCf(int row, int cloumn, double value);
    Exception setValueInCm(int row, int cloumn, double value);

    Exception setValueInXdmin(int row, int cloumn, double value);
    Exception setValueInXfmin(int row, int cloumn, double value);
    Exception setValueInXmmin(int row, int cloumn, double value);

    Exception setValueInXdmax(int row, int cloumn, double value);
    Exception setValueInXfmax(int row, int cloumn, double value);
    Exception setValueInXmmax(int row, int cloumn, double value);

    Exception setValueInSd(int index, double value);
    Exception setValueInSf(int index, double value);
    Exception setValueInSm(int index, double value);
    Exception setValueInSs(int index, double value);

    Exception setValueInUd(int index, double value);
    Exception setValueInUf(int index, double value);
    Exception setValueInUm(int index, double value);
    Exception setValueInP(int index, double value);
private:
    int numberOfDeliverers;
    int numberOfFactories;
    int numberOfMagazines;
    int numberOfStores;

    Matrix cd;
    Matrix cf;
    Matrix cm;

    Matrix xdmin;
    Matrix xfmin;
    Matrix xmmin;

    Matrix xdmax;
    Matrix xfmax;
    Matrix xmmax;

    Array sd;
    Array sf;
    Array sm;
    Array ss;

    Array ud;
    Array uf;
    Array um;
    Array p;

    Exception getQuality(Matrix& xd, Matrix& xf, Matrix& xm, double& result);
    Exception constraintsSatified(Matrix& xd, Matrix& xf, Matrix& xm, bool& result);

    Exception setNonNegativeValueMatrix(int row, int column, double value, Matrix& matrix);
    Exception setNonNegativeValueArray(int index, double value, Array& array);
    Exception solutionToMatrices(double* solution, Matrix& xd, Matrix& xf, Matrix& xm);
    Exception checkNonNativeNumbersInMatrices(Matrix& xd, Matrix& xf, Matrix& xm);

    Exception writeMatrixBoundsToFile(FILE* file, Matrix& min, Matrix& max, std::string header);
	Exception readMatrixBoundsFromFile(FILE* file, Matrix& min, Matrix& max, std::string header);

    double getIncome(Matrix& xm);
    double getCost(Matrix& xd, Matrix& xf, Matrix& xm);
    double getFixedCost(Matrix& xd, Matrix& xf, Matrix& xm);

    bool checkLimitForFacility(int facilitiesNumber, Matrix& facilityMatrix, Array& facilityLimitAray);

    Exception addFacilityFixedCost(Matrix& matrix, Array& costArray, double& fixedCost);
};

