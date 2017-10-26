/* sicxe_asm.cpp
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include "sicxe_asm.h"
#include "file_parser.h
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

int main(int argc, char *argv[]){
    //    if(argc != 2) {
//        cout << "Error, you must supply the name of the file " <<
//             "to process at the command line." << endl;
//        exit(1);
//    }
    string filename = "/Users/edwin.coronado/CLionProjects/Project3/source4.txt";
    try {
        file_parser parser(filename); //Initialize file_parser object
        symtab symbtab; //Initialize Symbol Table object

        parser.read_file(); //Read the file (tokenize it)

        try {
            sicxe_asm assembler;
            assembler.parse_rows(parser);
            assembler.symtab.add_symbol("EDWIN", 24);
            assembler.symtab.print_map();
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

void sicxe_asm::parse_rows(file_parser parser) {
    //This outer loop goes through each row in the file parser
    line_number = 0;
    line = nextline;
    while (to_upper_string(parser.get_token(line_number, OPCODE)) != "START" && line_number < parser.size()) {
        string temp_label = parser.get_token(line_number, LABEL);
        string temp_opcode = parser.get_token(line_number, OPCODE);
        string temp_operands = parser.get_token(line_number, OPERANDS);

        if (temp_label != "" || temp_opcode != "" || temp_operands != "")
            throw symtab_exception("Invalid syntax before 'START' opcode on line " + line_number);
        line_number++;
    }
    if (line_number == parser.size()) {
        throw symtab_exception("'START' directive not found");
    } else {
        program_name = parser.get_token(line_number, LABEL);
        LOC_CTR = format_address(parser.get_token(line_number, OPERANDS));
    }
    //TODO: Code the pseudocode that Sam sent in the e-mail. Start here.
    while (to_upper_string(temp_opcode) != "END"){
        LOC_CTR = format_address(parser.get.token(line_number, OPCODE));
            if(to_upper_string(temp_opcode) = assembler_directives[8]){
                if(temp_opcode == "EQU"){
                    if(temp_label == "")
                        throw symtab_exception("Invalid syntax on 'EQU' opcode on line " + line_number);
                }

            }
    }

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
        return stoi(temp, nullptr, 16);
    }
    else if (is_dec) {
        string temp = "";
        for (int i = 0; i < str_addr.length(); i++) {
            if (isdigit(str_addr[i]))
                temp += str_addr[i];
        }
        return stoi(temp);
    }
}


void sicxe_asm::write_to_file(string fileName) {
    string firstLine[] = {"Line#","Address","Lable","Opcode","Operand"};
    string secondLine[] = {"=====","=======","=====","======","======="};
    fileName.erase((fileName.end()-3),fileName.end());
    fileName.append("lis");

    myfile.open(fileName);
    int i = 0;
    for (i = 0; i < 5; i++) {
        myfile << firstLine[i] << "\t";
    }
    myfile << endl;
    for (i = 0; i < 5; i++) {
        myfile << secondLine[i] << "\t";
    }
    myfile << endl;

}

bool sicxe_asm::is_assm_dir(string code) {
    bool found = false;
    for (int i = 0; i < assembler_directives->length(); i++) {
        if (to_upper_string(code) == assembler_directives[i]) {
            found = true;
            return found;
        }
    }
}

//void sicxe_asm::add_to_symtab(string address, string label, string operand) {
//    symtab symbol;
//    symbol.add_symbol(string(label),int(0));
//
//}

string sicxe_asm::to_upper_string(string s) {
    string temp = "";
    for (int i=0; i<s.length(); i++) {
        temp += toupper(s[i]);
    }
    return temp;
}
