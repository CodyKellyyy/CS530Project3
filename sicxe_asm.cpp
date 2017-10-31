/* sicxe_asm.cpp
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include <iostream>
#include <iomanip>
#include <string>
#include "sicxe_asm.h"
#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

int main(int argc, char *argv[]){

    string filename = "/Users/tommyvalenta/CLionProjects/CS530Project3/source1.txt";
    try {
        file_parser parser(filename); //Initialize file_parser object
        symtab symbtable; //Initialize Symbol Table object

        parser.read_file(); //Read the file (tokenize it)

        try {
            sicxe_asm assembler;
            assembler.parse_rows(parser, filename);
            //assembler.symtab.print_symtab();
        } catch (symtab_exception& e) {
            cerr << e.getMessage() << endl;
            exit(EXIT_FAILURE);
        }
    }
    catch(file_parse_exception excpt) {
        cerr << "**Sorry, error: " << excpt.getMessage() << endl;
    }

}

sicxe_asm::sicxe_asm() {}

void sicxe_asm::parse_rows(file_parser parser, string fileName) {
    write_headers(fileName);//Write headers to the listing file

    opcodetab opcodetab;
    //This outer loop goes through each row in the file parser
    line_number = 0;
    while (to_upper_string(parser.get_token(line_number, OPCODE)) != "START" && line_number < parser.size()) {
        string temp_label = parser.get_token(line_number, LABEL);
        string temp_opcode = parser.get_token(line_number, OPCODE);
        string temp_operand = parser.get_token(line_number, OPERANDS);

        if (temp_label != "" || temp_opcode != "" || temp_operand != "")
            throw symtab_exception("Invalid syntax before 'START' opcode on line " + line_number);
        line_number++;
        write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);
    }

    string temp_label = parser.get_token(line_number, LABEL);
    string temp_opcode = parser.get_token(line_number, OPCODE);
    string temp_operand = parser.get_token(line_number, OPERANDS);
    if (line_number == parser.size()) {
        throw symtab_exception("'START' directive not found");
    } else {
        program_name = parser.get_token(line_number, LABEL);
        LOC_CTR = format_address(parser.get_token(line_number, OPERANDS));
    }
    line_number++;
    write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);

    while (line_number < parser.size()){
        string temp_label = parser.get_token(line_number, LABEL);
        string temp_opcode = parser.get_token(line_number, OPCODE);
        string temp_operand = parser.get_token(line_number, OPERANDS);
        if(is_assm_dir(to_upper_string(temp_opcode))){
            if(temp_opcode == "EQU"){
                if(temp_label == "")
                    throw symtab_exception("Invalid syntax on 'EQU' opcode on line " + line_number);
                if(symtab.symbol_exists(temp_label) == true)
                    throw symtab_exception("Label is already in use, reused on line " + line_number);
                symtab.add_symbol(temp_label, format_address(temp_operand));
            } else {
                if (temp_label != "") {
                    if (symtab.symbol_exists(temp_label) == true)
                        throw symtab_exception("Label is already in use, reused on line " + line_number);
                    symtab.add_symbol(temp_label, LOC_CTR);
                }
            }

            if (temp_opcode == "BASE")
                base = temp_operand;
            if (to_upper_string(temp_opcode) == "NOBASE")
                base = "";
            if (to_upper_string(temp_opcode) == "WORD")
                LOC_CTR += 3;
            if (to_upper_string(temp_opcode) == "BYTE") {
                token = substring_quotes(temp_operand);
                if (toupper(temp_operand[0]) == 'C')
                    LOC_CTR += token.length();
                else if (toupper(temp_operand[0]) == 'X'){
                    if((token.length() & 1) == 1)
                        throw symtab_exception("Cannot have hex value with odd number of digits on line" + line_number);
                    LOC_CTR += (token.length() >> 1);
                }
            }
            else if (to_upper_string(temp_opcode) == "RESW")
                LOC_CTR += (3 * (format_address(temp_operand)));
            else if (to_upper_string(temp_opcode) == "RESB")
                LOC_CTR += format_address(temp_operand);
            line_number++;
            write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);

        } else {
            if (temp_label != "") {
                if (symtab.symbol_exists(temp_label))
                    throw symtab_exception("Label is already in use, reused on line " + line_number);
                else symtab.add_symbol(temp_label,LOC_CTR);
            }
            //cout << "Temp label: "  << temp_label << " Temp Opcode: " << temp_opcode << " LOC_CTR: " << LOC_CTR << endl;
            if (temp_opcode == "") {
                line_number++;
            } else {
                size = opcodetab.get_instruction_size(temp_opcode);
                if (size == 0)
                    throw symtab_exception("Size of Opcode " + temp_opcode + " on line number " + to_string(line_number) + " not found");
                LOC_CTR += size;
                line_number++;
            }
            write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);
        }
    }
    myfile.close();
}

int sicxe_asm::format_address(string str_addr) {
    bool is_hex;
    bool is_dec;

    size_t found_hex = str_addr.find("$");
    if (found_hex != string::npos) {
        is_hex = true;
        is_dec = false;
    } else {
        is_dec = true;
        is_hex = false;
    }

    if (is_hex) {
        string temp = "";
        for (int i = 0; i < str_addr.length(); i++) {
            if (isdigit(str_addr[i]))
                temp += str_addr[i];
        }
        return string_to_int(temp);
    }
    else if (is_dec) {
        string temp = "";
        for (int i = 0; i < str_addr.length(); i++) {
            if (isdigit(str_addr[i]))
                temp += str_addr[i];
        }
        return string_to_int(temp);
    }
}


void sicxe_asm::write_headers(string fileName) {
    fileName.erase((fileName.end()-3),fileName.end());
    fileName.append("lis");
    myfile.open(fileName);
    string firstLine[] = {"Line#","Address","Label","Opcode","Operand"};
    string secondLine[] = {"=====","=======","=====","======","======="};

    int i = 0;
    for (i = 0; i < 5; i++) {
        myfile << setw(10) << std::left << firstLine[i];
    }
    myfile << "\n";
    for (i = 0; i < 5; i++) {
        myfile << setw(10) << std::left << secondLine[i];
    }
    myfile << "\n";
}

void sicxe_asm::write_to_file(int line_num, int address, string label, string opcode, string operand){
    myfile << setw(10) << std::left << line_num;
    myfile << setw(10) << std::left << address;
    myfile << setw(10) << std::left << label;
    myfile << setw(10) << std::left << opcode;
    myfile << setw(10) << std::left << operand;
    myfile << "\n";
}



bool sicxe_asm::is_assm_dir(string code) {
    bool found = false;
    for (int i = 0; i < 8; i++) {
        if (to_upper_string(code) == assembler_directives[i]) {
            found = true;
            return found;
        }
    }
}

string sicxe_asm::to_upper_string(string s) {
    string temp = "";
    for (int i=0; i<s.length(); i++) {
        temp += toupper(s[i]);
    }
    return temp;
}

string sicxe_asm::substring_quotes(string operand) {
    try {
        int first_quote = operand.find_first_of("'")+1;
        int last_quote  = operand.find_last_of("'");
        return operand.substr(first_quote,last_quote-first_quote);
    } catch (symtab_exception& e) {
        cerr << e.getMessage() << endl;
        exit(EXIT_FAILURE);
    }
}

int sicxe_asm::string_to_int(string i) {
    if(i[0] == '#' || i[0] == '$' || i[0] == '@') {
        i.erase(0,1);
    }
    int intOut;
    sscanf(i.c_str(),"%x",&intOut);
    return intOut;
}