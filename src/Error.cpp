#include "../include/Error.h"

vector<Error> Errors::allErrors;

void Errors::addError(string message, string line, string filename, int fileLine, int errorType) {
    Error error = {
        fileLine,
        errorType,
        message,
        line,
        filename
    };

    Errors::allErrors.push_back(error);
}

bool Errors::hasError() {
    return !Errors::allErrors.empty();
}

void Errors::printErrors() {
    if (Errors::hasError()){
        cout << "Programa não montado devido aos seguintes erros:" << endl << endl;
    }
    for (Error e: Errors::allErrors) {
        string line;
        line += "Erro " + errorTypes[e.errorType] + " no arquivo " + e.filename + ", na linha " + to_string(e.fileLine) + ": ";
        line += e.line + '\n';
        line += "Motivo: " + e.message + "\n\n";
        cout << line;
    }
}