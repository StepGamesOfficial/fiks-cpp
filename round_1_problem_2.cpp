// Network.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


// GLOBAL VARIABLES
std::vector<std::vector<int>> inputs;
// ...


int main()
{
	// PARSE AND INPUT LOGIC:______________________________
	std::cout << "type file name:" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::string infilename = filename + ".txt";
	std::string outfilename = "output.txt"; // filename + ".out";
	std::fstream inputFile;
	std::ofstream outputFile;
	inputFile.open(infilename, std::ios::in);
	outputFile.open(outfilename);

	std::string currentLine;
	std::string currText = "";


	int totalinputs = 0;
	int inputsleft = 0;
	while (getline(inputFile, currentLine))
	{
		int i = -1;
		for (char c : currentLine)
		{
			i++;
			if (c == ' ' || i == currentLine.length()-1)
			{
				if(i == currentLine.length()-1)
					currText += c;

				//std::cout << currText << std::endl;

				if (totalinputs == 0)
				{
					totalinputs = std::stoi(currText);
					currText = "";
				}
				else if (inputsleft == 0)
				{
					/*if(!inputs.empty())
						std::cout << "No inputs left, first one was: " << inputs.back().front() << ", new inputs: "<< currText << std::endl;*/
					inputsleft = std::stoi(currText);
					currText = "";
					inputs.push_back({});
				}
				else
				{
					inputs.back().push_back(std::stoi(currText));
					currText = "";
					inputsleft--;
				}
			}
			else
			{
				currText += c;
			}
		}
	}


	// ACTUAL CODE: _________________________________________________
	std::cout << "Starting with: " << totalinputs << " inputs!\n";
	int currinput = 0;
	for (auto currentInput : inputs)
	{
		currinput++;
		std::cout << currinput << std::endl;

		std::sort(currentInput.begin(), currentInput.end());

		int result = 0;

		int largest = 1;
		int largestcount = 0;


		for (int i = currentInput.size(); i >= 1; i--)
		{
			int count = 0;
			for (int number : currentInput)
			{
				if (number % i == 0)
					count++;
			}
			if (count >= i)
			{
				largest = i;
				break;
			}
		}


		/* OG SOLUTION:		(for 3 points)

		for (int number : currentInput)
		{
			int count = 0;
			for (int i = currentInput.size(); i >= 1; i--)
			{
				if (number % i == 0)
				{
					count++;
				}				
				if (i * count > largestcount && i <= count)
				{
					largest = i;
					int largestcount = i * count;
					break;
				}
			}
		}
		*/
		
		std::cout << std::endl;
		std::cout << "LARGEST: " << largest << std::endl;
		
		outputFile << largest << "\n";
	}
	
	outputFile.close();
}
