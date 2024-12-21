#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>
#include "inputcheck.h"
#include "GasNetwork.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::unordered_map;



void printHelp()
{
	cout << "Instructions:" << endl << endl;
	cout << "1 - Add a pipe" << endl;
	cout << "2 - Add CS" << endl;
	cout << "3 - Actions with pipes" << endl;
	cout << "4 - Actions with CS" << endl;
	cout << "5 - View all pipes and CS" << endl;
	cout << "6 - Create connection (add to gas pipeline)" << endl;
	cout << "7 - Show the entire gas pipeline" << endl;
	cout << "8 - Delete connection" << endl;
	cout << "9 - Topological sorting" << endl;
	cout << "10 - Find the shortest path" << endl;
	cout << "11 - Find the maximum flow in the network" << endl;
	cout << "12 - Save" << endl;
	cout << "13 - Upload" << endl;

	cout << "0 - Exit" << endl << endl;
}


int main()
{
	setlocale(LC_ALL, "ru");

	RedirectOutputWrapper cerrOut(std::cerr);
	string time = std::format("{:%d_%m_%Y_%H_%M_%OS}",std::chrono::system_clock::now());
	std::ofstream logfile("logs\\log_" + time + ".txt");

	if (logfile)
		cerrOut.redirect(logfile);

	printHelp();

	int choice;

	unordered_map<int, CS> mapCS;
	unordered_map<int, Pipe> mapPipe;
	std::vector<Connection> arrConnection;
	
	while (true)
	{
		cout << "Enter the number: ";
		checkInput(choice, 0, 13);

		system("cls");

		switch (choice)
		{
		case 1:
			addPipe(mapPipe);
			break;

		case 2:
			addCS(mapCS);
			break;

		case 3:
			chooseAction(mapPipe);
			break;

		case 4:
			chooseAction(mapCS);
			break;

		case 5:
			printAll(mapPipe, mapCS);
			break;

		case 6:
			addConnection(arrConnection, mapPipe, mapCS);
			break;
		
		case 7:
			printConnection(arrConnection, mapPipe);
			break;

		case 8:
			deleteConnection(arrConnection, mapPipe, mapCS);
			break;

		case 9:
			printConnection(arrConnection, mapPipe);
			topologicalSort(arrConnection, mapPipe);
			break;

		case 10:
			printConnection(arrConnection, mapPipe);
			findShortestPath(arrConnection, mapPipe);
			break;

		case 11:
			printConnection(arrConnection, mapPipe);
			findMaxFlow(arrConnection, mapPipe);
			break;

		case 12:
			saveObjects(mapPipe, mapCS, arrConnection);
			break;

		case 13:
			loadObjects(mapPipe, mapCS, arrConnection);
			break;

		case 0:
			cout << "See you soon!" << endl;
			return 0;
		}

		cout << "Press Enter...";

		cin.ignore(10000, '\n');
		std::cerr << endl;
		system("cls");

		printHelp();
	}

	return 0;
}
