#pragma once
#include <iostream>
#include <string>
#include <fstream>

#include "inputcheck.h"

const std::vector<int> pipeDiameters = { 500, 700, 1000, 1400 };

class Pipe
{
public:
	static int idCount;
	bool isInConnection = false;

	int getID();
	float getLength() const;
	int getDiameter() const;
	int getStatus() const;

	friend void editPipe(Pipe& p);
	friend void createPipeWithGivenDiameter(Pipe& p, int diameter);
	friend bool filtByName(const Pipe& p, std::string name);
	friend bool filtByRepairingFlag(const Pipe& p, bool type);
	friend bool filtByDiameter(const Pipe& p, int d);

	friend std::istream& operator >> (std::istream& in, Pipe& p);
	friend std::ostream& operator << (std::ostream& out, const Pipe& p);
	friend std::ifstream& operator >> (std::ifstream& in, Pipe& p);
	friend std::ofstream& operator << (std::ofstream& out, const Pipe& p);

private:

	int id;
	std::string kmMark;
	float length;
	int diameter;
	bool isUnderRepair;
};

void editPipe(Pipe& p);
void createPipeWithGivenDiameter(Pipe& p, int diameter);

bool filtByName(const Pipe& p, std::string name);
bool filtByRepairingFlag(const Pipe& p, bool type);
bool filtByDiameter(const Pipe& p, int d);
