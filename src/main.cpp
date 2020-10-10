#include <iostream>
#include <string.h>
#include "../include/Assembler.h"
#include "../include/Error.h"

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
    } else {
        op = -1;
    }

    Assembler *assembler = new Assembler(programFilepath);

    // Errors e;

    try {
        assembler->assemble(op);
    } 
    catch(const invalid_argument& e) {
        std::cerr << e.what() << '\n';
        Errors::printErrors();

        return 0;
    }
    catch(string& e) {
        cout << e << endl;
        return 0;
    }

    assembler->printAssembledFile();

    // TODO resolver as labels duplas
    // Validar argumentos das instruções
    return 0;
}
