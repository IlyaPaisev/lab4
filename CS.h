#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "inputcheck.h"

class CS
{
public:
	static int idCount;
	bool isInConnection = false;

	int getID();

	friend void editCS(CS& cs);
	friend bool filtByName(const CS& cs, std::string name);
	friend bool filtByGuildLowerPercent(const CS& cs, int percentage);
	friend bool filtByGuildUpperPercent(const CS& cs, int percentage);
	friend bool filtByGuildEqualPercent(const CS& cs, int percentage);

	friend std::istream& operator >> (std::istream& in, CS& cs);
	friend std::ostream& operator << (std::ostream& out, const CS& cs);
	friend std::ifstream& operator >> (std::ifstream& in, CS& cs);
	friend std::ofstream& operator << (std::ofstream& out, const CS& cs);

private:

	int id;
	std::string name;
	int guildCount;
	int guildCountInWork;
	float efficiency;
};

void editCS(CS& cs);


bool filtByName(const CS& cs, std::string name);
bool filtByGuildLowerPercent(const CS& cs, int percentage);
bool filtByGuildUpperPercent(const CS& cs, int percentage);
bool filtByGuildEqualPercent(const CS& cs, int percentage);
