#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class File {
public:
    virtual bool getNextLine(string&) = 0;
    File();
    ~File();
};

class WrittenFile : public File {
private:
    fstream program;

public:
    bool getNextLine(string&) override;

    WrittenFile(string);
    ~WrittenFile();
};

class MemoryFile : public File {
private:
    vector<string> program;
    int count = 0;

public:
    bool getNextLine(string&) override;

    MemoryFile(vector<string>);
    ~MemoryFile();
};

#endif // FILE_H
