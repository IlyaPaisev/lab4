#include <iostream>
#include <fstream>

#include "Pipe.h"
#include "Get_Correct.cpp"

int Pipe::Nextid = 1;

Pipe::Pipe()
{
    id = Nextid++;
}

void Pipe::read() {
    std::cout << "Enter the pipe name: ";
    name = get_str();
    std::cout << "Enter the length of the pipe (in km): ";
    length = get_correct_value<int>(1, INT_MAX);
    diameter = get_correct_diameter();
    under_repair = false;
    id_cs_of_the_entrance = 0;
    id_cs_of_the_exit = 0;
}

std::ostream& operator << (std::ostream& out, const Pipe& p){
    out << "ID: " << p.id << "\n";
    out << "Name: " << p.name << "\n";
    out << "Length (km): " << p.length << "\n";
    out << "Diameter: " << p.diameter << "\n";
    out << "Under repair: " << (p.under_repair ? "Yes" : "No") << "\n";
    out << "ID_cs_of_the_Entrance: " << p.id_cs_of_the_entrance << "\n";
    out << "ID_cs_of_the_Exit: " << p.id_cs_of_the_exit << "\n";
    return out;
}

void Pipe::toggle_repair() {
    under_repair = !under_repair;
}

void Pipe::connecting_with_cs(int id_from, int id_to){
    id_cs_of_the_entrance = id_from;
    id_cs_of_the_exit = id_to;
}

void Pipe::save_data(std::ofstream& out) {
    if (out.is_open()) {
        out << "Pipe\n";
        out<< id << "\n";
        out << name << "\n";
        out << length << "\n";
        out << diameter << "\n";
        out << under_repair << "\n";
        out << id_cs_of_the_entrance << "\n";
        out << id_cs_of_the_exit << "\n";
    }
}

void Pipe::load_data(std::ifstream& read) {
    if (read.is_open()) {
        read >> id;
        read >> name;
        read >> length;
        read >> diameter;
        read >> under_repair;
        read >> id_cs_of_the_entrance;
        read >> id_cs_of_the_exit;
    }
    else {
        std::cerr << "Error!";
    }
}