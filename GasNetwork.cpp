#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <cmath>
#include "inputcheck.h"
#include <cfloat>
#include "GasNetwork.h"
#include "Pipe.h"
#include "CS.h"

using std::ofstream;
using std::ifstream;
using std::string;
using std::cout;
using std::cin;
using std::endl;

std::ifstream& operator>>(std::ifstream& in, Connection& c)
{
	int inputCSId, outputCSId, pipeId;

	in >> inputCSId >> outputCSId >> pipeId;

	if (in.fail())
	{
		c.pipeId = 0;
		return in;
	}

	c.csInputId = inputCSId;
	c.csOutputId = outputCSId;
	c.pipeId = pipeId;

	return in;
}
std::ofstream& operator<<(std::ofstream& out, const Connection& c)
{
	out << 'G' << endl;
	out << c.csInputId << " ";
	out << c.csOutputId << " ";
	out << c.pipeId << endl;

	return out;
}

void saveObjects(const std::unordered_map<int, Pipe>& mapPipe,
	const std::unordered_map<int, CS>& mapCS, const std::vector<Connection>& arrCon)
{
	if (mapPipe.empty() && mapCS.empty())
	{
		cout << "There is no data to save!" << endl;
		return;
	}

	string fileName;

	cout << "Enter the file name (without extension and spaces): ";
	cin >> std::ws;
	inputLine(fileName, cin);

	std::ofstream f(fileName + ".txt");

	if (!f.is_open())
	{
		cout << "Error: the file to save was not found!" << endl;
		return;
	}

	for (const auto& p : mapPipe)
	{
		f << p.second;
	}

	for (const auto& cs : mapCS)
	{
		f << cs.second;
	}

	for (const auto& c : arrCon)
	{
		f << c;
	}

	f.close();

	cout << "Done!" << endl;
}

void loadObjects(std::unordered_map<int, Pipe>& mapPipe,
	std::unordered_map<int, CS>& mapCS, std::vector<Connection>& arrCon)
{
	string fileName;

	cout << "Enter the file name (without extension and spaces): ";
	cin >> std::ws;
	inputLine(fileName, cin);

	std::ifstream f(fileName + ".txt");

	if (!f.is_open() || f.peek() == EOF)
	{
		cout << "Error: the download file was not found or is empty!" << endl;
		return;
	}

	int maxPipeId, maxCSId;

	maxPipeId = Pipe::idCount;
	Pipe::idCount = 0;
	maxCSId = CS::idCount;
	CS::idCount = 0;

	std::unordered_map<int, Pipe> newMapPipe;
	std::unordered_map<int, CS> newMapCS;
	std::vector<Connection> newArrCon;

	Pipe tmpPipe;
	CS tmpCS;
	Connection tmpConnection;
	char type;

	while (f.peek() != EOF)
	{
		type = f.get();
		f.get();

		if (type != 'P' && type != 'C' && type != 'G')
		{
			cout << "Error: incorrect reading of the file!" << endl;

			Pipe::idCount = maxPipeId;
			CS::idCount = maxCSId;

			f.close();
			return;
		}

		if (type == 'P')
		{
			f >> tmpPipe;

			if (tmpPipe.getID() == -1)
			{
				cout << "Error: incorrect reading of the file!" << endl;

				Pipe::idCount = maxPipeId;
				CS::idCount = maxCSId;

				f.close();
				return;
			}

			Pipe::idCount = std::max(Pipe::idCount, tmpPipe.getID() + 1);
			newMapPipe.emplace(tmpPipe.getID(), tmpPipe);
		}
		else if (type == 'C')
		{
			f >> tmpCS;

			if (tmpCS.getID() == -1)
			{
				cout << "Error: incorrect reading of the file!" << endl;

				Pipe::idCount = maxPipeId;
				CS::idCount = maxCSId;

				f.close();
				return;
			}

			CS::idCount = std::max(CS::idCount, tmpCS.getID() + 1);
			newMapCS.emplace(tmpCS.getID(), tmpCS);
		}
		else
		{
			f >> tmpConnection;

			if (tmpConnection.pipeId  == -1)
			{
				cout << "Error: incorrect reading of the file!" << endl;

				Pipe::idCount = maxPipeId;
				CS::idCount = maxCSId;

				f.close();
				return;
			}

			newArrCon.push_back(tmpConnection);
		}

		f.get();
	}

	mapPipe = newMapPipe;
	mapCS = newMapCS;
	arrCon = newArrCon;

	f.close();

	cout << "Done!" << endl;
}

