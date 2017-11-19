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
#include "file_parser.h"
#include "file_parse_exception.h"
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

int main(int argc, char *argv[]){

    string filename = "C:\\Users\\Samuel (Yomi)\\CLionProjects\\CS530Project3\\source1.txt";
    //string filename = argv[1];
    try {
        sicxe_asm assembler(filename);
        assembler.pass_one();
        assembler.pass_two();
        cout << "EDWIN the flags are: " << assembler.flags_to_int() << endl;
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
    load_registers();
    load_flags();
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
                size = optab.get_instruction_size(temp_opcode);
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

unsigned int sicxe_asm::format_address(string str_addr) {
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

    pass_one_record temp_rec(line_num, address, label, opcode, operand);
    pass_one_tab.push_back(temp_rec);
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

unsigned int sicxe_asm::string_to_int(string i) {
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

    bool start = false;
    /* This for loop will iterate through the "pass_one_tab" vector. Which
     * hold the data that was generated by pass_one() */
    for (unsigned int i = 0; i < pass_one_tab.size(); i++) {
        /* The following cout statement loops through the "pass_one_tab" vector.
         * The output of it is the same as the content of the listing file
         * generated by pass_one(). Look at it to learn how to access the
         * variables. You can uncomment it to debug*/
     //   cout << "Line: " << pass_one_tab.at(i).line << " "
      //       << "Address: " << int_to_hex(pass_one_tab.at(i).address, 5) << " "
      //       << "Label: " << pass_one_tab.at(i).label << " "
      //       << "Opcode: " << pass_one_tab.at(i).opcode << " "
      //       << "Operand: " << pass_one_tab.at(i).operand << " "
      //       << "Machine code: " << pass_one_tab.at(i).machine_code << endl;

        /* NOTE: all integers are being treated as unsigned integers,
         * all strings are converted to uppercase. Please keep this in
         * mind as you parse the code */

        unsigned int machine_code = 0;
        unsigned int temp_address = pass_one_tab.at(i).address;
        string temp_label = pass_one_tab.at(i).label;
        string temp_opcode = to_upper_string(pass_one_tab.at(i).opcode);
        string temp_operand = to_upper_string(pass_one_tab.at(i).operand);

        //Set the 'start' boolean to true. Also set PC reg.
        if (temp_opcode == "START") {
            start = true;
        }

        if (start) {

            /* If the opcode is "WORD", then just save the operand as the machine code
             * Note that we pass it through format_address() to determine if it's hex or dec */
            if (temp_opcode == "WORD") {
                machine_code = format_address(pass_one_tab.at(i).operand);
            } else if (temp_opcode == "BYTE") {

            }


            if (temp_opcode == "WORD" || temp_opcode == "BYTE") {
                switch (tolower(temp_operand[0])) {
                    case 120: { // 120 is decimal ASCII for 'x'

                        break;
                    }
                    case 99: { // 99 is decimal ASCII for 'c'

                        break;
                    }
                    default: {

                        break;
                    }
                }


            }

        }
    }
}

//Structure of what we have to do to get the object code

//void sicxe_asm::format1_objectCode(){
//    string objectcode = optab.get_machine_code(opcode);
//}
//
//void sicxe_asm::format2_objectCode(){
//
//}
//
//void sicxe_asm::format3_objectCode(){
//    if(temp_opcode[0] == "+"){
//        format4_objectCode();
//        return;
//    }
//    if(temp_operand[0] == "@"){
//        //write code
//    }
//
//    if(temp_operand[0] == "#"){
//        //write code
//    }
//
//}
//
//void sicxe_asm::format4_objectCode(){
//
//}

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

//Gets the displacement for addresses
int sicxe_asm::get_displacement(int firstaddr, int secondaddr) {
    int disp_base = firstaddr - baseAddr;
    int disp = firstaddr - secondaddr;

    if(disp >= -2048 && disp <= 2047){
        set_flag('p', 1);
        return disp;
    }
    else if(disp_base >= 0 && disp_base <= 4095){
        set_flag('b', 1);
        return disp_base;
    }

    return 0;
}

//string sicxe_asm::get_displacement(string address, string nixbpe, string operand) {
//
//}


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

int sicxe_asm::hex_string_to_int(string s) {
    istringstream converter(s);
    unsigned int value;
    converter >> std::hex >> value;
    return value;
}

/* This function does a bitwise manipulation to remove
 * the last two bits of the opcode */
unsigned int sicxe_asm::opcode_to_6_bit(unsigned int op) {
    unsigned int mask = 0xFC000000;
    return op & mask;
}


// NOTE: DON'T modify the "registers" map directly. Use the functions below.
void sicxe_asm::load_registers() {
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("A", pair<int, int>(0, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("X", pair<int, int>(1, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("L", pair<int, int>(2, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("PC", pair<int, int>(8, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("SW", pair<int, int>(9, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("B", pair<int, int>(3, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("S", pair<int, int>(4, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("T", pair<int, int>(5, 0)));
    registers.insert(pair<string, pair<unsigned int, unsigned int> >("F", pair<int, int>(6, 0)));
}

// Sets the register's value. i.e.: set_reg_value("PC", 1232);
void sicxe_asm::set_reg_value(string reg, unsigned int val) {
    registers[reg].second = val;
}

// Gets the register's number.
int sicxe_asm::get_reg_number(string reg) {
    return registers[reg].first;
}

// Gets the register's value.
int sicxe_asm::get_reg_value(string reg) {
    return registers[reg].second;
}

// NOTE: DON'T modify the "flags" map directly. Use the functions below.
void sicxe_asm::load_flags() {
    flags.insert(pair<char, unsigned int> ('n', 1));
    flags.insert(pair<char, unsigned int> ('i', 0));
    flags.insert(pair<char, unsigned int> ('x', 1));
    flags.insert(pair<char, unsigned int> ('b', 0));
    flags.insert(pair<char, unsigned int> ('p', 1));
    flags.insert(pair<char, unsigned int> ('e', 0));
}

// Use this method to set a flag, i.e: set_flag('n', 1);
void sicxe_asm::set_flag(char flag, unsigned int value) {
    flags[flag] = value;
}

// Use this method to get the value of the flag. i.e.: get_flag('n');
unsigned int sicxe_asm::get_flag(char flag) {
    return flags[flag];
}

/* This function turns the "flags" map into
 * an unsigned int with the following form:
 * 000000nixbpe00000000000000000000 */
unsigned int sicxe_asm::flags_to_int() {
    unsigned int answer = 0;
    answer |= (get_flag('n') << 25);
    answer |= (get_flag('i') << 24);
    answer |= (get_flag('x') << 23);
    answer |= (get_flag('b') << 22);
    answer |= (get_flag('p') << 21);
    answer |= (get_flag('e') << 20);
    return answer;
}




