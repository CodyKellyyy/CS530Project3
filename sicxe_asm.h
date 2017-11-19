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
#include <cstdlib>
#include <map>
#include <vector>

#include "file_parser.h"
#include "file_parse_exception.h"

#include "opcodetab.h"
#include "opcode_error_exception.h"

#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

class sicxe_asm {

public:
    sicxe_asm(string);
    string to_upper_string(string);
    string to_string(int);
    void pass_one();
    void pass_two();
    unsigned int format_address(string);
    unsigned long hex_string_to_int(string);
    unsigned int opcode_to_6_bit(unsigned int);
    bool is_assm_dir(string);
    string opcode_binary(string);
    string nixbpe(string);
    string format_headername(string);
    string get_displacement(string, string, string);
    symtab symtable;
    opcodetab optab;
    void set_reg_value(string, unsigned int);
    int get_reg_number(string);
    int get_reg_value(string);
    void set_flag(char, unsigned int);
    unsigned int get_flag(char);
    unsigned int flags_to_int();

    /* We never really saved the line num, address, label, opcode, etc.. in
     * pass one. This object will help us save that. I added a vector<pass_one_record>
     * and add to it every time we call "write_to_file()" in pass one. So as we
     * print to the file, we also save the data so that it makes pass 2 easier to iterate */
    class pass_one_record {
    public:
        pass_one_record(){};
        pass_one_record(int line_num, int addr, string lab, string opc, string oper) {
            line = line_num;
            address = addr;
            label = lab;
            opcode = opc;
            operand = oper;
            machine_code = "";
        }
        unsigned int line = 0;
        unsigned int address = 0;
        string label = "";
        string opcode = "";
        string operand = "";
        string machine_code = "";
    };

private:

    /* ---- Members ---- */
    // All values to be put into list file
    int line_number;
    int address = 0;
    int LOC_CTR;
    int size;
    file_parser *parser;

    string file_name;
    string program_name;
    string token;
    string base;

    ofstream myfile;

    /* ---- Functions ---- */
    // Writes the Line number, address, label, opcode, and operand to the listing file
    void write_headers();
    //Writes to the listing files
    void write_to_file(int, int, string, string, string);
    // Formats the address depending if it's hex or dec
    // Changes string to int
    unsigned int string_to_int(string);
    // Loads the registers to a map
    void load_registers();
    // Loads the flags to a map
    void load_flags();
    //Converts from int to hex
    string int_to_hex(int, int);
    //gets binary form of hex number
    string hex_to_bin(string);
    //Converts a normal string i.e.: "edwin" and returns is hex ascii equivalent: "656477696e"
    string string_to_hex_string(string);
    //TODO: define the substring_quotes() fxn
    string substring_quotes(string);

    /* Vector of Assembler Directives */
    string assembler_directives[9] = {"START",
                                      "END",
                                      "EQU",
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

    /* This map will hold the register name as key, and a pair
     * of unsigned ints where the 'first' is the register number
     * and 'second' is the current value */
    map<string, pair<unsigned int, unsigned int> > registers;
    /* This map will hold the flag char (i.e.: 'n') and its
     * current value */
    map<char, unsigned int> flags;
    /* This is a vector of "pass_one_record" objects that
     * will help us parse everything and make iteration
     * easier */
    vector<pass_one_record> pass_one_tab;



};

#endif