#include "pch.h"
#include "Random.h"


Random::Random()
{
    std::random_device rd;
    gen = std::mt19937(rd());
}

Random::Random(long unsigned int seed)
{
    gen = std::mt19937(seed);
}


Random::~Random()
{
}

int Random::getInt(int min, int max)
{
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

double Random::getDouble(double min, double max)
{
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}
