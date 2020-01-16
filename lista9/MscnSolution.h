#pragma once

#include "Matrix.h"
#include "Array.h"
#include "Solution.h"
#include <string>

class MscnSolution : public Solution
{
public:
    static const int DEFAULT_FACILITIES_NUMBER;

    static const std::string DELIVERERES;
    static const std::string FACTORIES;
    static const std::string MAGAZINES;
    static const std::string STORES;

    static const std::string MATRIX_DELIVERERS;
    static const std::string MATRIX_FACTORIES;
    static const std::string MATRIX_MAGAZINES;

    MscnSolution();
    explicit MscnSolution(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores, Exception& exception);
    ~MscnSolution();

    Exception readFormFile(std::string path);
    Exception writeToFile(std::string path);

    Exception loadFromArray(Array& array);
    Exception toArray(double*& array);

    int getSolutionLenght();
	Exception setSize(int numberOfDeliverers, int numberOfFactories, int numberOfMagazines, int numberOfStores);

    Exception setNumberOfDeliverers(int numberOfDeliverers);
    Exception setNumberOfFactories(int numberOfFactories);
    Exception setNumberOfMagazines(int numberOfMagazines);
    Exception setNumberOfStores(int numberOfStores);

	Exception setValue(int index, double value);
	Exception getValue(int index, double& value);

	Solution* clone();

	int getNumberOfDeliverers();
	int getNumberOfFactories();
	int getNumberOfMagazines();
	int getNumberOfStores();

    Matrix& getDeliverersMatrix();
    Matrix& getFactoriesMatrix();
    Matrix& getMagazinesMatrix();
private:
    int numberOfDeliverers;
    int numberOfFactories;
    int numberOfMagazines;
    int numberOfStores;

    Matrix xd;
    Matrix xf;
    Matrix xm;
};

