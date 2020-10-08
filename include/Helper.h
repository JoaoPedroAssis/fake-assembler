#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct {
    string label, operation;
    vector<string> args;
} Line;

// Splits the string on the two specifyed delimiters
const vector<string> split(const string&, const char&, const char&);

// Separate all line elements
Line* getLineElements(vector<string>&);

bool hasOnlyLabel(Line*);

string printArgs(Line*);

#endif // HELPER_H