void addConnection(std::vector<Connection>& arrCon, std::unordered_map<int, Pipe>& mapP,
	std::unordered_map<int, CS>& mapCS)
{
	if (mapP.empty() || mapCS.size() < 2)
	{
		cout << "You cannot create a connection: there is not enough CS!" << endl;
		return;
	}

	bool isExisting;
	Connection newC;
	int inputId, outputId, diameter;

	printCS(mapCS);
	
	cout << "Enter sequentially through Enter "
		<< "The ID of the input CS and the ID of the output CS:" << endl;

	while (true)
	{
		checkInput(inputId, 0, INT_MAX);

		if (!mapCS.contains(inputId))
		{
			std::cout << "There is no CS with the specified id!" << std::endl
				<< "Try again: ";
			continue;
		}

		break;
	}

	while (true)
	{
		checkInput(outputId, 0, INT_MAX);

		if (!mapCS.contains(outputId) || (outputId == inputId))
		{
			cout << "Incorrect input!" << endl
				<< "Try again: ";
			continue;
		}

		break;
	}

	isExisting = false;
	for (const auto& c : arrCon)
	{
		if (c.csInputId == inputId && c.csOutputId == outputId)
		{
			isExisting = true;
			break;
		}
	}

	if (isExisting)
	{
		cout << "The pipe between the specified CS already exists!" << endl;
		return;
	}

	newC.csInputId = inputId;
	newC.csOutputId = outputId;

	mapCS.at(inputId).isInConnection = true;
	mapCS.at(outputId).isInConnection = true;

	cout << "Select the pipe diameter: " << endl;
	cout << "1) 500 mm" << endl << "2) 700 mm" << endl
		<< "3) 1000 mm" << endl << "4) 1400 mm" << endl;
	checkInput(diameter, 1, 4);
	diameter = pipeDiameters.at(diameter - 1);

	newC.pipeId = -1;

	for (const auto& [id, p] : mapP)
	{
		if (filtByDiameter(p, diameter))
		{
			if (p.getStatus() || p.isInConnection)
			{
				continue;
			}

			newC.pipeId = id;
			break;		
		}
	}

	if (newC.pipeId == -1)
	{
		Pipe p;
		createPipeWithGivenDiameter(p, diameter);
		mapP.emplace(p.getID(), p);
		newC.pipeId = p.getID();
	}

	arrCon.push_back(newC);
	mapP.at(newC.pipeId).isInConnection = true;

	cout << "Done!" << endl;
}

void printConnection(const Connection& c,
	const std::unordered_map<int, Pipe>& mapP)
{
	std::cout << "CS input ID: " << c.csInputId << " -> "
		<< "Pipe " << c.pipeId << " diameter " << mapP.find(c.pipeId)->second.getDiameter()
		<< " mm and length "  << mapP.find(c.pipeId)->second.getLength() 
		<< " m -> CS output ID: " << c.csOutputId << endl;
}

void printConnection(const std::vector<Connection> arrCon,
	const std::unordered_map<int, Pipe>& mapP)
{
	if (arrCon.empty())
	{
		cout << "There are no connections!" << endl;
		return;
	}


	cout << "Connections:" << endl << endl;

	for (int i = 0; i < arrCon.size(); ++i)
	{
		cout << i + 1 << ") ";
		printConnection(arrCon.at(i), mapP);
	}

	cout << endl;
}

