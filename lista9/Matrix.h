#pragma once

#include "Exception.h"
#include <string>

class Matrix
{
public:
    static const std::string SEPARATOR;
    static const int DEFAULT_SIZE;

    Matrix();
    Matrix(int sizeY, int sizeX, Exception& exception);
    Matrix(int sizeY, int sizeX, double* array, Exception& exception);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other);
    ~Matrix();

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other);
    double* operator[](int index);

    Exception getRowSum(int index, double& result);
    Exception getColumnSum(int index, double& result);

    bool allNumbersNonNegative();
    Exception isInBounds(Matrix& min, Matrix& max, bool& result);

    Exception writeMatrixToFile(FILE* file, std::string header);
    Exception readMatrixFromFile(FILE* file, std::string header);

    int getSizeX();
    int getSizeY();
    Exception setSizeX(int sizeX);
    Exception setSizeY(int sizeY);
private:
    double** matrix;
    int sizeX;
    int sizeY;

    void create();
    void copy(const Matrix& other);
    void move(Matrix& other);
    void remove();

    bool isTagEqual(FILE* file, std::string tag);
};
