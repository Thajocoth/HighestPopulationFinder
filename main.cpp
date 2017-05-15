//You can either enter a filename at a prompt, or give filenames as args (compatible with multiple filenames this way).

#include <string>
#include <iostream>
#include <fstream>

//Constants
const int C_FIRST_YEAR = 1900;
const int C_LAST_YEAR = 2000;
const int C_YEAR_DIGITS = 4;

//I initially considered making an object for this instead, but since it's only one array, that ultimately felt like overkill
int g_PopulationChangeArray[C_LAST_YEAR - C_FIRST_YEAR];

//Simple linear search: O(n)
void performFinalSearch()
{
	unsigned int currentPopulation = 0;
	unsigned int highestPopulation = 0;
	int highestYear = 0;
	for (int i = 0; i < (C_LAST_YEAR - C_FIRST_YEAR); i++)
	{
		currentPopulation += g_PopulationChangeArray[i];
		if (currentPopulation > highestPopulation)
		{
			highestPopulation = currentPopulation;
			highestYear = i;
		}
	}
	std::cout << "The highest population was in " << (highestYear + C_FIRST_YEAR) << " at " << highestPopulation << " people.\n";
}

bool processLine(std::string line)
{
	int birth, death;
	char number[C_YEAR_DIGITS];
	std::size_t index;

	//Extract the two years by finding the first two numbers on this line
	index = line.find_first_of("0123456789");
	strncpy(number, line.c_str() + index, C_YEAR_DIGITS);
	birth = atoi(number);
	index = line.find_first_of("0123456789", index + C_YEAR_DIGITS);
	strncpy(number, line.c_str() + index, C_YEAR_DIGITS);
	death = atoi(number);

	//If a number found is not a year, or not enough years were found, abort the line
	if ((birth < C_FIRST_YEAR) || (birth > C_LAST_YEAR) || (death < C_FIRST_YEAR) || (death > C_LAST_YEAR))
	{
		return false;
	}

	if (birth > death)
	{
		int temp = birth;
		birth = death;
		death = temp;
	}

	g_PopulationChangeArray[birth - C_FIRST_YEAR] += 1;
	if (death < C_LAST_YEAR)
	{
		g_PopulationChangeArray[death - C_FIRST_YEAR + 1] -= 1;
	}

	return true;
}

bool readFile(std::string filename)
{
	bool returnValue = false;
	std::ifstream theFile(filename);
	if (theFile.is_open())
	{
		while (!theFile.eof())
		{
			std::string line;
			std::getline(theFile, line);
			returnValue |= processLine(line);
		}
	}
	return returnValue;
}

int main(int argc, char *argv[])
{
	bool askForFilename = true;

	//Try the args first if there are any
	for (int i = 1; i < argc; i++)
	{
		if (readFile(argv[i]))
		{
			askForFilename = false;
		}
	}

	//If the args didn't have a valid file, ask for a file
	if (askForFilename)
	{
		std::string filename;
		std::cout << "Please enter the name of the file that contains the population data.\n";
		getline(std::cin, filename);
		if (readFile(filename))
		{
			askForFilename = false;
		}
	}

	//If any population data was gathered, find the highest population year
	if (!askForFilename)
	{
		performFinalSearch();
		std::cout << "Press \"Enter\" to exit.";
		std::getchar();
	}

	return 0;
}