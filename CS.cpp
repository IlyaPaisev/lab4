#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "inputcheck.h"
#include "GasNetwork.h"
#include "CS.h"

using std::ofstream;
using std::ifstream;
using std::string;
using std::cout;
using std::cin;
using std::endl;

int CS::idCount = 0;

std::istream& operator>>(std::istream& in, CS& cs)
{
	cs.id = CS::idCount;
	++CS::idCount;

	cout << "Enter the CS name: ";
	cin >> std::ws;
	inputLine(cs.name, in);

	cout << "Enter the number of workshops: ";
	checkInput(cs.guildCount, 1, INT_MAX, in);

	cout << "Enter the number of workshops in operation: ";
	checkInput(cs.guildCountInWork, 0, cs.guildCount, in);

	cout << "Enter the CS efficiency: ";
	checkInput(cs.efficiency, float(-100), float(100), in);

	cout << endl << "CS has been initialized!" << endl;

	return in;
}

std::ostream& operator<<(std::ostream& out, const CS& cs)
{
	out << "CS ID: " << cs.id << endl;
	out << "Name of the CS: " << cs.name << endl;
	out << "Number of workshops: " << cs.guildCount << endl;
	out << "Number of workshops in operation: " << cs.guildCountInWork << endl;
	out << "Efficiency coefficient: " << cs.efficiency << endl;
	out << "Participates in the gas pipeline: " << (cs.isInConnection ? "Yes" : "No") << endl;
	out << endl;

	return out;
}

std::ifstream& operator>>(std::ifstream& in, CS& cs)
{
	string name;
	in >> std::ws;
	std::getline(in, name);

	int guildCount, guildCountInWork, id;
	float efficiency;
	bool isInConnection;

	in >> id >> guildCount >> guildCountInWork >> efficiency >> isInConnection;

	if (name == "" || in.fail() || id < 0
		|| guildCount < 0 || guildCountInWork < 0 ||
		guildCount < guildCountInWork)
	{
		cs.id = -1;
		return in;
	}

	cs.id = id;
	cs.name = name;
	cs.guildCount = guildCount;
	cs.guildCountInWork = guildCountInWork;
	cs.efficiency = efficiency;
	cs.isInConnection = isInConnection;

	return in;
}

std::ofstream& operator<<(std::ofstream& out, const CS& cs)
{
	if (cs.name == "")
	{
		return out;
	}

	out << 'C' << endl;
	out << cs.name << endl;
	out << cs.id << " ";
	out << cs.guildCount << " ";
	out << cs.guildCountInWork << " ";
	out << cs.efficiency << " ";
	out << cs.isInConnection << endl;

	return out;
}

int CS::getID()
{
	return id;
}

void editCS(CS& cs)
{
	int increase;

	cout << "CS ID: " << cs.id << endl;
	cout << "Number of workshops: " << cs.guildCount << endl;
	cout << "Number of workshops in operation: " << cs.guildCountInWork << endl;
	cout << "Add workshop - 1, remove - 0" << endl;

	cout << "Enter the number: ";
	checkInput(increase, 0, 1);

	if (increase)
	{
		if (cs.guildCount == cs.guildCountInWork)
		{
			cout << "The number of workshops has been exceeded!" << endl << endl;
			return;
		}

		++cs.guildCountInWork;
	}
	else
	{
		if (cs.guildCountInWork == 0)
		{
			cout << "The number of workshops is zero!" << endl << endl;
			return;
		}

		--cs.guildCountInWork;
	}

	cout << "Current number of workshops: " << cs.guildCountInWork
		<< endl << endl;
}

bool filtByName(const CS& cs, std::string name)
{
	return cs.name.find(name) != string::npos;
}
bool filtByGuildUpperPercent(const CS& cs, int percent)
{
	return float(cs.guildCount - cs.guildCountInWork)
		/ cs.guildCount * 100 > percent;
}
bool filtByGuildEqualPercent(const CS& cs, int percent)
{
	return float(cs.guildCount - cs.guildCountInWork)
		/ cs.guildCount * 100 == percent;
}
bool filtByGuildLowerPercent(const CS& cs, int percent)
{
	return float(cs.guildCount - cs.guildCountInWork)
		/ cs.guildCount * 100 < percent;
}
