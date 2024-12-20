#include <iostream>
#include <string>

#include "Logging.h"

using namespace std;

inline string get_str() {
    string str;
    cin >> ws;
    getline(cin, str);
    cerr << str << "\n";
    return str;
}

inline int get_correct_diameter() {
    int diameter;
    while (true) {
        std::cout << "Enter the pipe diameter (500, 700, 1000, 1400 mm): ";
        if (!(std::cin >> diameter) || (diameter != 500 && diameter != 700 && diameter != 1000 && diameter != 1400)) {
            std::cout << "Invalid input! Please enter a valid diameter.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        } else {
            break;
        }
    }
    return diameter;
}

template <typename T>
T get_correct_value(T min, T max) {
    T value;
    cin >> value;
    cerr << value << "\n";
    while (cin.fail() || cin.peek() != '\n' || value < min || value > max) {
        cin.clear();
        cin.ignore(10000, '\n');
        cerr << "ERROR: Enter a valid value: ";
        cin >> value;
        cerr << value << "\n";
    }
    return value;
}

template<typename K>
int get_valid_id(const string& message, const std::unordered_map<int, K>& items) {
    int id;
    cout << message;
    while (!(cin >> id) || items.find(id) == items.end()) {
        cerr << "Error! Enter an exiting ID: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cout << message;
    }
    return id;
}

template <typename T>
int get_correct_id(unordered_map<int, T>& dict){
    int checking_the_key = get_correct_value(1, INT_MAX);
    while(dict.find(checking_the_key) != dict.end()){
        cout << "This is no  with this id! Enter the correct id: ";
        checking_the_key = get_correct_value(1, INT_MAX);
    }
    return checking_the_key;
}