void deleteConnection(std::vector<Connection>& arrCon, std::unordered_map<int, Pipe>& mapP,
	std::unordered_map<int, CS>& mapCS)
{
	if (arrCon.empty())
	{
		cout << "There are no connections!" << endl;
		return;
	}

	printConnection(arrCon, mapP);

	cout << "Enter the number of the connection you want to delete (0 - cancel): "; 

	int indToDelete;
	checkInput(indToDelete, 0, int(arrCon.size()));

	if (indToDelete == 0)
	{
		cout << "Cancellation of the action" << endl;
		return;
	}

	--indToDelete;
	
	mapP.at(arrCon.at(indToDelete).pipeId).isInConnection = false;

	bool inputCSHaveConnections = false;
	bool outputCSHaveConnections = false;

	for (int i = 0; i < arrCon.size(); ++i)
	{
		if ((arrCon.at(i).csInputId == arrCon.at(indToDelete).csInputId ||
			arrCon.at(i).csOutputId == arrCon.at(indToDelete).csInputId) && i != indToDelete)
		{
			inputCSHaveConnections = true;
		}

		if ((arrCon.at(i).csInputId == arrCon.at(indToDelete).csOutputId ||
			arrCon.at(i).csOutputId == arrCon.at(indToDelete).csOutputId) && i != indToDelete)
		{
			outputCSHaveConnections = true;
		}

		if (inputCSHaveConnections && outputCSHaveConnections)
			break;
	}

	if (!inputCSHaveConnections)
	{
		mapCS.at(arrCon.at(indToDelete).csInputId).isInConnection = false;
	}

	if (!outputCSHaveConnections)
	{
		mapCS.at(arrCon.at(indToDelete).csOutputId).isInConnection = false;

	}

	arrCon.erase(arrCon.begin() + indToDelete);

	cout << "Done!" << endl;
}


void convertToMatrix(const std::vector<Connection>& arrCon,
	const std::unordered_map<int, Pipe>& mapPipe, graphMatrix& graph)
{
	if (arrCon.empty())
	{
		return;
	}

	std::unordered_set<int> inVerts;

	for (const auto& con : arrCon)
	{
		if (!graph.contains(con.csInputId))
		{
			graph.emplace(con.csInputId, std::unordered_map<int, int>());
		}

		graph.at(con.csInputId).emplace(con.csOutputId,
			mapPipe.at(con.pipeId).getLength());
		inVerts.insert(con.csInputId);
		inVerts.insert(con.csOutputId);
	}

	for (auto& vert : inVerts)
	{
		if (!graph.contains(vert))
		{
			graph.emplace(vert, std::unordered_map<int, int>());
		}

		for (auto& outVert : inVerts)
		{
			if (!graph.at(vert).contains(outVert))
			{
				graph.at(vert).emplace(outVert, 0);
			}
		}
	}
}

float calculateFlow(float length, int diameter)
{
	if (length == 0)
	{
		return 0;
	}

	return sqrt(float(pow(diameter, 5)) / length);
}

void convertToFlowMatrix(const std::vector<Connection>& arrCon,
	const std::unordered_map<int, Pipe>& mapPipe, capacityMatrix& graph)
{
	if (arrCon.empty())
	{
		return;
	}

	std::unordered_set<int> inVerts;

	for (const auto& con : arrCon)
	{
		if (!graph.contains(con.csInputId))
		{
			graph.emplace(con.csInputId, std::unordered_map<int, float>());
		}

		graph.at(con.csInputId).emplace(con.csOutputId, calculateFlow
			(mapPipe.at(con.pipeId).getLength(),
			mapPipe.at(con.pipeId).getDiameter()));

		inVerts.insert(con.csInputId);
		inVerts.insert(con.csOutputId);
	}

	for (auto& vert : inVerts)
	{
		if (!graph.contains(vert))
		{
			graph.emplace(vert, std::unordered_map<int, float>());
		}

		for (auto& outVert : inVerts)
		{
			if (!graph.at(vert).contains(outVert))
			{
				graph.at(vert).emplace(outVert, 0);
			}
		}
	}
}


bool dfs(int vert, graphMatrix& graph, 
	std::unordered_map<int, int>& visited, std::vector<int>& result)
{
	visited.at(vert) = 1;

	for (auto& [outVert, status] : graph.at(vert))
	{
		if (status == 0)
		{
			continue;
		}

		if (visited[outVert] == 0)
		{  
			if (dfs(outVert, graph, visited, result))
			{
				return true;  
			}
		}
		else if (visited[outVert] == 1)
		{  
			return true;
		}
	}

	visited[vert] = 2;
	result.push_back(vert);
	return false;
}

void restorePath(graphMatrix& graph, const std::unordered_map<int, int>& distances,
	int minPath, std::vector<int>& path, int currentVert)
{
	if (minPath == 0)
	{
		return;
	}

	for (auto& [inVert, outVerts] : graph)
	{
		if (outVerts.at(currentVert) == 0)
		{
			continue;
		}

		if (minPath - outVerts.at(currentVert) == distances.at(inVert))
		{
			path.push_back(inVert);
			minPath -= outVerts.at(currentVert);
			restorePath(graph, distances, minPath, path, inVert);

			if (minPath == 0)
			{
				return;
			}
		}
	}
}

