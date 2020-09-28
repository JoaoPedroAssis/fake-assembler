#include <iostream>
#include <string.h>
#include "../include/Assembler.h"

using namespace std;

int main(int argc, char const *argv[]) {
    
    if (argc != 3) {
        cout << "Incorrect number of arguments. Given: " << argc << ", Expected 3" << endl;
        return 0;
    }

    string op, filepath;

    op = argv[1];
    filepath = argv[2];

    // Splitting the filename from filepath
    string file = filepath.substr(filepath.find_last_of("/") + 1);
    string filename = file.substr(0, file.find_last_of('.'));

    return 0;
}
