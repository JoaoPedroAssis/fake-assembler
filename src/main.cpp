#include <iostream>
#include <string.h>
#include "../include/Assembler.h"

using namespace std;

int main(int argc, char const *argv[]) {
    
    if (argc != 3) {
        cout << "Incorrect number of arguments. Given: " << argc << ", Expected 3" << endl;
        return 0;
    }

    string flag, programFilepath;

    flag = argv[1];
    programFilepath = argv[2];

    int op;
    if (flag == "-p") {
        op = 0;
    } else if ( flag == "-o") {
        op = 1;
    } else if (flag == "-po" || flag == "-op") {
        op = 2;
    }
    

    Assembler *assembler = new Assembler(programFilepath);

    try {
        assembler->assemble(op);
    } 
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}
