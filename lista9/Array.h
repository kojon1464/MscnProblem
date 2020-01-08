#pragma once

#include "Exception.h"
#include "Random.h"
#include <string>

class Array
{
public:
    static const std::string SEPARATOR;
    static const int DEFAULT_SIZE;

    Array();
    Array(int size, Exception exception);
    Array(const Array& other);
    Array(Array&& other);
    ~Array();

    Array& operator=(Array& other);
    Array& operator=(Array&& other);
    double& operator[](int i);

    Exception writeArrayToFile(FILE* file, std::string header);
    Exception readArrayFromFile(FILE* file, std::string header);

    void fillRandomly(Random& random, double min, double max);

    int getSize();
    double* getInternalArray();
    Exception setSize(int size);
private:
    double* array;
    int size;

    void create();
    void copy(const Array& other);
    void move(Array& other);
    void remove();
};

