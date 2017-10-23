/* sicxe_asm.h
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#ifndef SICXE_ASM
#define SICXE_ASM

#include <iostream>
#include <fstream>
#include <string>

#include "file_parser.h"
#include "file_parse_exception.h"

#include "opcodetab.h"
#include "opcode_error_exception.h"

#include "symtab.h"
#include "symtab_exception.h"

class sicxe_asm {

public:


private:
    int lineNumber;

    ofstream myfile;
    void write_to_file(string fileName);

};