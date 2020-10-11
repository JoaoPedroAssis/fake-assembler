#include <iostream>
#include <fstream>
#include "../include/Assembler.h"
#include "../include/PreProcessor.h"
#include "../include/Helper.h"
#include "../include/Error.h"

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
        this->section = TEXT;
    } else if (section == "DATA") {
        this->section = DATA;
    } else {
        this->section = INVALID;
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
        } else {
            throw invalid_argument("Rótulo indefinido");
        }
    }

    return retLine;
}

void Assembler::printAssembledFile() {
    ofstream out("./" + this->programFilename + ".obj");
    out << this->outputFile << endl;
    out.close();
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
        Line* l;
        try {
            l = getLineElements(lineContents);
        } catch (invalid_argument& e) {
            Errors::addError(
                e.what(),
                line,
                this->programFilepath,
                this->line,
                Errors::SYNTATIC_ERROR
            );
            this->line++;
            continue;
        }

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
                    Errors::addError(
                        "Rótulo já definido",
                        line,
                        this->programFilepath,
                        this->line,
                        Errors::SEMANTIC_ERROR
                    );
                    this->line++;
                    continue;
                }
            } else {
                // ERROR! LABEL INVALID
                Errors::addError(
                    "Formato de rótulo inválido",
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::LEXICAL_ERROR
                );
                this->line++;
                continue;
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
                Errors::addError(
                    "Operação não identificada",
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SYNTATIC_ERROR
                );
                this->line++;
                continue;
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

        if (line == "") {
            this->line++;
            continue;
        }
        vector<string> lineContents;
        int opIdx = 0;

        // Splitting each line in spaces and tabs
        lineContents = split(line, ' ', '\t');
        Line* l;
        try {
            l = getLineElements(lineContents);
        } catch (invalid_argument& e) {
            // Error already added in first pass
            this->line++;
            continue;
        }

        // Check if the Section TEXT is missing in the first line
        if (this->line == 1 and (l->operation != "SECTION" and l->args[0] != "TEXT")) {
            Errors::addError(
                "Seção TEXT faltante",
                line,
                this->programFilepath,
                this->line,
                Errors::SEMANTIC_ERROR
            );
            this->line++;
            continue;
        }

        // Check section
        if (l->operation == "SECTION") {

            setSection(l->args[0]);

            if (this->section == INVALID) {
                Errors::addError(
                    "Seção inválida",
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SEMANTIC_ERROR
                );
                this->line++;
                continue;
            }
        }

        Directive directive;
        Instruction instruction;
        if(Instructions.find(l->operation) != Instructions.end()) {
            if (section != TEXT) {
                //ERRO
                Errors::addError(
                    "Operação em seção incorreta",
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SEMANTIC_ERROR
                );
                this->line++;
                continue;
            }

            instruction = Instructions[l->operation];

            if (instruction.args != l->args.size()) {
                Errors::addError(
                    "Número incorreto de operandos. Esperado " +
                    to_string(instruction.args) + ", obtido " +
                    to_string(l->args.size()),
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SYNTATIC_ERROR
                );
                this->line++;
                continue;
            }

            try {
                assembledLine += to_string(instruction.opcode) + " " + assembleArgs(l->args);
            } catch(invalid_argument& e) {
                Errors::addError(
                    e.what(),
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SEMANTIC_ERROR
                );
            }
        } else if (Directives.find(l->operation) != Directives.end()) {
            if (section != DATA and l->operation != "SECTION") {
               // ERRO
                Errors::addError(
                    "Operação em seção incorreta",
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SEMANTIC_ERROR
                );
                this->line++;
                continue;
            }

            directive = Directives[l->operation];

            if (directive.args != l->args.size()) {
                // TODO ERROR! NUMERO ERRADO DE OPERANDOS
                Errors::addError(
                    "Número incorreto de operandos. Esperado " +
                    to_string(instruction.args) + ", obtido " +
                    to_string(l->args.size()),
                    line,
                    this->programFilepath,
                    this->line,
                    Errors::SYNTATIC_ERROR
                );
                this->line++;
                continue;
            }
            
            if (l->operation == "CONST") {
                if (isInteger(l->args[0])) {
                    assembledLine += l->args[0] + " ";
                } else {
                    Errors::addError(
                        "Operando inválido. Um inteiro é esperado.",
                        line,
                        this->programFilepath,
                        this->line,
                        Errors::SYNTATIC_ERROR
                    );
                }
            } else if (l->operation == "SPACE") {
                assembledLine += "0 ";
            }
        } else {
            // Otherwise, operation is invalid
            // Error already added in first pass
            this->line++;
            continue;
        }
        this->line++;
    }

    this->outputFile = assembledLine;
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

        if (Errors::hasError()) {
            throw invalid_argument("O Pré Processamento do arquivo falhou");
        }

        break;
    
    // Assemble only "-o"
    case 1:
        
        wf = new WrittenFile(programFilepath);
        this->program = wf;

        firstPass();
        secondPass();

        if (Errors::hasError()) {
            throw invalid_argument("A montagem do arquivo falhou");
        }

        break;

    // Pre-process then assemble "-po"
    case 2:
        preProcessor = new PreProcessor(programFilepath, false);
        mf = new MemoryFile(preProcessor->preProcess());
        this->program = mf;

        if (Errors::hasError()) {
            throw invalid_argument("O Pré Processamento do arquivo falhou");
        }

        firstPass();
        secondPass();

        if (Errors::hasError()) {
            throw invalid_argument("A montagem do arquivo falhou");
        }

        break;
    
    default:
        throw invalid_argument("Opção inválida");
        break;
    }
}