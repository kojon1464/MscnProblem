#pragma once

#include "Matrix.h"
#include <string>
class Solution
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

    Solution();
    ~Solution();

    Exception readFormFile(std::string path);

    Exception toArray(double*& array);

    Exception setNumberOfDeliverers(int numberOfDeliverers);
    Exception setNumberOfFactories(int numberOfFactories);
    Exception setNumberOfMagazines(int numberOfMagazines);
    Exception setNumberOfStores(int numberOfStores);
private:
    int numberOfDeliverers;
    int numberOfFactories;
    int numberOfMagazines;
    int numberOfStores;

    Matrix xd;
    Matrix xf;
    Matrix xm;
};

