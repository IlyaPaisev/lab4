#include <iostream>
#include <fstream>
#include <format>
#include <chrono>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "Pipe.h"
#include "CompressorStation.h"
#include "Logging.h"
#include "Get_Correct.cpp"
#include "Graph.h"
#include "GasNetwork.h"

using namespace std;
using namespace chrono;

template<typename T1, typename T2>
using filter = bool(*)(const T1& dict, T2 param);

template<typename T>
bool filter_by_name(const T& dict, string name) {
    return dict.name.find(name) != string::npos;
}

bool filter_by_status(const Pipe& dict, bool status) {
    return dict.under_repair == status;
}

bool filter_by_non_working(const CompressorStation& dict, int non_working) {
    return dict.num_workshops - dict.num_workshops_in_operation >= dict.num_workshops * non_working / 100;
}

template <typename T1, typename T2>
unordered_set<int> find_by_filter(const unordered_map<int, T1>& dict, filter<T1, T2> f, T2 par) {
    unordered_set<int> ids;
    for (auto& pair : dict) {
        if (f(pair.second, par)) {
            ids.insert(pair.first);
        }
    }
    return ids;
}

template <typename F>
void display_id(unordered_map<int, F> dict, int id) {
    for (auto const& pair : dict) {
        if (id == pair.first) {
            cout << pair.second << endl;
        }
    }
}

template <typename T>
void edit_multiple_items(unordered_map<int, T> dict, const unordered_set<int>& ids) {
    for (int id : ids) {
        cout << "Are you sure you want to edit the pipe:\n";
        display_id(dict, id);
        cout << "0-No 1-Yes\n";
        if(get_correct_value(0, 1)){
            T& item = dict[id];
            item.toggle_repair();
        }
    }
    cout << "Editing completed for the selected items." << endl;
}

template <typename T>
void delete_multiple_items(unordered_map<int, T> dict, const unordered_set<int>& ids) {
    for (int id : ids) {
        cout << "Are you sure you want to delete the pipe:\n";
        display_id(dict, id);
        cout << "0-No 1-Yes\n";
        if(get_correct_value(0, 1)){
            dict.erase(id);
        }
    }
    cout << "Deletion completed for the selected items." << endl;
}


void filter_by_pipes(GasNetwork net){
    if (net.getpipes().size() != 0){
        cout << "0 - By the 'under repair' status \n1 - By pipe name\nChoose by which filter you want to filter: ";
        if (get_correct_value(0, 1)) {
            cout << "Enter the name of the pipes you want to find: ";
            string name = get_str();
            for (int i : find_by_filter(net.getpipes(), filter_by_name, name)) {
                display_id(net.getpipes(), i);
            }
        }else {
            cout << "0 - pipe not under repair\n1 - pipe under repair\nEnter the number: ";
            bool under_repair = get_correct_value(0, 1);
            for (int i : find_by_filter(net.getpipes(), filter_by_status, under_repair)) {
                display_id(net.getpipes(), i);
            }
        }
    }else{
        cout << "No data\n";
    }
}

void filter_by_cs(GasNetwork net){
    if(net.getcs().size() != 0){
        cout << "0 - By the percentage of unused workshops\n1- By the name of compressor stations\nChoose by which filter you want to filter: ";
        if (get_correct_value(0, 1)) {
            cout << "Enter the name of the compressor station you want to find: ";
            string name = get_str();
            for (int i : find_by_filter(net.getcs(), filter_by_name, name)) {
                display_id(net.getcs(), i);
            }
        }else {
            cout << "Enter the efficiency of the compressor stations you want to find: ";
            int non_working = get_correct_value(0, 100);
            for (int i : find_by_filter(net.getcs(), filter_by_non_working, non_working)) {
                display_id(net.getcs(), i);
            }
        }
    }else{
        cout << "No data\n";
    }
}

void batch_editing_pipes(GasNetwork net){
    if (net.getpipes().size() != 0){
        cout << "0 - By 'under repair' status\n1 - By pipe name\nChoose the criterion for selecting pipes for batch editing: ";
        int filter_choice = get_correct_value<int>(0, 1);
        
        if (filter_choice == 0) {
            cout << "0 - Not under repair\n1 - Under repair\nChoose the pipe status: ";
            bool under_repair = get_correct_value(0, 1);
            unordered_set<int> selected_pipes = find_by_filter(net.getpipes(), filter_by_status, under_repair);
            edit_multiple_items(net.getpipes(), selected_pipes);
        } else if (filter_choice == 1) {
            cout << "Enter the pipe name for search: ";
            string name = get_str();
            unordered_set<int> selected_pipes = find_by_filter(net.getpipes(), filter_by_name, name);
            edit_multiple_items(net.getpipes(), selected_pipes);
        }
    }else{
        cout << "No data\n";
    }
}

