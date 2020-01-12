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

Exception util::generateRandomSolution(MscnProblem& problem, Solution& solution)
{
	if (!problem.sameSize(solution))
	{
		return Exception(true);
	}

    Random random;
    double min, max;
    Exception exception;
    Array tempSolution(solution.getSolutionLenght(), exception);

    for (int i = 0; i < solution.getSolutionLenght(); i++)
    {
        problem.getSolutionBounds(i, min, max);
        tempSolution[i] = random.getDouble(min, max);
    }

    solution.loadFromArray(tempSolution);
	return Exception(false);
}
