#include "pch.h"
#include "Exception.h"


Exception::Exception()
{
    ocurred = false;
}

Exception::Exception(bool ocurred)
{
    this->ocurred = ocurred;
}


Exception::~Exception()
{
}

bool Exception::operator==(Exception & other)
{
    return ocurred == other.ocurred;
}

bool Exception::getOcurred()
{
    return ocurred;
}

void Exception::setOcurred(bool ocurred)
{
    this->ocurred = ocurred;
}