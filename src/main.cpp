#include <iostream>
#include <string.h>
#include "../include/Assembler.h"

using namespace std;

int main(int argc, char const *argv[]) {
    
    if (argc != 3) {
        cout << "Incorrect number of arguments. Given: " << argc << ", Expected 3" << endl;
        return 0;
    }

    string op, programFilepath;

    op = argv[1];
    programFilepath = argv[2];

    

    Assembler *assembler = new Assembler(programFilepath);

    assembler->assemble(1);

    return 0;
}
