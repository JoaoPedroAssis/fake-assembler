#ifndef HELPER_H
#define HELPER_H
#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

typedef struct {
    string label, operation;
    vector<string> args;
} Line;

// Splits the string on the two specifyed delimiters
const vector<string> split(const string&, const char&, const char&);

// Separate all line elements
Line* getLineElements(vector<string>&);

// Check if a line struct only has a label setted
bool hasOnlyLabel(Line*);

// Format all arguments in a line struct
string printArgs(Line*);

// Transform all chars to uppercase
string toUpper(string);

// Check if a string is integer
bool isInteger(const string &s);

#endif // HELPER_H
