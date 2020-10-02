#include <iostream>
#include <fstream>
#include "../include/Assembler.h"
#include "../include/PreProcessor.h"

using namespace std;

Assembler::Assembler(string programFilepath) {

    this->programFilepath = programFilepath;

    // Splitting the filename from filepath
    string file = programFilepath.substr(programFilepath.find_last_of("/") + 1);
    programFilename = file.substr(0, file.find_last_of('.'));
}

Assembler::~Assembler() {
}

// Splits a string in every character c1 and c2
const vector<string> Assembler::split(const string& s, const char& c1, const char& c2) {
	string buff{""};
	vector<string> v;
	
	for(auto n:s) {
		if(n != c1 && n != c2) {
            buff+=n; 
        }
        else if((n == c1 || n == c2) && buff != "") { 
            v.push_back(buff); buff = ""; 
        }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}

void Assembler::firstPass() {

    // Reset line and memAddr counters
    this->line = 1;
    this->memAddr = 0;

    string line;
    while(program->getNextLine(line)) {
        
        vector<string> lineContents;
        int instructionIdx = 0;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');

        // If the first line content has a ':' it is a label
        size_t pos = lineContents[0].find(":");
        string label;

        // If the line contains a label
        if (pos != string::npos) {
            // TODO fazer o label ser case insensitive e validar formato
            label = lineContents[0].substr(0, pos);
            instructionIdx = 1;

            // Check if label is already defined
            if (symbolTable.find(label) == symbolTable.end()) {
                // If not, add label
                symbolTable[label] = this->memAddr;
            } else {
                // Otherwise throw error
                // ERRO! Não é pra ter mais de um label
            }
        }            

        Instruction instruction;
        Directive directive;
        if (Instructions.find(lineContents[instructionIdx]) != Instructions.end()) {
            // If the instruction is found, add its size to the memAddr counter
            instruction = Instructions[lineContents[instructionIdx]];
            this->memAddr += instruction.size;
        } else {
            // If the instructions is not found, search the directives table
            if (Directives.find(lineContents[instructionIdx]) != Directives.end()) {
                // Sice the program has been pre-processed
                // if a directive is found, it can only be a SPACE or CONST
                directive = Directives[lineContents[instructionIdx]];
                this->memAddr += directive.size;
            } else {
                // Otherwise, operation is invalid
                // ERRO! operação não identificada  
            }
        }
        this->line++;
    }
}

void Assembler::secondPass() {}

void Assembler::assemble(int option) {
    PreProcessor *preProcessor;
    WrittenFile *wf;
    MemoryFile *mf;

    switch (option) {
    // Pre-process only "-p" and print pre-processed file
    case 0:
        preProcessor = new PreProcessor(programFilepath, true);
        preProcessor->preProcess();
        break;
    
    // Assemble only "-o"
    case 1:
        
        wf = new WrittenFile(programFilepath);
        this->program = wf;

        firstPass();
        secondPass();
        break;

    // Pre-process then assemble "-po"
    case 2:
        preProcessor = new PreProcessor(programFilepath, false);
        mf = new MemoryFile(preProcessor->preProcess());
        this->program = mf;

        this->firstPass();
        this->secondPass();
        break;
    
    default:
        // Erro! opção inválida
        break;
    }
}