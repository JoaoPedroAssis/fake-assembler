#include <iostream>
#include <string.h>
#include "../include/Assembler.h"
#include "../include/Error.h"

using namespace std;

int main(int argc, char const *argv[]) {

    string flag, programFilepath;
    int op;

    if (argc == 2) {
        programFilepath = argv[1];
        op = 2;
    } else if (argc == 3) {
        flag = argv[1];
        programFilepath = argv[2];

        if (flag == "-p") {
            op = 0;
        } else if ( flag == "-o") {
            op = 1;
        } else {
            op = -1;
        }
    } else {
        cout << "Número incorreto de argumentos." << endl;
        cout << "Para montar diretamente: \"./montador <nome_do_programa.asm>\"" << endl;
        cout << "Para pré-processar apenas: \"./montador -p <nome_do_programa.asm>\"" << endl;
        cout << "Para montar um arquivo pré-processado: \"./montador -o <nome_do_programa.pre>\"" << endl;
        return 0;
    }

    Assembler *assembler = new Assembler(programFilepath);

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
