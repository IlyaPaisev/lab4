#include <iostream>
#include <fstream>

#include "CompressorStation.h"
#include "Get_Correct.cpp"

int CompressorStation::Nextid = 1;

CompressorStation::CompressorStation()
{
    id = Nextid++;
}

void CompressorStation::read() {
    std::cout << "Enter the compressor station name: ";
    name = get_str();
    std::cout << "Enter the number of workshops: ";
    num_workshops = get_correct_value<int>(0, INT_MAX);
    std::cout << "Enter the number of workshops in operation: ";
    num_workshops_in_operation = get_correct_value<int>(0, num_workshops);
    std::cout << "Enter the efficiency (0 to 100): ";
    efficiency = get_correct_value<double>(0.0, 100.0);
}

std::ostream& operator << (std::ostream& out, const CompressorStation& cs){
    out << "Id: " << cs.id << "\n";
    out << "Name: " << cs.name << "\n";
    out << "Number of workshops: " << cs.num_workshops << "\n";
    out << "Number of workshops in operation: " << cs.num_workshops_in_operation << "\n";
    out << "Efficiency: " << cs.efficiency << "\n";
    return out;
}

void CompressorStation::edit() {
    std::cout << "Enter the new number of workshops in operation: ";
    num_workshops_in_operation = get_correct_value<int>(0, num_workshops);
}

void CompressorStation::save_data(std::ofstream& out) {
    if (out.is_open()) {
        out << "Compressor Station\n";
        out << id << "\n";
        out << name << "\n";
        out << num_workshops << "\n";
        out << num_workshops_in_operation << "\n";
        out << efficiency << "\n";
    }
}

void CompressorStation::load_data(std::ifstream& read) {
    if (read.is_open()) {
        read >> id;
        read >> name;
        read >> num_workshops;
        read >> num_workshops_in_operation;
        read >> efficiency;
    }
    else {
        std::cout << "Error!";
    }
}