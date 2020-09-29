#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

typedef struct {
    unsigned short int args, opcode, size;
} Instruction;

typedef struct {
    unsigned short int args, size;
} Directive;

class Assembler {

private:
    ifstream program;
    string programFilename;
    int line;
    int memAddr;

    unordered_map<string, Instruction> Instructions = {
        {"ADD",     (Instruction) {1, 1, 2}},
        {"SUB",     (Instruction) {1, 2, 2}},
        {"MULT",    (Instruction) {1, 3, 2}},
        {"DIV",     (Instruction) {1, 4, 2}},
        {"JMP",     (Instruction) {1, 5, 2}},
        {"JMPN",    (Instruction) {1, 6, 2}},
        {"JMPP",    (Instruction) {1, 7, 2}},
        {"JMPZ",    (Instruction) {1, 8, 2}},
        {"COPY",    (Instruction) {2, 9, 3}},
        {"LOAD",    (Instruction) {1, 10, 2}},
        {"STORE",   (Instruction) {1, 11, 2}},
        {"INPUT",   (Instruction) {1, 12, 2}},
        {"OUTPUT",  (Instruction) {1, 13, 2}},
        {"STOP",    (Instruction) {0, 14, 2}}
    };

    unordered_map<string, Directive> Directives = {
        {"SECTION", (Directive) {1, 0}},
        {"SPACE",   (Directive) {0, 1}},
        {"CONST",   (Directive) {1, 1}},
        {"EQU",     (Directive) {1, 0}},
        {"IF",      (Directive) {1, 0}},

    };

    unordered_map<string, int> symbolTable;

    const vector<string> split(const string&, const char&, const char&);
    void firstPass();
    void secondPass();

public:
    Assembler(string);
    ~Assembler();
    void assemble(unsigned short int);
};
