#include "pch.h"
#include "Solution.h"

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
    setNumberOfDeliverers(DEFAULT_FACILITIES_NUMBER);
    setNumberOfFactories(DEFAULT_FACILITIES_NUMBER);
    setNumberOfMagazines(DEFAULT_FACILITIES_NUMBER);
    setNumberOfStores(DEFAULT_FACILITIES_NUMBER);
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

Exception Solution::toArray(double *& array)
{
    int solutionLenght = numberOfDeliverers * numberOfFactories +
        numberOfFactories * numberOfMagazines +
        numberOfMagazines * numberOfStores;
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