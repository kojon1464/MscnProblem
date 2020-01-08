#pragma once

#include <random>

class Random
{
public:
    Random();
    Random(long unsigned int seed);
    ~Random();

    int getInt(int min, int max);
    double getDouble(double min, double max);
private:
    std::mt19937 gen;
};