void findShortestPath(const std::vector<Connection>& arrCon,
	const std::unordered_map<int, Pipe>& mapPipe)
{
	if (arrCon.empty())
	{
		return;
	}

	int startVert, endVert;

	graphMatrix graph;
	convertToMatrix(arrCon, mapPipe, graph);

	cout << "Enter the ID of the CS that is the beginning of the path: ";

	while (true)
	{
		checkInput(startVert, 0, INT_MAX);

		if (!graph.contains(startVert))
		{
			cout << "There is no CS with the specified ID in the pipeline!" << endl;
			cout << "Try again: ";

			continue;
		}

		break;
	}

	cout << "Enter the ID of the CS that is the end of the path: ";

	while (true)
	{
		checkInput(endVert, 0, INT_MAX);

		if (!graph.contains(endVert) || endVert == startVert)
		{
			cout << "There is no CS with the specified ID in the pipeline!" << endl;
			cout << "Try again: ";

			continue;
		}

		break;
	}

	std::unordered_map<int, int> distances;

	for (const auto& [inVert, outVerts] : graph)
	{
		distances.emplace(inVert, (startVert == inVert ? 0 : INT_MAX));
	}

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
	std::greater<std::pair<int, int>>> usingVerts;
	usingVerts.push({0, startVert});

	int currentDistance;
	int currentVert;

	while (!usingVerts.empty()) 
	{
		currentDistance = usingVerts.top().first;
		currentVert = usingVerts.top().second;
		usingVerts.pop();

		if (currentDistance > distances[currentVert])
		{
			continue;
		}

		for (const auto& [vert, outVerts] : graph)
		{
			if (graph.at(currentVert).at(vert) != 0 && 
				distances.at(vert) > distances.at(currentVert) + graph.at(currentVert).at(vert))
			{
				distances.at(vert) = distances.at(currentVert) + graph.at(currentVert).at(vert);
				usingVerts.push({distances.at(vert), vert});
			}
		}

		if (currentVert == endVert)
		{
			break;
		}
	}

	if (distances.at(endVert) == INT_MAX)
	{
		cout << "It is impossible to pave the specified path!" << endl;
	}
	else
	{
		std::vector<int> path;
		path.push_back(endVert);
		restorePath(graph, distances, distances.at(endVert), path, endVert);
		std::reverse(path.begin(), path.end());
		
		cout << "Shortest distance from CS " << startVert << " to CS "
			<< endVert << ": ";
		cout << distances.at(endVert) << "m" << endl;
		cout << "Path: ";

		for (const auto& vert : path)
		{
			cout << vert << " ";
		}
		cout << endl;
	}
}


void topologicalSort(const std::vector<Connection>& arrCon, 
	const std::unordered_map<int, Pipe>& mapPipe)
{
	if (arrCon.empty())
	{
		return;
	}

	graphMatrix graph;
	convertToMatrix(arrCon, mapPipe, graph);

	int n = graph.size();
	std::unordered_map<int, int> visited;
	
	int workVert;
	std::vector<int> topSort, result;

	for (auto& [inVert, outVerts] : graph)
	{
		visited.emplace(inVert, 0);

		if (visited.at(inVert) == 0 && dfs(inVert, graph, visited, topSort))
		{
			cout << "A cycle in a graph!" << endl;
			return;
		}

		topSort.clear();
	}
	
	cout << "Enter the ID of the initial CS for topological sorting: ";

	while (true)
	{
		checkInput(workVert, 0, INT_MAX);
		
		if (!graph.contains(workVert))
		{
			cout << "There is no CS with the specified ID in the pipeline!" << endl;
			cout << "Try again: ";

			continue;
		}

		break;
	}
	

	for (auto& [vert, status] : visited)
	{
		status = 0;
	}

	dfs(workVert, graph, visited, result);
	std::reverse(result.begin(), result.end());

	cout << "Topological sorting: " << endl;
	for (int vert : result) 
	{
		cout << vert << " ";
	}
	cout << endl;
}

