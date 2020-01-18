#pragma once

#include <windows.h>

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    double getTime();
private:
    LARGE_INTEGER startTime;
    LARGE_INTEGER frequency;

    bool started;
};

