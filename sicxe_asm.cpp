/* sicxe_asm.cpp
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <sstream>
#include "sicxe_asm.h"
//#include "file_parser.cc"
#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

int main(int argc, char *argv[]){

    string filename = "/Users/codykelly/CLionProjects/CS530Project3/source1.txt";
    //string filename = argv[1];
    try {
        sicxe_asm assembler(filename);
        assembler.pass_one();
        assembler.pass_two();
    } catch (exception& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }

}

sicxe_asm::sicxe_asm(string filename) {
    file_name = filename;
    try {
        parser = new file_parser(file_name); //Initialize file_parser object
        parser->read_file(); //Read the file (tokenize it)
    } catch (file_parse_exception& e) {
        cerr << e.getMessage() << endl;
        exit(EXIT_FAILURE);
    }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pass 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void sicxe_asm::pass_one() {
    write_headers();//Write headers to the listing file

    //This outer loop goes through each row in the file parser
    line_number = 0;
    while (to_upper_string(parser->get_token(line_number, OPCODE)) != "START" && line_number < parser->size()) {
        string temp_label = parser->get_token(line_number, LABEL);
        string temp_opcode = parser->get_token(line_number, OPCODE);
        string temp_operand = parser->get_token(line_number, OPERANDS);

        if (temp_label != "" || temp_opcode != "" || temp_operand != "")
            throw symtab_exception("Invalid syntax before 'START' opcode on line " + line_number);
        line_number++;
        write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);
    }

    string temp_label = parser->get_token(line_number, LABEL);
    string temp_opcode = parser->get_token(line_number, OPCODE);
    string temp_operand = parser->get_token(line_number, OPERANDS);
    address = LOC_CTR;
    if (line_number == parser->size()) {
        throw symtab_exception("'START' directive not found");
    } else {
        program_name = parser->get_token(line_number, LABEL);
        LOC_CTR = format_address(parser->get_token(line_number, OPERANDS));
    }
    line_number++;
    write_to_file(line_number, address, temp_label, temp_opcode, temp_operand);
    address = LOC_CTR;

    while (line_number < parser->size()){
        string temp_label = parser->get_token(line_number, LABEL);
        string temp_opcode = parser->get_token(line_number, OPCODE);
        string temp_operand = parser->get_token(line_number, OPERANDS);
        if(is_assm_dir(to_upper_string(temp_opcode))){
            if(temp_opcode == "EQU"){
                if(temp_label == "")
                    throw symtab_exception("Invalid syntax on 'EQU' opcode on line " + line_number);
                if(symtable.symbol_exists(temp_label) == true)
                    throw symtab_exception("Label is already in use, reused on line " + line_number);
                symtable.add_symbol(temp_label, format_address(temp_operand));
            } else {
                if (temp_label != "") {
                    if (symtable.symbol_exists(temp_label) == true)
                        throw symtab_exception("Label is already in use, reused on line " + line_number);
                    symtable.add_symbol(temp_label, LOC_CTR);
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
            write_to_file(line_number, address, temp_label, temp_opcode, temp_operand);
            address = LOC_CTR;

        } else {
            if (temp_label != "") {
                if (symtable.symbol_exists(temp_label))
                    throw symtab_exception("Label is already in use, reused on line " + line_number);
                else symtable.add_symbol(temp_label,LOC_CTR);
            }
            //cout << "Temp label: "  << temp_label << " Temp Opcode: " << temp_opcode << " LOC_CTR: " << LOC_CTR << endl;
            if (temp_opcode == "") {
                line_number++;
                write_to_file(line_number, LOC_CTR, temp_label, temp_opcode, temp_operand);
            } else {
                size = opcodetable.get_instruction_size(temp_opcode);
                if (size == 0)
                    throw symtab_exception("Size of Opcode " + temp_opcode + " on line number " + to_string(line_number) + " not found");
                LOC_CTR = LOC_CTR + size;
                line_number = line_number + 1;
                write_to_file(line_number, address, temp_label, temp_opcode, temp_operand);
                address = LOC_CTR;
            }

        }
    }
    myfile.close();
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~Pass 1 Helper Functions~~~~~~~~~~~~~~~~~~~~~~~~~~


string sicxe_asm::to_string(int i) {
    ostringstream stream;
    stream << i;
    return stream.str();
}

int sicxe_asm::format_address(string str_addr) {
    bool is_hex = false;

    size_t found_hex = str_addr.find("$");
    if (found_hex != string::npos) {
        is_hex = true;
    }

    string temp = "";
    for (int i = 0; i < str_addr.length(); i++) {
        if (isdigit(str_addr[i]))
            temp += str_addr[i];
    }

    int answer = string_to_int(temp);

    if (is_hex) {
        int x;
        std::stringstream ss;
        ss << std::hex << temp;
        ss >> x;
        return x;
    } else {
        return answer;
    }
}

void sicxe_asm::write_headers() {
    string fileName = this->file_name;
    fileName.erase((fileName.end()-3),fileName.end());
    fileName.append("lis");
    myfile.open(fileName.c_str());
    string firstLine[] = {"Line#","Address","Label","Opcode","Operand","Machine Code"};
    string secondLine[] = {"=====","=======","=====","======","=======","============"};

    int i = 0;
    for (i = 0; i < 6; i++) {
        myfile << setw(10) << std::left << firstLine[i];
    }
    myfile << "\n";
    for (i = 0; i < 6; i++) {
        myfile << setw(10) << std::left << secondLine[i];
    }
    myfile << "\n";
}

void sicxe_asm::write_to_file(int line_num, int address, string label, string opcode, string operand){
    myfile << setw(10) << std::left << line_num;
    string hexaddr = int_to_hex(address, 5);
    myfile << setw(10) << std::left << hexaddr;
    myfile << setw(10) << std::left << label;
    myfile << setw(10) << std::left << opcode;
    myfile << setw(10) << std::left << operand;
    myfile << "\n";
}



bool sicxe_asm::is_assm_dir(string code) {
    bool found = false;
    for (int i = 0; i < 9; i++) {
        if (to_upper_string(code) == assembler_directives[i]) {
            found = true;
            return found;
        }
    }
    return found;
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

string sicxe_asm::to_string(int n) {
    ostringstream stream;
    stream << n;
    return stream.str();
}

int sicxe_asm::string_to_int(string i) {
    int n;
    sscanf(i.c_str(), "%d", &n);
    return n;
}

string sicxe_asm::int_to_hex(int num, int width) {
    stringstream out;
    out << setw(width) << setfill('0') << hex << num;
    return to_upper_string(out.str());
}

string sicxe_asm::format_headername(string name) {
    stringstream out;
    out<< setw(6) << setfill(' ') << name;
    return out.str();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Pass 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void sicxe_asm::pass_two(){
    line_number = 0;
    string machine_code = "";
    string header_record = "";

    //interate until start is found. Maybe make into method?
    while (to_upper_string(parser->get_token(line_number, OPCODE)) != "START" && line_number < parser->size()) {
        string temp_label = parser->get_token(line_number, LABEL);
        string temp_opcode = parser->get_token(line_number, OPCODE);
        string temp_operand = parser->get_token(line_number, OPERANDS);

        if (temp_label != "" || temp_opcode != "" || temp_operand != "")
            throw symtab_exception("Invalid syntax before 'START' opcode on line " + line_number);
        line_number++;

    }
    //linenumber now at start
    //creates correct header record for program
    string temp_label = parser->get_token(line_number, LABEL);
    string temp_opcode = parser->get_token(line_number, OPCODE);
    string temp_operand = parser->get_token(line_number, OPERANDS);
    int start_addrress = format_address(temp_operand);
    string start_add_hex = int_to_hex(start_addrress,6);
    int end_address = LOC_CTR;
    string end_add_hex = int_to_hex(end_address,6);
    string prog_name = format_headername(temp_label);
    header_record = "H"+prog_name +start_add_hex + end_add_hex;
    std::cout<< "Header record is " << header_record << endl;

    //while loop to get each part of machine code (by line) so we can add that line together
    while (line_number < parser->size()) {
        string temp_label = parser->get_token(line_number, LABEL);
        string temp_opcode = parser->get_token(line_number, OPCODE);
        string temp_operand = parser->get_token(line_number, OPERANDS);





        //We need to generate opcode table and serch to see if opcode exists
        //if(optab.opcode_exists(temp_opcode)== false)
        //    throw opcode_error_exception("opcode not found");
        //cout <<"opcode found " << temp_opcode << endl;

        //string opcode_bits = opcode_binary(temp_opcode);  will get first 6 bits in binary (before nixbpe)
        //string nixbpe = get_nixpbe(temp_operand);
        //string displacement = get_displacement(address, nixbpe, temp_operand); //address: need to get address into pass 2 --- nixbpe: if last digit is 1, it is format 4
        //machine_code = opcode_bits + nixbpe + displacement;
        //write machine_code to file
        line_number++;
    }

}

string sicxe_asm::opcode_binary(string opcode){
        string binary;
        string opcode_hex = optab.get_machine_code(opcode);
        binary = hex_to_bin(opcode_hex);
        binary = binary.substr(0,binary.size()-2);
}

string sicxe_asm::nixbpe(string operand) {
    string N = "0";
    string I = "0";
    string X = "0";
    string B = "0";
    string P = "0";
    string E = "0";
}

string sicxe_asm::get_displacement(string address, string nixbpe, string operand) {

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~Pass 2 Helper Functions~~~~~~~~~~~~~~~~~~~~~~~~~~~


string sicxe_asm::hex_to_bin(string hex) {
    hex = to_upper_string(hex);
    string final_binary = "";
    string get_binary = "";
    int i = 0;
    while(i < hex.size()){
        char hexval = hex.at(i);
        if(hexval == '0') get_binary = "0000";
        else if(hexval == '1') get_binary = "0001";
        else if(hexval == '2') get_binary = "0010";
        else if(hexval == '3') get_binary = "0011";
        else if(hexval == '4') get_binary = "0100";
        else if(hexval == '5') get_binary = "0101";
        else if(hexval == '6') get_binary = "0110";
        else if(hexval == '7') get_binary = "0111";
        else if(hexval == '8') get_binary = "1000";
        else if(hexval == '9') get_binary = "1001";
        else if(hexval == 'A') get_binary = "1010";
        else if(hexval == 'B') get_binary = "1011";
        else if(hexval == 'C') get_binary = "1100";
        else if(hexval == 'D') get_binary = "1101";
        else if(hexval == 'E') get_binary = "1110";
        else if(hexval == 'F') get_binary = "1111";
        final_binary = final_binary + get_binary;
        i++;
    }
    return final_binary;
}