bool findFlow(capacityMatrix& graph, capacityMatrix& flow,
	std::unordered_map<int, int>& parent, int source, int stock)
{
	std::queue<int> q;
	q.push(source);

	int workVert;

	while (!q.empty())
	{
		workVert = q.front();
		q.pop();

		for (auto& [inVert, outVerts] : graph)
		{
			if (inVert != workVert && parent.at(inVert) == -1 &&
				graph.at(workVert).at(inVert) - flow.at(workVert).at(inVert) > 0)
			{
				parent.at(inVert) = workVert;

				if (inVert == stock)
				{
					return true;
				}

				q.push(inVert);
			}
		}
	}

	return false;
}

void findMaxFlow(const std::vector<Connection>& arrCon,
	const std::unordered_map<int, Pipe>& mapPipe)
{
	if (arrCon.empty())
	{
		return;
	}

	int source, stock;

	capacityMatrix graph;
	capacityMatrix flow;
	convertToFlowMatrix(arrCon, mapPipe, graph);

	cout << "Enter the ID CS to start (source): ";

	while (true)
	{
		checkInput(source, 0, INT_MAX);

		if (!graph.contains(source))
		{
			cout << "There is no CS with the specified ID in the pipeline!" << endl;
			cout << "Try again: ";

			continue;
		}

		break;
	}

	cout << "Enter the ID CS at the end: ";

	while (true)
	{
		checkInput(stock, 0, INT_MAX);

		if (!graph.contains(stock) || stock == source)
		{
			cout << "There is no CS with the specified ID in the pipeline!" << endl;
			cout << "Try again: ";

			continue;
		}

		break;
	}

	float maxFlow = 0;
	std::unordered_map<int, int> parent;

	for (auto& [inVert, outVerts] : graph)
	{
		flow.emplace(inVert, outVerts);
		parent.emplace(inVert, -1);

		for (auto& [outVert, status] : flow.at(inVert))
		{
			if (outVert == source)
			{
				status = FLT_MAX;
			}
			else
			{
				status = 0;
			}
		}
	}

	float pathFlow;
	int workVert;
	
	while (findFlow(graph, flow, parent, source, stock))
	{
		pathFlow = FLT_MAX;

		for (int i = stock; source != i; i = parent[i])
		{
			workVert = parent.at(i);
			pathFlow = std::min(pathFlow, graph.at(workVert).at(i) - flow.at(workVert).at(i));
		}

		for (int i = stock; i != source; i = parent[i])
		{
			workVert = parent.at(i);
			flow.at(workVert).at(i) += pathFlow;

			if (flow.contains(i) && flow.at(i).contains(workVert))
			{
				flow[i][workVert] -= pathFlow;
			}
		}

		maxFlow += pathFlow;
	}

	cout << "Maximum flow: " << maxFlow << endl;
}


void addPipe(std::unordered_map<int, Pipe>& map)
{
	Pipe p;
	cin >> p;

	map.emplace(p.getID(), p);
}

void addCS(std::unordered_map<int, CS>& map)
{
	CS cs;
	cin >> cs;

	map.emplace(cs.getID(), cs);
}

void printPipes(const std::unordered_map<int, Pipe>& map)
{
	cout << "Pipes:" << endl << endl;

	if (map.empty())
	{
		cout << "Pipes are missing" << endl << endl;
	}
	else
	{
		for (const auto& p : map)
		{
			cout << p.second;
		}
	}

	cout << endl;
}

void printCS(const std::unordered_map<int, CS>& map)
{
	cout << "CS:" << endl << endl;

	if (map.empty())
	{
		cout << "CS are missing" << endl << endl;
	}
	else
	{
		for (const auto& cs : map)
		{
			cout << cs.second;
		}
	}

	cout << endl;
}

void printAll(const std::unordered_map<int, Pipe>& mapPipe,
	 const std::unordered_map<int, CS>& mapCS)
{
	printPipes(mapPipe);
	printCS(mapCS);
}

void editObjects(std::unordered_map<int, Pipe>& map, std::unordered_set<int>& set)
{
	for (auto& id : set)
	{
		editPipe(map.at(id));
	}
}

void editObjects(std::unordered_map<int, CS>& map, std::unordered_set<int>& set)
{
	for (auto& id : set)
	{
		editCS(map.at(id));
	}
}

