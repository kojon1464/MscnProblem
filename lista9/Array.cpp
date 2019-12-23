#include "pch.h"
#include "Array.h"
#include "util.h"
#include <algorithm>

const std::string Array::SEPARATOR = ";";
const int Array::DEFAULT_SIZE = 10;

Array::Array()
{
    size = DEFAULT_SIZE;
    create();
}

Array::Array(int size, Exception exception)
{
    if (size <= 0)
    {
        exception.setOcurred(true);
        size = DEFAULT_SIZE;
    }
    else
    {
        this->size = size;
    }

    create();
}

Array::Array(const Array& other)
{
    copy(other);
}

Array::Array(Array&& other)
{
    move(other);
}

Array::~Array()
{
    remove();
}

Array& Array::operator=(Array& other)
{
    if (this == &other)
    {
        return *this;
    }
    remove();
    copy(other);
    return *this;
}

Array& Array::operator=(Array&& other)
{
    if (this == &other)
    {
        return *this;
    }

    remove();
    move(other);
    return *this;
}

double& Array::operator[](int i)
{
    if (i < 0 || i >= size)
    {
        throw new Exception(true);
    }
    return array[i];
}

Exception Array::writeArrayToFile(FILE * file, std::string header)
{
    if (file == NULL)
    {
        return Exception(true);
    }

    if (fprintf(file, (header + "\n").c_str()) <= 0)
    {
        return Exception(true);
    }

    for (int i = 0; i < getSize(); i++)
    {
        if (fprintf(file, ("%lf" + SEPARATOR).c_str(), array[i]) <= 0)
        {
            return Exception(true);
        }
    }

    if (fprintf(file, "\n") <= 0)
    {
        return Exception(true);
    }

    return Exception(false);
}

Exception Array::readArrayFromFile(FILE * file, std::string header)
{
    if (file == NULL)
    {
        return Exception(true);
    }

    if (!util::isTagEqual(file, header))
    {
        return Exception(true);
    }

    for (int i = 0; i < getSize(); i++)
    {
        double number;
        char* c;
        if (fscanf(file, (" %lf%[" + SEPARATOR + "]").c_str(), &number, &c) < 2)
        {
            return Exception(true);
        }
        array[i] = number;
    }
    return Exception(false);
}

int Array::getSize()
{
    return size;
}

Exception Array::setSize(int size)
{
    if (size <= 0)
    {
        return Exception(true);
    }
    else if (size != this->size)
    {
        int copyLenght = std::min(size, this->size);
        double* newArray = new double[size];
        std::copy(array, array + copyLenght, newArray);
        delete[] array;
        array = newArray;
        this->size = size;
    }
    return Exception(false);
}

void Array::create()
{
    array = new double[size];
}

void Array::copy(const Array& other)
{
    size = other.size;
    create();
    std::copy(other.array, other.array + other.size, array);
}

void Array::move(Array& other)
{
    size = other.size;
    array = other.array;

    other.array = NULL;
}

void Array::remove()
{
    delete[] array;
}
