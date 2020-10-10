#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
    int fileLine, errorType;
    string message, line, filename;
} Error;

const vector<string> errorTypes = {
    "Léxico",
    "Sintático",
    "Semântico"
};

class Errors {
public:
    static vector<Error> allErrors;
    enum {
        LEXICAL_ERROR,
        SYNTATIC_ERROR,
        SEMANTIC_ERROR
    };

    static void addError(string message, string line, string filename, int fileLine, int errorType);
    static bool hasError();
    static void printErrors();
};

#endif // ERROR_H
