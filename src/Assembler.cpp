#include <iostream>
#include <fstream>
#include "../include/Assembler.h"
#include "../include/PreProcessor.h"
#include "../include/Helper.h"

using namespace std;

Assembler::Assembler(string programFilepath) {

    this->programFilepath = programFilepath;

    // Splitting the filename from filepath
    string file = programFilepath.substr(programFilepath.find_last_of("/") + 1);
    programFilename = file.substr(0, file.find_last_of('.'));
}

Assembler::~Assembler() {
}



void Assembler::firstPass() {

    // Reset line and memAddr counters
    this->line = 1;
    this->memAddr = 0;

    string line;
    while(program->getNextLine(line)) {
        
        vector<string> lineContents;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');
        Line *l = getLineElements(lineContents);

        // If the line contains a label
        if (!l->label.empty()) {
            // TODO fazer o label ser case insensitive e validar formato

            // Check if label is already defined
            if (symbolTable.find(l->label) == symbolTable.end()) {
                // If not, add label
                symbolTable[l->label] = this->memAddr;
            } else {
                // Otherwise throw error
                // ERRO! Não é pra ter mais de um label
            }
        }            

        Instruction instruction;
        Directive directive;
        if (Instructions.find(l->operation) != Instructions.end()) {
            // If the instruction is found, add its size to the memAddr counter
            instruction = Instructions[l->operation];
            this->memAddr += instruction.size;
        } else {
            // If the instructions is not found, search the directives table
            if (Directives.find(l->operation) != Directives.end()) {
                //** Sice the program has been pre-processed
                // if a directive is found, it can only be a SPACE or CONST
                directive = Directives[l->operation];
                this->memAddr += directive.size;
            } else {
                // Otherwise, operation is invalid
                // ERRO! operação não identificada  
            }
        }
        this->line++;
    }

    return;
}

void Assembler::secondPass() {
    // Reset line and memory counters
    this->line = 1;
    this->memAddr = 0;

    string line;
    bool isInText = false;
    bool isInData = false;
    while(program->getNextLine(line)) {

        vector<string> lineContents;
        int opIdx = 0;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');
        Line *l = getLineElements(lineContents); 

        Directive directive;
        Instruction instruction;
        if (Directives.find(l->operation) != Directives.end()) {
            directive = Directives[l->operation];
            if (directive.args != l->args.size()) {
                // TODO ERROR! NUMERO ERRADO DE OPERANDOS
            }
            if (l->operation == "SECTION") {
                if (l->args[0] == "TEXT") {
                    isInText = true;
                    isInData = false;
                } else if (l->args[0] == "DATA") {
                    isInData = true;
                    isInText = false;
                } else {
                    // SECTION INVALID;
                }  
            }
        } else {
            // Otherwise, operation is invalid
            // ERRO! operação não identificada  
        }
    }
}

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

        firstPass();
        secondPass();
        break;
    
    default:
        // Erro! opção inválida
        break;
    }
}