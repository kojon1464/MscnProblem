#pragma once

#include "Matrix.h"
#include "Array.h"
#include <string>

class MscnProblem
{
public:
    static const std::string SEPARTOR;
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

    MscnProblem();
    ~MscnProblem();

    Exception getQuality(double* solution, double& result);
    Exception constraintsSatified(double* solution, bool& result);

    Exception writeToFile(std::string path);
	Exception readFormFile(std::string path);

	Exception getSolutionBounds(int index, double& min, double& max);

    Exception setNumberOfDeliverers(int numberOfDeliverers);
    Exception setNumberOfFactories(int numberOfFactories);
    Exception setNumberOfMagazines(int numberOfMagazines);
    Exception setNumberOfStores(int numberOfStores);

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

    Exception setNonNegativeValueMatrix(int row, int column, double value, Matrix& matrix);
    Exception setNonNegativeValueArray(int index, double value, Array& array);
    Exception solutionToMatrices(double* solution, Matrix& xd, Matrix& xf, Matrix& xm);
    Exception solutionToMatricesAndCheckNonNative(double* solution, Matrix& xd, Matrix& xf, Matrix& xm);

    Exception writeMatrixBoundsToFile(FILE* file, Matrix& min, Matrix& max, std::string header);
	Exception readMatrixBoundsFromFile(FILE* file, Matrix& min, Matrix& max, std::string header);

	bool isTagEqual(FILE* file, std::string tag);

    double getIncome(Matrix& xm);
    double getCost(Matrix& xd, Matrix& xf, Matrix& xm);
    double getFixedCost(Matrix& xd, Matrix& xf, Matrix& xm);
};

