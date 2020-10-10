#include <iostream>
#include "../include/PreProcessor.h"
#include "../include/Helper.h"
#include "../include/Error.h"

using namespace std;

PreProcessor::PreProcessor(string program, bool print) {

    programFilepath = program;

    // TODO: verify if file is indeed opened
    // if its not, terminate program
    this->program.open(program);
    if (!this->program.is_open()) {
        throw ("Não foi possível abrir o arquivo: " + program);
    }
    
    this->print = print;

    string file = program.substr(program.find_last_of("/") + 1);
    programFilename = file.substr(0, file.find_last_of('.'));
}

PreProcessor::~PreProcessor() {
    program.close();
}

vector<string> PreProcessor::preProcess() {
    vector<string> outputFile;
    
    int fileLine = 1;
    string line;
    string preProcessedLine = "";
    while(getline(this->program, line)) {
        if (line == "" or line[0] == ';') {
            // Ignore empty lines
            fileLine++;
            continue;
        }
        vector<string> lineContents;

        lineContents = split(line, ' ', '\t');
        Line *l;
        try {
            l = getLineElements(lineContents);
        } catch (invalid_argument& e) {
            Errors::addError(
                e.what(),
                line,
                this->programFilepath,
                fileLine,
                Errors::SYNTATIC_ERROR
            );
            fileLine++;
            continue;
        }
        bool printCurrLine = true;

        if (hasOnlyLabel(l)) {
            if (!l->label.empty()) {
                preProcessedLine += l->label + ": ";
            }

            if (!getline(this->program, line)) {
                //EOF
                break;
            }
            fileLine++;

            vector<string> tmpLineContents = split(line, ' ', '\t');

            Line *tmp;
            try {
                tmp = getLineElements(tmpLineContents);
            } catch (invalid_argument& e) {
                Errors::addError(
                    e.what(),
                    line,
                    this->programFilepath,
                    fileLine,
                    Errors::SYNTATIC_ERROR
                );
                fileLine++;
                continue;
            }

            if (tmp->operation == "EQU") {
                if (tmp->args.size() != 1) {
                    // ERRO!
                    Errors::addError(
                        "Número de parâmetros incorreto",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SYNTATIC_ERROR
                    );
                    fileLine++;
                    continue;
                }

                // Define se for a primeira vez, se for a segunda define por cima
                defines[l->label] = stoi(tmp->args[0]);
                printCurrLine = false;
            } else if (tmp->operation == "IF") {
                if (tmp->args.size() != 1) {
                    // ERRO!
                    Errors::addError(
                        "Número de parâmetros incorreto",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SYNTATIC_ERROR
                    );
                    fileLine++;
                    continue;
                }
                
                if (defines.find(l->args[0]) == defines.end()) {
                    // ERRO LABEL INDEFINIDO
                    Errors::addError(
                        "Rótulo indefinido",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SEMANTIC_ERROR
                    );
                    fileLine++;
                    continue;
                } else {
                    int define = defines[l->args[0]];
                    if (define == 0) {
                        // pega a prox linha e ignora
                        getline(this->program, line);
                        fileLine++;
                    }
                }
                printCurrLine = false;
            } else {
                for (int i = 0; i < tmp->args.size(); i++) {
                    if (defines.find(tmp->args[i]) == defines.end()) {
                        // ERRO LABEL INDEFINIDO
                        Errors::addError(
                            "Rótulo indefinido",
                            line,
                            this->programFilepath,
                            fileLine,
                            Errors::SEMANTIC_ERROR
                        );
                        fileLine++;
                        continue;
                    } else {
                        tmp->args[i] = defines[tmp->args[i]];
                    }
                }
            }

            if (printCurrLine) {
                preProcessedLine += tmp->operation + " " + printArgs(tmp);
                outputFile.push_back(preProcessedLine);
                preProcessedLine = "";
            } else {
                preProcessedLine = "";
            }
        } else {
            if (!l->label.empty()) {
                preProcessedLine += l->label + ": ";
            }

            if (l->operation == "EQU") {
                if (l->args.size() != 1) {
                    // ERRO!
                    Errors::addError(
                        "Número de parâmetros incorreto",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SYNTATIC_ERROR
                    );
                    fileLine++;
                    continue;
                }

                // Define se for a primeira vez, se for a segunda define por cima
                defines[l->label] = stoi(l->args[0]);
                printCurrLine = false;
            } else if (l->operation == "IF") {
                if (l->args.size() != 1) {
                    // ERRO!
                    Errors::addError(
                        "Número de parâmetros incorreto",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SYNTATIC_ERROR
                    );
                    fileLine++;
                    continue;
                }

                if (defines.find(l->args[0]) == defines.end()) {
                    // ERRO LABEL INDEFINIDO
                    Errors::addError(
                        "Rótulo indefinido",
                        line,
                        this->programFilepath,
                        fileLine,
                        Errors::SEMANTIC_ERROR
                    );
                    fileLine++;
                    continue;
                } else {
                    int define = defines[l->args[0]];
                    if (define == 0) {
                        // pega a prox linha e ignora
                        getline(this->program, line);
                        fileLine++;
                    }
                }
                printCurrLine = false;
            } else {
                for (int i = 0; i < l->args.size(); i++) {
                    if (defines.find(l->args[i]) != defines.end()) {
                        l->args[i] = to_string(defines[l->args[i]]);
                    }
                }
            }

            if (printCurrLine) {
                preProcessedLine += l->operation + " " + printArgs(l);
                outputFile.push_back(toUpper(preProcessedLine));
                preProcessedLine = "";
            } else {
                preProcessedLine = "";
            }
        }
        fileLine++;
    }

    if (print) {
        printPreprocessedFile(outputFile);
    } 

    return outputFile;
}

void PreProcessor::printPreprocessedFile(vector<string> outputVector) {
    ofstream outputFile("./" + this->programFilename + ".pre");

    for (string line: outputVector) {
        outputFile << line << endl;
    }

    outputFile.close();
}