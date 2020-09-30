#include <iostream>
#include "../include/PreProcessor.h"

PreProcessor::PreProcessor(string program, bool print) {
    // TODO: verify if file is indeed opened
    // if its not, terminate program
    this->program.open(program);
    this->print = print;
}

PreProcessor::~PreProcessor() {
    program.close();
}