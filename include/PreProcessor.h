#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

using namespace std;

class PreProcessor {
private:
    fstream program;
    string programFilename;
    string programFilepath;
    bool print;
    unordered_map<string, int> defines;

    void printPreprocessedFile(vector<string>);
public:
    PreProcessor(string, bool);
    ~PreProcessor();
    
    vector<string> preProcess(); 
};

#endif // PRE_PROCESSOR_H
