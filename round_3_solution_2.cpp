#include "pch.h"
#include <conio.h> 
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

struct STC {
	std::vector<std::bitset<32>> subnets[2];
	int originalSubnetIndex;
	int sum;
};

struct Input
{
	int subnetsNum;
	std::vector<std::bitset<32>> IPs;
};


// functions:
std::string ipToBinary(const std::string& ipAddress);
std::string binaryToIp(const std::string& binaryIp);
bool isNumeric(std::string line);
std::bitset<32> binaryTo32bit(std::string binarynumber);
bool sortFunc(STC a, STC b);
void printIP(std::bitset<32> IP);
void printIPs(std::vector<std::bitset<32>> IPs);
//...




int main() {

	// INPUT: ___________________________________________
	std::cout << "type file name:" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::string infilename = filename + ".txt";
	std::string outfilename = filename + ".out";
	std::fstream inputFile;
	std::ofstream outputFile;
	inputFile.open(infilename, std::ios::in);
	char currentCharacter;
	std::string currNumber = "";
	bool wasValidBefore = false;

	std::vector<Input> Inputs;
	int inputsLeft = -1;
	int IPsLeft = 0;
	bool lastIPs = false;



	//____________________________________________PARSE LOGIC:
	while (inputFile.get(currentCharacter))
	{
		if (currentCharacter != ' ' && currentCharacter != '\n')
		{
			currNumber.push_back(currentCharacter);
			wasValidBefore = true;
		}
		else if (wasValidBefore)
		{
			wasValidBefore = false;
			if (inputsLeft == -1)
			{
				inputsLeft = std::stoi(currNumber);
			}
			else if (IPsLeft == 0)
			{
				IPsLeft = std::stoi(currNumber);
				lastIPs = true;
				Inputs.push_back({});
				inputsLeft--;
			}
			else if (lastIPs)
			{
				lastIPs = false;
				Inputs.back().subnetsNum = std::stoi(currNumber);
			}
			else
			{
				Inputs.back().IPs.push_back(binaryTo32bit(ipToBinary(currNumber)) );
				
				IPsLeft--;
			}
			wasValidBefore = false;
			currNumber = "";
		}
	}
	// ...



	// _______________________________________________ CODE:
	outputFile.open(outfilename);

	for (Input currInput : Inputs)
	{
		int oneSubnetSize = currInput.IPs.size() / currInput.subnetsNum;
		int subnetsErased = 0;


		std::vector<std::bitset<32>> ipsToAdd = currInput.IPs;
		std::vector<std::vector<std::bitset<32>>> subnets;

		subnets.push_back({});
		for (auto currIP : currInput.IPs)
			subnets.back().push_back(currIP);

		std::vector<STC> subnetsToCreate;

		for (int bi = 31; bi >= 0; bi--) // bits 
		{
			for (size_t si = 0; si < subnets.size(); si++) // subnets
			{

				std::vector<std::bitset<32>> zeroMatches;
				std::vector<std::bitset<32>> oneMatches;

				for (size_t ipi = 0; ipi < subnets[si].size(); ipi++) // subnet ips
				{

					if (!subnets[si][ipi].test(bi))
					{
						zeroMatches.push_back(subnets[si][ipi]);
					}
					else
					{
						oneMatches.push_back(subnets[si][ipi]);
					}
				}
				// save zeroMatches + oneMatches into subnetsToCreate
				if (!zeroMatches.empty() && !oneMatches.empty())
				{
					subnetsToCreate.push_back({});
					subnetsToCreate.back().subnets[0] = zeroMatches; 
					subnetsToCreate.back().subnets[1] = oneMatches;
					subnetsToCreate.back().originalSubnetIndex = si;
					if (zeroMatches.size() > oneMatches.size())
					{
						subnetsToCreate.back().sum = zeroMatches.size();
					}
					else
					{
						subnetsToCreate.back().sum = oneMatches.size();
					}
				}
			}

			std::sort(subnetsToCreate.begin(), subnetsToCreate.end(), [&](STC &e1, STC &e2) {return e1.sum > e2.sum;});

				std::vector<std::vector<std::bitset<32>>> tempSubnets = subnets;
				int tempsubnetsErased = subnetsErased;
				for (auto currSTC : subnetsToCreate)
				{
					if ((tempSubnets.size() - tempsubnetsErased) >= currInput.subnetsNum)
					{
						break;
					}

					tempSubnets[currSTC.originalSubnetIndex] = {};

					tempSubnets.push_back(currSTC.subnets[0]);
					tempSubnets.push_back(currSTC.subnets[1]);
					tempsubnetsErased++;
				}

					bool passed = true;
					int last = -1;
					for (auto currtempsub : tempSubnets)
					{
						for (auto currtempIp : currtempsub)
						{
							if (last == -1)
								last = currtempIp.test(bi);
							else if (last != currtempIp[bi])
							{
								passed = false;
								break;
							}
						}
						last = -1;
						if (!passed)
							break;
					}

					if (passed) // passed
					{
						for (auto currSTC : subnetsToCreate)
						{
							if ((subnets.size() - subnetsErased) >= currInput.subnetsNum)
								break;

							subnets[currSTC.originalSubnetIndex] = {};
							subnets.push_back(currSTC.subnets[0]);
							subnets.push_back(currSTC.subnets[1]);
							subnetsErased++;
						}
					}

					subnetsToCreate.clear();

				if ((subnets.size() - subnetsErased) >= currInput.subnetsNum)
					break;
		}

		// generate mask
		std::bitset<32> mask;
		for (size_t bi = 0; bi < 32; bi++)
		{
			bool yes = true;
			int last = -1;
			for (auto currSub : subnets)
			{
				for (auto currIP : currSub)
				{
					if (last == -1)
						last = currIP.test(bi);
					else if (last != currIP[bi])
					{
						yes = false;
						break;
					}
				}
				last = -1;
				if (!yes)
					break;
			}
			mask[bi] = yes;
		}
	

		outputFile << binaryToIp(mask.to_string()) + '\n';


	}

	//______________________________ OUTPUT
	outputFile.close();
}

void printIP(std::bitset<32> IP) {

	std::cout << IP.to_string() << " / ";
	std::cout << binaryToIp(IP.to_string());
		std::cout << std::endl;
}
void printIPs(std::vector<std::bitset<32>> IPs) {
	for (auto currIP : IPs)
	{
		
		std::cout << currIP.to_string() << " / ";
		std::cout << binaryToIp(currIP.to_string());
		std::cout << std::endl;
	}
}

// IP TO BINARY + BINARY TO IP LOGIC
std::string ipToBinary(const std::string& ipAddress) {
	std::stringstream ss(ipAddress);
	std::string binaryIp, octet;
	while (std::getline(ss, octet, '.')) {
		binaryIp += std::bitset<8>(std::stoi(octet)).to_string();
	}
	return binaryIp;
}

std::string binaryToIp(const std::string& binaryIp) {
	std::string ipAddress;
	for (size_t i = 0; i < binaryIp.size(); i += 8) {
		ipAddress += std::to_string(std::bitset<8>(binaryIp.substr(i, 8)).to_ulong());
		if (i + 8 < binaryIp.size()) {
			ipAddress += '.';
		}
	}
	return ipAddress;
}

std::bitset<32> binaryTo32bit(std::string binarynumber)
{
	std::bitset<32> binarynum(binarynumber);
	return binarynum;
}

bool isNumeric(std::string line)
{
	char* p;
	strtol(line.c_str(), &p, 10);
	return *p == 0;
}

bool sortFunc(STC a, STC b)
{
	return (a.sum) > (a.sum);
}