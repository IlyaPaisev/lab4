#pragma once
#include <fstream>

class CompressorStation {
private:
    int id;
    static int Nextid;
public:
    inline int getid() const { return id; };
    
    std::string name;
    int num_workshops;
    int num_workshops_in_operation;
    double efficiency;
    int number_of_connected_pipes;
    
    CompressorStation();
    void read();
    void edit();
    void save_data(std::ofstream& out);
    void load_data(std::ifstream& read);
    friend std::ostream& operator << (std::ostream& out, const CompressorStation& cs);
};