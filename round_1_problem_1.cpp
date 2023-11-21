// Uz jsi dodelal tmaze settings?
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

struct Machine
{
	int y, x;
	int id;
};

struct Input
{
	std::vector<Machine> machines;
	int height, width;
};

// GLOBAL VARIABLES
int inputLength = NULL;
bool debug = false;
std::vector<Input> Inputs;


// HASHMAPS
std::unordered_map<std::string, int> ids;
//...

// FUNCTION DECLARATIONS
bool sortMachines(Machine &m1, Machine &m2, Input currentInput);
//...

//---------
int main()
{
	// INPUT AND PARSE LOGIC: ______________________________________________________________

	std::cout << "debug? y/n" << std::endl;
	std::string debugornot;
	std::cin >> debugornot;
	if (debugornot == "y")
		debug = true;

	std::cout << "type file name:" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::string infilename = filename + ".txt";
	std::string outfilename = "output.txt"; // filename + ".out";
	std::fstream inputFile;
	std::ofstream outputFile;
	inputFile.open(infilename, std::ios::in);

	std::string currentLine;
	std::string currText = "";

	int inputStep = 1; //		___1 = h / y ___ 2 = w / x ___ 3 = n / c___

	int machinesLeft = 0;

	while (getline(inputFile, currentLine))
	{
		if (machinesLeft == 0) // New Input?
		{
			Inputs.push_back({});
		}
		else
		{
			Inputs.back().machines.push_back({});
		}

		int i = -1;
		for (char c : currentLine)
		{
			i++;
			if (i == currentLine.length() - 1)
				currText += c;

			if (c == ' ' || i == currentLine.length() - 1)
			{
				if (debug)
					std::cout << "NEW INPUT:" << currText << ";" << std::endl;

				// ---- DECIDING WHICH VALUE THIS IS ----
				if (inputLength == NULL)
				{
					inputLength = std::stoi(currText);
					if (debug)
						std::cout << "This: >" + currText + "< is the amount of inputs" << std::endl;
				}
				else if (inputStep == 1)
				{
					if (machinesLeft == 0)
					{
						if (debug)
							printf("Height\n");
						Inputs.back().height = std::stoi(currText);
					}
					else
					{
						if (debug)
							printf("<Y>\n");
						Inputs.back().machines.back().y = std::stoi(currText);;
					}
					inputStep = 2;
				}
				else if (inputStep == 2)
				{
					if (machinesLeft == 0)
					{
						if (debug)
							printf("Width\n");
						Inputs.back().width = std::stoi(currText);
					}
					else
					{
						if (debug)
							printf("<X>\n");
						Inputs.back().machines.back().x = std::stoi(currText);;
					}
					inputStep = 3;
				}
				else if (inputStep == 3)
				{
					if (machinesLeft == 0)
					{
						if (debug)
							printf("MachineCount\n");
						machinesLeft = std::stoi(currText);
					}
					else
					{
						if (debug)
							printf("MachineName\n");
						auto idfindtry = ids.find(currText);
						if (idfindtry == ids.end())
						{
							int newid = ids.size();
							ids.insert({ currText, ids.size() });
							Inputs.back().machines.back().id = newid;
						}
						else
						{
							Inputs.back().machines.back().id = idfindtry->second;
						}

						if (debug)
							std::cout << currText << " has the id of: " << Inputs.back().machines.back().id << std::endl;
						machinesLeft--;
					}
					inputStep = 1;
				}
				// ----
				if (debug)
					printf("\n");
				currText = "";
			}
			else
			{
				currText += c;
			}
		}
	}
	std::cout << "INPUT IS RECEIVED!" << std::endl;

	// ACTUAL CODE: ___________________________________________________________________________

	std::cout << "Starting! Total inputs: " << Inputs.size() << std::endl << std::endl;

	Inputs.erase(Inputs.begin());
	outputFile.open(outfilename);

	int inputcount = -1;
	for (Input currentInput : Inputs)
	{
		inputcount++;
		std::sort(currentInput.machines.begin(), currentInput.machines.end(), [&](Machine &m1, Machine &m2) { return  sortMachines(m1, m2, currentInput); });


		std::vector<Machine> stack;
		for (auto i : currentInput.machines)
		{
			if (stack.empty())
			{
				stack.push_back(i);
			}
			else if (i.id == stack.back().id)
			{
				stack.pop_back();
			}
			else
			{
				stack.push_back(i);
			}
		}

		if (stack.empty())
		{
			std::cout << "pujde to" << std::endl;
			outputFile << "pujde to\n";
		}
		else
		{
			std::cout << "ajajaj" << std::endl;
			outputFile << "ajajaj\n";
		}
		std::cout << inputcount << std::endl;

	}
	outputFile.close();
}

// FUNCTIONS:
bool sortMachines(Machine &m1, Machine &m2, Input currentInput)
{
	int aloc = 0; // (loc = location)
	int	bloc = 0; // which quarter? 0 - 3

	int i = -1;

	for (Machine m : {m1, m2})
	{
		i++;

		if (m.y == 0)
			if (i == 0) { aloc = 0; }
			else { bloc = 0; };
		if (m.x == currentInput.width && m.y != 0)
			if (i == 0) { aloc = 1; }
			else { bloc = 1; };
		if (m.y == currentInput.height && m.x != currentInput.width)
			if (i == 0) { aloc = 2; }
			else { bloc = 2; };
		if (m.x == 0 && m.y != currentInput.height && m.y != 0)
			if (i == 0) { aloc = 3; }
			else { bloc = 3; };
	}

	if (aloc > bloc)
		return true;
	else if (aloc < bloc)
		return false;
	else
	{
		switch (aloc)
		{
		case 0:
			return m1.x > m2.x;
			break;

		case 1:
			return m1.y > m2.y;
			break;

		case 2:
			return m1.x < m2.x;
			break;

		case 3:
			return m1.y < m2.y;
			break;
		}
	}
}