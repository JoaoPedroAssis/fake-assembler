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

void Assembler::setSection(string section) {
    if (section == "TEXT") {
        section = TEXT;
    } else if (section == "DATA") {
        section = DATA;
    } else {
        // Section invalid trow ERROR
        section = INVALID;
    }
}

// Validates variable format
bool Assembler::labelIsValid(string label) {
    return 
        label.size() <= 50 and
        regex_match(label, regex("^[a-zA-Z_$][a-zA-Z_$0-9]*"));
}

string Assembler::assembleArgs(vector<string> args) {
    string retLine;
    for (int i = 0; i < args.size(); i++) {
        int memAddr;
        if (symbolTable.find(args[i]) != symbolTable.end()) {
            retLine += to_string(symbolTable[args[i]]) + " ";
        }
    }

    return retLine;
}

void Assembler::firstPass() {

    // Reset line and memAddr counters
    this->line = 1;
    this->memAddr = 0;

    string line;
    while(program->getNextLine(line)) {
        
        if (line == "" or line[0] == ';') {
            // Ignore empty lines
            continue;
        }

        vector<string> lineContents;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');
        Line *l = getLineElements(lineContents);

        // If the line contains a label
        if (!l->label.empty()) {
            if (labelIsValid(l->label)) {
                // Check if label is already defined
                if (this->symbolTable.find(l->label) == this->symbolTable.end()) {
                    // If not, add label
                    this->symbolTable[l->label] = this->memAddr;
                } else {
                    // Otherwise throw error
                    // ERRO! Não é pra ter mais de um label
                }
            } else {
                // ERROR! LABEL INVALID
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
    string assembledLine;
    while(program->getNextLine(line)) {

        vector<string> lineContents;
        int opIdx = 0;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');
        Line *l = getLineElements(lineContents); 

        // Check section
        if (l->operation == "SECTION") {
            setSection(l->args[0]);
        }

        Directive directive;
        Instruction instruction;
        if(Instructions.find(l->operation) != Instructions.end()) {
            if (section != TEXT) {
                //ERRO
            }

            instruction = Instructions[l->operation];

            if (instruction.args != l->args.size()) {
                // TODO ERROR! NUMERO ERRADO DE OPERANDOS
            }
            assembledLine += to_string(instruction.opcode) + " " + assembleArgs(l->args);
        } else if (Directives.find(l->operation) != Directives.end()) {
            if (section != DATA) {
               // ERRO 
            }

            directive = Directives[l->operation];

            if (directive.args != l->args.size()) {
                // TODO ERROR! NUMERO ERRADO DE OPERANDOS
            }
            
            if (l->operation == "CONST") {
                assembledLine += l->args[0] + " ";
            } else if (l->operation == "SPACE") {
                assembledLine += "0 ";
            }
        } else {
            // Otherwise, operation is invalid
            // ERRO! operação não identificada  
        }
        this->line++;
    }

    ofstream outputFile("./" + this->programFilename + ".obj");
    outputFile << assembledLine;
    outputFile.close();
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