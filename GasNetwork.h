#pragma once
#include <unordered_map>
#include <unordered_set>

#include "Pipe.h"
#include "CompressorStation.h"

class GasNetwork {
private:
    unordered_map<int, Pipe> pipes;
    unordered_map<int, CompressorStation> stations;
public:
    inline unordered_map<int, Pipe> getpipes() { return pipes;};
    inline unordered_map<int, CompressorStation> getcs() { return stations;};
    
    void connectPipesToStations();
    void print_menu();
    void add_pipe(Pipe pipe);
    void add_station(CompressorStation station);
    void print_pipe_and_cs();
    void edit_pipe();
    void edit_cs();
    void save_file();
    void load_file();
    void delete_pipe();
    void delete_cs();
};