void deleteObjects(std::unordered_map<int, Pipe>& map, std::unordered_set<int>& set)
{
	for (auto& id : set)
	{
		if (map.at(id).isInConnection)
		{
			cout << "Pipe " << id << " is in pipeline!"
				<< " first, delete connection!" << endl;
			continue;
		}

		map.erase(id);
		cout << "Pipe " << id << " deleted" << endl;
	}

	cout << endl;
}

void deleteObjects(std::unordered_map<int, CS>& map, std::unordered_set<int>& set)
{
	for (auto& id : set)
	{
		if (map.at(id).isInConnection)
		{
			cout << "CS " << id << " is in pipeline!"
				<< " first, delete connection!" << endl;
			continue;
		}

		map.erase(id);
		cout << "CS " << id << " deleted" << endl;
	}

	cout << endl;
}

std::unordered_set<int> filterIntersection(std::unordered_set<int>& oldSet,
	std::unordered_set<int>& newSet)
{
	std::unordered_set<int> set;

	if (oldSet.empty())
	{
		set = newSet;
	}
	else
	{
		std::set_intersection(oldSet.begin(), oldSet.end(), newSet.begin(), newSet.end(),
			std::inserter(set, set.begin()));
	}

	if (set.empty())
	{
		std::cout << "No objects matching the filter were found!"
			<< std::endl << std::endl;
	}
	else
	{
		std::cout << "ID of the found items: ";

		for (int id : set)
		{
			std::cout << id << " ";
		}

		std::cout << std::endl << std::endl;
	}

	oldSet.clear();
	newSet.clear();

	return set;
}

std::unordered_set<int> makeSetOfFilteredPipes(const std::unordered_map<int, Pipe>& map)
{
	int filtType;
	std::unordered_set<int> set, oldSet, newSet;

	std::string name;
	int status;

	printPipes(map);

	cout << endl;
	cout << "List of filters: " << endl;
	cout << "1 - filter by name" << endl 
		<< "2 - based on under repair status" << endl << endl;

	while (true)
	{
		cout << "Enter the filter number (0 if you want to finish filtering):  ";

		checkInput(filtType, 0, 2);

		if (!filtType)
		{
			cout << endl;
			break;
		}

		if (filtType == 1)
		{
			cout << "Enter the name for the filter: ";
			cin >> std::ws;
			inputLine(name, cin);

			filtObjects(map, newSet, filtByName, name);
		}
		else if (filtType == 2)
		{
			cout << "Enter the status (0 - status under repair, 1 - status working) ";
			checkInput(status, 0, 1);

			filtObjects(map, newSet, filtByRepairingFlag, !bool(status));
		}

		oldSet.insert(set.begin(), set.end());
		set.clear();
		set = filterIntersection(oldSet, newSet);
	}

	return set;
}

std::unordered_set<int> makeSetOfFilteredCS(const std::unordered_map<int, CS>& map)
{
	int filtType;
	std::unordered_set<int> set, oldSet, newSet;

	std::string name;
	int percent;
	int comparePercent;

	printCS(map);

	cout << endl;
	cout << "List of filters:: " << endl;
	cout << "1 - filter by name" << endl
		<< "2 - by percentage of unused workshops" << endl << endl;

	while (true)
	{
		cout << "Enter the filter number (0 if you want to finish filtering): ";
		checkInput(filtType, 0, 2);

		if (!filtType)
		{
			cout << endl;
			break;
		}

		if (filtType == 1)
		{
			cout << "Enter the name for the filter:  ";
			cin >> std::ws;
			inputLine(name, cin);

			filtObjects(map, newSet, filtByName, name);
		}
		else if (filtType == 2)
		{
			cout << "Enter the percentage of unused workshops: ";
			checkInput(percent, 0, 100);

			cout << "Enter 1 to find the CS with a percentage greater than the entered one, "
				<< "2 - less than the entered one, 3 - equal to the entered one: ";
			checkInput(comparePercent, 1, 3);

			if (comparePercent == 1)
			{
				filtObjects(map, newSet, filtByGuildUpperPercent, percent);
			}
			else if (comparePercent == 2)
			{
				filtObjects(map, newSet, filtByGuildLowerPercent, percent);
			}
			else if (comparePercent == 3)
			{
				filtObjects(map, newSet, filtByGuildEqualPercent, percent);
			}
		}

		oldSet.insert(set.begin(), set.end());
		set.clear();
		set = filterIntersection(oldSet, newSet);
	}

	return set;
}




