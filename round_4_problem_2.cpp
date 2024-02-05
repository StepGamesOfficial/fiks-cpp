#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h> 
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

struct InputEdge 
{
	int A;
	int B;
	bool hot;

	int id;
};

struct Input
{
	int numOfNodes;
	std::vector<InputEdge> EdgesToAdd;
};

struct Edge;
struct Node;


struct Edge
{
	Node *start;
	Node *end;
	bool hot;

	int id;
};

struct Node 
{
	std::vector<Edge*> nextEdges;

	int id;
};


struct pathStep {			// this will be used for creating: 1) current queue, 2) next queue
	std::vector<int> pathSoFar;
	int tempSoFar;
	std::vector<Edge*> nextUp;
	std::unordered_set<int> visitedEdges;

	int lastNode;
};

struct solution {
	std::vector<int> path;
	int temperature;
};


int main()
{
	
	// ___________________________________________ INPUT: 
	std::cout << "type file name:" << std::endl;
	std::string filename;
	std::cin >> filename;
	std::string infilename = filename + ".txt";
	std::string outfilename = filename + "_out.txt";
	std::fstream inputFile;
	std::ofstream outputFile;
	inputFile.open(infilename, std::ios::in);
	char currentCharacter;
	std::string currNumber = "";
	bool wasValidBefore = false;

	std::vector<Input> Inputs;

	int inputsLeft = -1;
	int pathsLeft = 0;
	int lastStep = -1;
	int nextEdgeIndex = 0;

	// ______________________________________________ PARSE:
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
			else if (pathsLeft == 0 && lastStep != 0)
			{
				// parse num of nodes
				lastStep = 0;
				nextEdgeIndex = 0;
				Inputs.push_back({});

				Inputs.back().numOfNodes = std::stoi(currNumber);
				inputsLeft--;
			}
			else if (lastStep == 0)
			{
				// parse num of paths
				pathsLeft = std::stoi(currNumber);
				lastStep = 1;
			}
			else // parse all edges' info
			{
				if (lastStep == 1) // parse A
				{
					Inputs.back().EdgesToAdd.push_back({}); // new edge
					Inputs.back().EdgesToAdd.back().A = std::stoi(currNumber);
					lastStep = 2;

					Inputs.back().EdgesToAdd.back().id = nextEdgeIndex; nextEdgeIndex++; // parse id

				}
				else if (lastStep == 2) // parse B
				{
					Inputs.back().EdgesToAdd.back().B = std::stoi(currNumber);
					lastStep = 3;

				}
				else if (lastStep == 3) // parse temperature
				{
					Inputs.back().EdgesToAdd.back().hot = (currNumber == "ohniva");
					lastStep = 1;
					pathsLeft--;
				}
			}
			currNumber = "";
		}
	}





	// _______________________________________________ LOGIC:
	outputFile.open(outfilename);
	int inputIndex = -1;
	for (auto currInput : Inputs)
	{
		std::unordered_set<int> visitedNodes;
		inputIndex++;


		// __ graph generation:

		std::vector<Node*> nodes(currInput.numOfNodes);
		std::vector<Edge*> edges;
		int indexrn = -1;
		for (auto& currNode : nodes)
		{
			currNode = new Node();
			indexrn++;
			currNode->id = indexrn;
		}
		
		for (auto currNewEdge : currInput.EdgesToAdd)
		{
			edges.push_back(new Edge{});
			edges.back()->start = nodes[currNewEdge.A];
			edges.back()->end = nodes[currNewEdge.B];
			edges.back()->hot = currNewEdge.hot;
			edges.back()->id = currNewEdge.id;

			nodes[currNewEdge.A]->nextEdges.push_back(edges.back());
			nodes[currNewEdge.B]->nextEdges.push_back(edges.back());

		}


		// __ algorithm:

		bool reachedEnd = false;
		std::vector<pathStep> currentQueue;
		std::vector<pathStep> nextQueue;
		std::vector<solution> solutions;
		if (!nodes.empty())
		{
			nextQueue.push_back({});
			nextQueue.back().nextUp = nodes[0]->nextEdges;
			nextQueue.back().tempSoFar = 0;
			nextQueue.back().lastNode = nodes[0]->id;
		}
		


		while (!nextQueue.empty() && solutions.empty())
		{
			if (edges.empty())
				break;

			currentQueue = nextQueue;
			nextQueue.clear();
			nextQueue.shrink_to_fit();

			std::unordered_set<int> visitedNodesThisLayer;
			for (auto currPath : currentQueue)   
			{
				bool shouldContinue = true;

					
				if (currPath.nextUp.empty())
				{
					if(currPath.pathSoFar.empty())
						shouldContinue = false;
					else if ((edges[currPath.pathSoFar.back()]->end->id == nodes.back()->id) || (edges[currPath.pathSoFar.back()]->start->id == nodes.back()->id))
					{
						// valid path!!
						solutions.push_back({});
						solutions.back().path = currPath.pathSoFar;
						solutions.back().temperature = currPath.tempSoFar;
					}
					else
					{
						shouldContinue = false;
					}
				}
				else
				{
					if (!currPath.pathSoFar.empty())
					{
						if ((edges[currPath.pathSoFar.back()]->end->id == nodes.back()->id) || (edges[currPath.pathSoFar.back()]->start->id == nodes.back()->id))
						{
							// valid path!!
							solutions.push_back({});
							solutions.back().path = currPath.pathSoFar;
							solutions.back().temperature = currPath.tempSoFar;
						}
					}
					
				}

				if (shouldContinue)
				{
					for (auto &currNewNU : currPath.nextUp) //	------------------ nextQueue setup
					{

						bool useEnd = (currNewNU->start->id == currPath.lastNode);

						if (visitedNodes.count((useEnd ? currNewNU->end->id : currNewNU->start->id)) > 0)
						{
							continue;
						}
						

						nextQueue.push_back({});
						nextQueue.back().tempSoFar = currPath.tempSoFar + ((currNewNU->hot) ? +1 : -1);

						nextQueue.back().pathSoFar = currPath.pathSoFar;
						nextQueue.back().pathSoFar.push_back(currNewNU->id);
						nextQueue.back().nextUp = (useEnd ? currNewNU->end->nextEdges : currNewNU->start->nextEdges);

						nextQueue.back().lastNode = (useEnd ? currNewNU->end->id : currNewNU->start->id);

						visitedNodesThisLayer.insert((useEnd ? currNewNU->end->id : currNewNU->start->id));

					}
				}
			}

			for(auto& currnode : visitedNodesThisLayer)
				visitedNodes.insert(currnode);

			visitedNodesThisLayer.clear();


		}
		if (edges.empty() && nodes.size() == 1)
		{
			outputFile << "pohoda 0 0\n";
		}
		else if (solutions.empty())
		{
			outputFile << "ajajaj\n";
		}
		else
		{
			int closestTempValue = -1;
			int closestTempIndex = -1;

			int ind = -1;
			for (auto currSolution : solutions)
			{
				ind++;
				if (abs(currSolution.temperature) < closestTempValue || closestTempValue == -1)
				{
					closestTempValue = abs(currSolution.temperature);
					closestTempIndex = ind;
				}
			}


			outputFile << "pohoda " << solutions[closestTempIndex].temperature << " " << solutions[closestTempIndex].path.size() << " ";

			int currStepindex = -1;
			for (auto currStep : solutions[closestTempIndex].path)
			{
				currStepindex++;
				outputFile << currStep;

				if(currStepindex != solutions[closestTempIndex].path.size() - 1)
					outputFile << " ";
				else
					outputFile << "\n";
			}
		}

		
		for (auto& currNode : nodes)
			delete currNode;

		for (auto& currEdge : edges)
			delete currEdge;
	}

	outputFile.close();
}