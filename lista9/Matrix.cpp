#include "pch.h"
#include "Matrix.h"
#include "Util.h"
#include <algorithm>

const std::string Matrix::SEPARATOR = ";";
const int Matrix::DEFAULT_SIZE = 4;

Matrix::Matrix()
{
    sizeX = DEFAULT_SIZE;
    sizeY = DEFAULT_SIZE;
    create();
}

Matrix::Matrix(int sizeY, int sizeX, Exception& exception)
{
    if (sizeX <= 0 || sizeY <= 0)
    {
        exception.setOcurred(true);
        this->sizeX = DEFAULT_SIZE;
        this->sizeY = DEFAULT_SIZE;
    }
    else
    {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
    }

    create();
}

Matrix::Matrix(int sizeY, int sizeX, double* array, Exception& exception)
{
    if (array == NULL)
    {
        exception.setOcurred(true);
        this->sizeX = DEFAULT_SIZE;
        this->sizeY = DEFAULT_SIZE;
        create();
    }
    else
    {
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        create();

        int counter = 0;
        for (int i = 0; i < sizeY; i++)
        {
            for (int j = 0; j < sizeX; j++)
            {
                matrix[i][j] = array[counter++];
            }
        }
    }

}

Matrix::Matrix(const Matrix& other)
{
    copy(other);
}

Matrix::Matrix(Matrix&& other)
{
    move(other);
}

Matrix::~Matrix()
{
    remove();
}

Matrix& Matrix::operator=(const Matrix& other)
{
    if (this == &other)
    {
        return *this;
    }

    remove();
    copy(other);
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other)
{
    if (this == &other)
    {
        return *this;
    }

    remove();
    move(other);
    return *this;
}

double* Matrix::operator[](int i)
{
    if (i < 0 || i >= sizeY)
    {
        return NULL;
    }
    return matrix[i];
}

double Matrix::operator*(const Matrix& other)
{
    if (sizeY != other.sizeY || sizeX != other.sizeX)
    {
        throw Exception(true);
    }

    double sum = 0;
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            sum += other.matrix[i][j] * matrix[i][j];
        }
    }
    return sum;
}

Exception Matrix::getRowSum(int index, double& result)
{
    if (index < 0 || index >= sizeY)
    {
        return Exception(true);
    }

    double sum = 0;
    for (int i = 0; i < sizeX; i++)
    {
        sum += matrix[index][i];
    }
    result = sum;
    return Exception(false);
}

Exception Matrix::getColumnSum(int index, double & result)
{
    if (index < 0 || index >= sizeX)
    {
        return Exception(true);
    }

    double sum = 0;
    for (int i = 0; i < sizeY; i++)
    {
        sum += matrix[i][index];
    }
    result = sum;
    return Exception(false);
}

bool Matrix::allNumbersNonNegative()
{
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (matrix[i][j] < 0)
            {
                return false;
            }
        }
    }
    return true;
}

Exception Matrix::isInBounds(Matrix& min, Matrix& max, bool& result)
{
    if (sizeX != min.sizeX || sizeX != max.sizeX ||
        sizeY != min.sizeY || sizeY != max.sizeY)
    {
        return Exception(true);
    }

    result = true;
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (matrix[i][j] < min[i][j] || matrix[i][j] > max[i][j])
            {
                result = false;
            }
        }
    }
    return Exception(false);
}

Exception Matrix::writeMatrixToFile(FILE * file, std::string header)
{
    if (file == NULL)
    {
        return Exception(true);
    }

    if (fprintf(file, (header + "\n").c_str()) <= 0)
    {
        return Exception(true);
    }

    for (int i = 0; i < getSizeY(); i++)
    {
        for (int j = 0; j < getSizeX(); j++)
        {
            if (fprintf(file, ("%lf" + SEPARATOR).c_str(), matrix[i][j]) <= 0)
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

Exception Matrix::readMatrixFromFile(FILE * file, std::string header)
{
    if (file == NULL)
    {
        return Exception(true);
    }

    if (!util::isTagEqual(file, header))
    {
        return Exception(true);
    }

    for (int i = 0; i < getSizeY(); i++)
    {
        for (int j = 0; j < getSizeX(); j++)
        {
            double number;
            char* c;
            if (fscanf(file, (" %lf%[" + SEPARATOR + "]").c_str(), &number, &c) < 2)
            {
                return Exception(true);
            }
            matrix[i][j] = number;
        }
    }
    return Exception(false);
}

int Matrix::getSizeX()
{
    return sizeX;
}

int Matrix::getSizeY()
{
    return sizeY;
}

Exception Matrix::setSizeX(int sizeX)
{
    if (sizeX <= 0)
    {
        return Exception(true);
    }
    else if (sizeX != this->sizeX)
    {
        for (int i = 0; i < sizeY; i++)
        {
            int copyLenght = std::min(sizeX, this->sizeX);
            double* newRow = new double[sizeX];
            std::copy(matrix[i], matrix[i] + copyLenght, newRow);
            delete[] matrix[i];
            matrix[i] = newRow;
            this->sizeX = sizeX;
        }
    }
    return Exception(false);
}

Exception Matrix::setSizeY(int sizeY)
{
    if (sizeY <= 0)
    {
        return Exception(true);
    }
    else if (sizeY != this->sizeY)
    {
        int copyLenght = std::min(this->sizeY, sizeY);
        double** newMatrix = new double*[sizeY];
        std::copy(matrix, matrix + copyLenght, newMatrix);
		for (int i = this->sizeY; i < sizeY; i++)
		{
			newMatrix[i] = new double[sizeX];
		}
        for (int i = sizeY; i < this->sizeY; i++)
        {
            delete[] matrix[i];
		}
        delete[] matrix;
        matrix = newMatrix;
		this->sizeY = sizeY;
    }
	return Exception(false);
}

void Matrix::create()
{
    matrix = new double*[sizeY];
    for (int i = 0; i < sizeY; i++)
    {
        matrix[i] = new double[sizeX];
    }
}

void Matrix::copy(const Matrix& other)
{
    sizeX = other.sizeX;
    sizeY = other.sizeY;

    create();
    for (int i = 0; i < sizeY; i++)
    {
        std::copy(other.matrix[i], other.matrix[i] + other.sizeX, matrix[i]);
    }
}

void Matrix::move(Matrix& other)
{
    sizeX = other.sizeX;
    sizeY = other.sizeY;
    matrix = other.matrix;

    other.matrix = NULL;
}

void Matrix::remove()
{
    if (matrix != NULL)
    {
        for (int i = 0; i < sizeY; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
    }
}
