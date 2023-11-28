// This is a polynomial solution for round 2 problem 1 of FIKS
// This solution only calculates one possible solution, thus for 7 points
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

int inputLength;

struct Input {
	int numOfScripts;
	std::vector<std::vector<int>> config;
};

struct Script { // contains 'sum' for easier sorting
	std::vector<int> configList;
	int sum = 0;
};

std::vector<Input> Inputs;


int main()
{

	// INPUT LOGIC: ___________________________________________
	std::ofstream outputFile;
	std::string line;
	int configRemaining = 0;
	while (std::getline(std::cin, line))
	{
		if (line.empty())
			break;

		std::istringstream ss(line);
		if (configRemaining == 0)
		{
			Inputs.push_back({});

			ss >> configRemaining >> Inputs.back().numOfScripts;
		}
		else
		{
			configRemaining--;
			int currentNum;
			Inputs.back().config.push_back({}); // add new config index
			bool firstIt = true;
			while (ss >> currentNum)
			{
				if (firstIt)
					firstIt = false; // skip first number (size)
				else
				{
					Inputs.back().config.back().push_back(currentNum - 1); // put all scripts influenced by config in vector
				}
			}
		}
	}
	//...

	// SOLUTION: _______________________________________________ 

	for (Input currentInput : Inputs)
	{
		bool noSolution = false;
		std::vector <Script> scriptLocations;
		std::vector <int> finalConfig; // solution will be saved into this



		// now we convert the input: 
		// for n scripts we'll have m config bool values (true = this index's config influences this script)
		// NOTE: the variables are actually integers to allow empty slots => I say TRUE/FALSE as in 1/0
		
		// here we create the lists' items beforehand, so we can set individual indexes' values:

		bool finalConfigSetup = false;
		for (size_t i = 0; i < currentInput.numOfScripts; i++)
		{
			scriptLocations.push_back({});
			for (size_t i = 0; i < currentInput.config.size(); i++)
			{
				scriptLocations.back().configList.push_back(false);

				if(!finalConfigSetup)
					finalConfig.push_back(-1);
			}
			finalConfigSetup = true;
		}

		// here we assign the TRUE values, and calculate all sums for later sorting:
		std::vector<int> emptyConfig = finalConfig;
		int configIndex = -1;
		for (auto currentConfig : currentInput.config)
		{
			configIndex++;
			for (int currentScript : currentConfig)
			{
				scriptLocations[currentScript].configList[configIndex] = true;
				scriptLocations[currentScript].sum++;
			}
		}


		// now, we sort these lists by the num of TRUE's (sum of 1s and 0s)	(I should be allowed to use std::sort if I understand it, right?)
		std::sort(scriptLocations.begin(), scriptLocations.end(), [&](Script &s1, Script &s2) {return s1.sum < s2.sum; });


		// Now, let's find the solution.
		//
		// for each script in scriptLocations:
		// 1) we check the 'finalConfig' list, and if it contains values, we replace all TRUEs with those values
		// 2) for all remaining TRUE slots: let's check if the sum of all now occupied TRUE slots is dividible by two
		//		a) if sum / 2 we replace the first remaining TRUE slot with a 1, and all remaining with 0s
		//		b) if !(sum / 2), we replace all remaining TRUE slots with 0s
		//		(if the sum is still % 2 even after these operations, we have no solution.)
		// 3) these new values from step 2) are now inserted into 'finalConfig'
		// 
		// and REPEAT.


		for (auto currentScript : scriptLocations)
		{
			std::vector<int> curr = currentScript.configList;

			for (int i = 0; i < curr.size(); i++)
			{
				curr[i] -= 2; // converts 1s and 0s into -1s and -2s (to distinguish from 1s and 0s added into final config)
			}
			for (int i = 0; i < finalConfig.size(); i++)
			{
				if (finalConfig[i] != -1 && curr[i] != -2)
				{
					curr[i] = finalConfig[i];
				}
			}
			int sum = 0;
			bool insertedOne = false;
			for (int x : curr) 
			{
				if (x == 1)
					sum++;
			}

			if(sum % 2 != 0)	// do we need to balance it out (to be odd)?
				insertedOne = true;

			for (int i = 0; i < curr.size(); i++)
			{
				if (curr[i] == -1) // can we insert here? (-1 = TRUE, -2 = FALSE)
				{
					if (insertedOne == true) // have we already balanced this to be odd?
					{
						curr[i] = 0;
						finalConfig[i] = 0;
					}
					else
					{
						curr[i] = 1;
						finalConfig[i] = 1;
						insertedOne = true;
					}
				}

			}
			if (!insertedOne)
			{
				noSolution = true; break;
			}
		}
		

		// OUTPUT: _________________________________
		if(noSolution)
			std::cout << "No solution!" << std::endl;
		else
		{
			for (auto conf : finalConfig)
			{
				std::cout << conf;
			}
			std::cout << std::endl;
		}
	}
}
