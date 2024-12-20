#include <iostream>

#include "Get_Correct.cpp"
#include "GasNetwork.h"

void GasNetwork::print_menu(){
    cout << "Menu:\n";
    cout << "1. Add a pipe\n";
    cout << "2. Add a compressor station\n";
    cout << "3. View all objects\n";
    cout << "4. Edit a pipe\n";
    cout << "5. Edit a compressor station\n";
    cout << "6. Save\n";
    cout << "7. Load\n";
    cout << "8. Delete a pipe\n";
    cout << "9. Delete a compressor station\n";
    cout << "10. Searching pipes by filter\n";
    cout << "11. Searching compressor station by filter\n";
    cout << "12. Batch editing of pipes\n";
    cout << "13. Batch deletion of pipes\n";
    cout << "14. Add connection of pipes and compressor stations to the gas transmission network\n";
    cout << "15. Topological sorting\n";
    cout << "16. Calculate the maximum flow in the network\n";
    cout << "17. Calculate the shortest path between the given compressor station\n";
    cout << "0. Exit\n";
}

void GasNetwork::add_pipe(Pipe pipe){
    pipe.read();
    pipes.insert(make_pair(pipe.getid(), pipe));
}

void GasNetwork::add_station(CompressorStation station){
    station.read();
    stations.insert(make_pair(station.getid(), station));
}

void GasNetwork::print_pipe_and_cs(){
    if (pipes.size() != 0){
        cout << "Pipes:\n";
        for (auto& [id, pipe] : pipes){
            cout << pipe << endl;
        }
    }else{
        cout << "No pipes\n";
    }
    if(stations.size() != 0){
        cout << "Compressor Stations:\n";
        for (auto& [id, station] : stations){
            cout << station << endl;
        }
    }else{
        cout << "No compressor stations\n";
    }
}

void GasNetwork::edit_pipe(){
    if (pipes.size() != 0){
        int pipe_id = get_valid_id("Enter the pipe ID for editing: ", pipes);
        Pipe& pipe = pipes[pipe_id];
        pipe.toggle_repair();
        cout << "The state of the pipe with ID " << pipe_id << " has been changed to 'Under repair: " << (pipe.under_repair ? "Yes" : "No") << "'\n";
    }else{
        cout << "No data\n";
    }
}

void GasNetwork::edit_cs(){
    if(stations.size() != 0){
        int station_id = get_valid_id("Enter the compressor station ID for editing: ", stations);
        CompressorStation& station = stations[station_id];
        station.edit();
    }else{
        cout << "No data\n";
    }
}

void GasNetwork::save_file(){
    cout << "Enter the file name to save ('file name.txt'): ";
    string file_name = get_str();
    ofstream out(file_name);
    for (auto const& p : pipes){
        if(!p.second.name.empty()){
            pipes[p.first].save_data(out);
        }
    }
    for (auto const& cs : stations){
        if(!cs.second.name.empty()){
            stations[cs.first].save_data(out);
        }
    }
    cout << "Data saved to file: " << file_name << endl;
}

void GasNetwork::load_file(){
    cout << "Enter the file name: ";
    string file_name;
    string read_file = get_str();
    ifstream read(read_file);
    if (read.peek() == ifstream::traits_type::eof()) {
        cout << "Error! There is no data in the file.\n";
    }
    else {
        string Name;
        while (getline(read, Name)) {
            if (Name == "Pipe") {
                Pipe read_pipe;
                read_pipe.load_data(read);
                pipes.insert({ read_pipe.getid(), read_pipe });
                cout << "Pipe data " << read_pipe.getid() << " downloaded from a file." << '\n';
            }
            if (Name == "Compressor Station") {
                CompressorStation read_ks;
                read_ks.load_data(read);
                stations.insert({ read_ks.getid(), read_ks });
                cout << "Compressor Station data " << read_ks.getid() << " downloaded from a file." << '\n';
            }
        }
    }
}

void GasNetwork::delete_pipe(){
    if (pipes.size() != 0){
        int pipe_id = get_valid_id("Enter the pipe ID for delete: ", pipes);
        pipes.erase(pipe_id);
        cout << "Pipe with ID " << pipe_id << " has been deleted.\n";
    }else{
        cout << "No data\n";
    }
}

void GasNetwork::delete_cs(){
    if(stations.size() != 0){
        int station_id = get_valid_id("Enter the ID of the compressor station to delete: ", stations);
        stations.erase(station_id);
        for (auto& [id, pipe] : pipes) {
            if (pipe.getIdCSOfTheEntrance() == station_id || pipe.getIdCSOfTheExit() == station_id){
                pipe.connecting_with_cs(0, 0);
            }
        }
        cout << "Compressor station with ID " << station_id << " has been deleted.\n";
    }else{
        cout << "No data\n";
    }
}

void GasNetwork::connectPipesToStations() {
    if (pipes.size() != 0 && stations.size() != 0) {
        int in_id, out_id, pipe_diameter;

        std::cout << "Enter the ID of the input compressor station: ";
        in_id = get_correct_value<int>(1, INT_MAX);

        std::cout << "Enter the ID of the output compressor station: ";
        out_id = get_correct_value<int>(1, INT_MAX);

        pipe_diameter = get_correct_diameter();

        bool pipe_found = false;
        for (auto& [id, pipe] : pipes) {
            if (pipe.diameter == pipe_diameter && pipe.getIdCSOfTheEntrance() == 0 && pipe.getIdCSOfTheExit() == 0) {
                pipe.connecting_with_cs(in_id, out_id);
                pipe_found = true;
                break;
            }
        }

        if (!pipe_found) {
            std::cout << "The pipe with the entered diameter does not exist! Create a pipe.\n";
            Pipe pipe;
            pipe.read();
            //pipe.diameter = pipe_diameter;
            pipe.connecting_with_cs(in_id, out_id);
            pipes.insert(make_pair(pipe.getid(), pipe));
        }

        std::cout << "Pipe connected successfully.\n";
    } else {
        std::cout << "Insufficient data to establish connections.\n";
    }
}