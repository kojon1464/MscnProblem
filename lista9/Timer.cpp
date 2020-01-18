#include "pch.h"
#include "Timer.h"


Timer::Timer()
{
    QueryPerformanceFrequency(&frequency);
    started = false;
}

Timer::~Timer()
{
}

void Timer::start()
{
    started = true;
    QueryPerformanceCounter(&startTime);
}

double Timer::getTime()
{
    if (!started)
    {
        return 0;
    }
    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);
    return (end.QuadPart - startTime.QuadPart) / frequency.QuadPart;
}


