#include "../include/File.h"


File::File(){}
File::~File(){}

/* WrittenFile functions */
WrittenFile::WrittenFile(string program) {
    this->program.open(program);
    // Test if file is indeed opened
}

WrittenFile::~WrittenFile() {
    this->program.close();
}

bool WrittenFile::getNextLine(string& line) {
    if (program.eof()) {
        program.clear();
        program.seekg(0, ios::beg);
        return false;
    } else {
        getline(program, line);
        return true;
    }

}


/* MemoryFile functions */
MemoryFile::MemoryFile(vector<string> preProcessedProgram) {
    program = preProcessedProgram;
}

bool MemoryFile::getNextLine(string& line) {
    if (count >= program.size()) {
        count = 0;
        return false;
    } else {
        line = program[count];
        return true;
    }
}
