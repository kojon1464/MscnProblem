#include "pch.h"
#include "Util.h"

bool util::isTagEqual(FILE* file, std::string tag)
{
    if (file == NULL)
    {
        return false;
    }

    fscanf(file, " ");
    char* c = new char[tag.length() + 1];
    fgets(c, tag.length() + 1, file);

    if (strcmp(c, tag.c_str()) == 0)
    {
        delete[] c;
        return true;
    }

    delete[] c;
    return false;
}