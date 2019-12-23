#pragma once
class Exception
{
public:
    Exception();
    Exception(bool occured);
    ~Exception();

    bool operator==(Exception& other);

    bool getOcurred();
    void setOcurred(bool ocurred);
private:
    bool ocurred;
};
