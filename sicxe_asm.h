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
    sicxe_asm();
    string to_upper_string(string);
    void parse_rows(file_parser, string);
    int format_address(string);
    bool is_assm_dir(string);
    symtab symtab;



private:

    /* ---- Members ---- */
    // All values to be put into list file


    int line_number;
    int starting_address;
    int LOC_CTR;
    int size;


    string program_name;
    string address;
    string temp_label;
    string temp_opcode;
    string temp_operand;
    string token;
    string base;

    ofstream myfile;

    /* ---- Functions ---- */
    // Writes the Line number, address, label, opcode, and operand to the listing file
    void write_headers(string);
    // Adds the symbols in the code to symtab
    void add_to_symtab(string, string, string);
    //Writes to the listing files
    void write_to_file(int, string, string, string);
    // Formats the address depending if it's hex or dec
    // Changes string to int
    //int to_int(string);
    //TODO: define the substring_quotes() fxn
    string substring_quotes(string);

    /* Vector of Assembler Directives */
    string assembler_directives[8] = {"START",
                                      "END",
                                      "BYTE",
                                      "WORD",
                                      "RESB",
                                      "RESW",
                                      "BASE",
                                      "NOBASE"};

    /* ---- Enum for legibility purposes ---- */
    enum Token {
        LABEL = 0,
        OPCODE = 1,
        OPERANDS = 2,
        COMMENTS = 3
    };


};

#endif