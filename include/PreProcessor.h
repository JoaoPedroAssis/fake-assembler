#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class PreProcessor {
private:
    fstream program;
    bool print;
public:
    PreProcessor(string, bool);
    ~PreProcessor();
    
    vector<string> preProcess(); 
};

