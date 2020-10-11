#include "../include/File.h"


File::File(){}
File::~File(){}

/* WrittenFile functions */
WrittenFile::WrittenFile(string program) {

    if (program.find(".pre") == string::npos) {
        throw invalid_argument("Arquivo passado deve ter a extensão \".pre\"");
    }

    this->program.open(program);
    if (!this->program.is_open()) {
        throw "Não foi possível abrir o arquivo: " + program;
    }
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
        count++;
        return true;
    }
}
