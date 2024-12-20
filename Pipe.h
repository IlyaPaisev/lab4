#pragma once
#include <fstream>

class Pipe {
private:
    int id;
    static int Nextid;
    int id_cs_of_the_entrance;
    int id_cs_of_the_exit;

public:
    inline int getid() const { return id; };
    inline int getIdCSOfTheEntrance() const { return id_cs_of_the_entrance; };
    inline int getIdCSOfTheExit() const { return id_cs_of_the_exit; };
    
    std::string name;
    int length;
    int diameter;
    bool under_repair;

    Pipe();
    void read();
    void toggle_repair();
    void connecting_with_cs(int id_from, int id_to);
    void save_data(std::ofstream& out);
    void load_data(std::ifstream& read);
    friend std::ostream& operator << (std::ostream& out, const Pipe& p);
};