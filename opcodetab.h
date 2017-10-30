#ifndef OPCODETAB_H
#define OPCODETAB_H

#include <iostream>
#include <map>

using namespace std;

class opcodetab {
public:
    // ctor
    // creates a new dictionary structure and loads all of the opcodes for
    // the SIC/XE architecture into the table.  Use the STL
    // map for this.
    opcodetab();

    // takes a SIC/XE opcode and returns the machine code
    // equivalent as a two byte string in hexadecimal.
    // Example:  get_machine_code("ADD") returns the value 18
    // Note that opcodes may be prepended with a '+'.
    // throws an opcode_error_exception if the opcode is not
    // found in the table.
    string get_machine_code(string);

    // takes a SIC/XE opcode and returns the number of bytes
    // needed to encode the instruction, which is an int in
    // the range 1..4.
    // NOTE: the opcode must be prepended with a '+' for format 4.
    // throws an opcode_error_exception if the opcode is not
    // found in the table.
    int get_instruction_size(string);

private:
    string codes[60] = {"ADD","ADDF","ADDR","AND","CLEAR","COMP","COMPF","COMPR","DIV","DIVF","DIVR","FIX","FLOAT","HIO","J","JEQ","JGT","JLT","JSUB,","LDA","LDB","LDCH","LDF","LDL","LDS","LDT","LDX","LPS","MUL","MULF","MULR","NORM","OR","RD","RMO","RSUB","SHIFTL","SHIFTR","SIO","SSK","STA","STB","STCH","STF","STI","STL","STS","STSW","STT","STX","SUB","SUBF","SUBR","SVC","TD","TIO","TIX","TIXR","WD"};
    int opcodes[60] = {0x18,0x58,0x90,0x40,0xB4,0x28, 0x88, 0xA0, 0x24, 0x64,0x9C,0xC4,0xC0,0xF4,0x3C,0x30,0x34,0x38,0x48,0x00,0x68,0x50,0x70,0x08,0x6C,0x74,0x04,0xD0,0x20,0x60,0x98,0xC8,0x44,0xD8,0xAC,0x4C,0xA4,0xA8,0xF0,0xEC,0x0C,0x78,0x54,0x80,0xD4,0x14,0x7C,0xE8,0x84,0x10,0x1C,0x5C,0x94,0xB0,0xE0,0xF8,0x2C,0xB8,0xDC};
    int bytes[60] = {3,3,2,3,2,3,3,2,3,3,2,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,1,3,3,2,3,2,2,1,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,3,1,3,2,3};

    map<string,pair<int,int> > m;
    map<string,pair<int,int> >::iterator m_iter;

    void print_map();
    bool opcode_exists(string);


    int has_plus;
    int get_codes_size();
    int get_bytes_size();
    int get_opcodes_size();

    string to_upper_string(string);
    string remove_first_plus_sign(string);
};

#endif