void batch_deletion_pipes(GasNetwork net){
    if (net.getpipes().size() != 0){
        cout << "0 - By 'under repair' status\n1 - By pipe name\nChoose the criterion for selecting pipes for batch deletion: ";
        int filter_choice = get_correct_value<int>(0, 1);
        
        if (filter_choice == 0) {
            cout << "0 - Not under repair\n1 - Under repair\nChoose the pipe status: ";
            bool under_repair = get_correct_value(0, 1);
            unordered_set<int> selected_pipes = find_by_filter(net.getpipes(), filter_by_status, under_repair);
            delete_multiple_items(net.getpipes(), selected_pipes);
        } else if (filter_choice == 1) {
            cout << "Enter the pipe name for search: ";
            string name = get_str();
            unordered_set<int> selected_pipes = find_by_filter(net.getpipes(), filter_by_name, name);
            delete_multiple_items(net.getpipes(), selected_pipes);
        }
    }else{
        cout << "No data\n";
    }
}

int main() {
    redirect_output_wrapper cerr_out(cerr);
    ofstream logfile("log.txt");
    if (logfile) {
        cerr_out.redirect(logfile);
    }
    
    GasNetwork net;
    
    while (true) {
        net.print_menu();

        int choice;
        cout << "\nEnter a number from 0 to 17 to perform the corresponding action: ";
        choice = get_correct_value<int>(0, 17);
        switch (choice) {
            case 0:
                exit(0);
                break;
            case 1: {
                Pipe pipe;
                net.add_pipe(pipe);
                break;
            }
            case 2: {
                CompressorStation station;
                net.add_station(station);
                break;
            }
            case 3: {
                net.print_pipe_and_cs();
                break;
            }
            case 4: {
                net.edit_pipe();
                break;
            }
            case 5: {
                net.edit_cs();
                break;
            }
            case 6: {
                net.save_file();
                break;
            }
            case 7: {
                net.load_file();
                break;
            }
            case 8: {
                net.delete_pipe();
                break;
            }
            case 9: {
                net.delete_cs();
                break;
            }
            case 10:{
                filter_by_pipes(net);
                break;
            }
            case 11:{
                filter_by_cs(net);
                break;
            }
            case 12: {
                batch_editing_pipes(net);
                break;
            }
            case 13: {
                batch_deletion_pipes(net);
                break;
            }
            case 14: {
                net.connectPipesToStations();
                break;
            }
            case 15: {
                Graph gasGraph = buildGraph(net.getpipes(), net.getcs());

                std::vector<int> sortedNodes = topologicalSort(gasGraph);
                if (sortedNodes.size() != net.getcs().size()){
                    cout << "There is a cycle in the graph" << endl;
                }else{
                    cout << "Topological sorting result:" << endl;
                    for (int node : sortedNodes) {
                        std::cout << node << " ";
                    }
                    cout << endl;
                }
                break;
            }
            case 16: {
                Graph gasGraph = buildGraph(net.getpipes(), net.getcs());
                
                int sourceID = get_valid_id("Enter source vertex ID: ", net.getcs());
                int sinkID = get_valid_id("Enter sink vertex ID: ", net.getcs());

                double maxFlow = fordFulkerson(gasGraph, sourceID, sinkID);
                cout << "Maximum flow: " << maxFlow << endl;
                break;
            }
            case 17: {
                Graph gasGraph = buildGraph(net.getpipes(), net.getcs());
                
                int startStation = get_valid_id("Enter the ID of the initial compressor station: ", net.getcs());
                int endStation = get_valid_id("Enter the ID of the final compressor station: ", net.getcs());

                std::vector<int> path = shortestPath(gasGraph, startStation, endStation);

                if (path.empty()) {
                    cout << "The shortest path between the stations has not been found." << endl;
                } else {
                    cout << "The shortest way: ";
                    for (size_t i = 0; i < path.size(); ++i) {
                        cout << path[i];
                        if (i != path.size() - 1) {
                            cout << " -> ";
                        }
                    }
                    cout << endl;
                }
                break;
            }   
            default: {
                cerr << "Invalid choice. Please try again.\n";
                break;
            }
        }
    }
    return 0;
}