#pragma once
#include <iostream>
#include <string>

class RedirectOutputWrapper
{
	std::ostream& stream;
	std::streambuf* const oldBuf;
public:
	RedirectOutputWrapper(std::ostream& src)
		:oldBuf(src.rdbuf()), stream(src)
	{
	}

	~RedirectOutputWrapper() {
		stream.rdbuf(oldBuf);
	}
	
	void redirect(std::ostream& dest)
	{
		stream.rdbuf(dest.rdbuf());
	}
};

void inputLine(std::string& str, std::istream& in);

template <typename T>
void checkInput(T& var, T left, T right,
	std::istream& in = std::cin)
{
	std::string type;

	while (!(in >> var) || (in.peek() != '\n') || (var < left || var > right))
	{
		std::cout << "Error!" << std::endl;
		std::cout << "Try again: ";
	
		in.clear();
		in.ignore(10000, '\n');
		continue;
	}

	std::cin.get();
	std::cerr << var << std::endl;
}
