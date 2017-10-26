/* sicxe_asm.cpp
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include "sicxe_asm.h"

int main(int argc, char *argv[]){
    //    if(argc != 2) {
//        cout << "Error, you must supply the name of the file " <<
//             "to process at the command line." << endl;
//        exit(1);
//    }
    string filename = "/Users/edwin.coronado/CLionProjects/SIC-XE_Tokenizer/source4.txt";
    try {
        file_parser parser(filename); //Initialize file_parser object
        symtab symbtab; //Initialize Symbol Table object

        parser.read_file(); //Read the file (tokenize it)

        sicxe_asm::parse_rows(parser);


    }
    catch(file_parse_exception excpt) {
        cerr << "**Sorry, error: " << excpt.getMessage() << endl;
    }

}

sicxe_asm::sicxe_asm() {}

void sicxe_asm::parse_rows(file_parser parser) {

    //This outer loop goes through each row in the file parser
    for (int i = 0; i < parser.size(); i++) {
        if (parser.get_token(i, OPCODE) != "END") {
            //LOCCTR = format_address(parser.get_token(i,OPERANDS));
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
    vector<string> assembler_directives;
    assembler_directives.push_back("START");
    assembler_directives.push_back("END");
    assembler_directives.push_back("BYTE");
    assembler_directives.push_back("WORD");
    assembler_directives.push_back("RESB");
    assembler_directives.push_back("RESW");
    assembler_directives.push_back("BASE");
    assembler_directives.push_back("NOBASE");

    if (find(assembler_directives.begin(), assembler_directives.end(), code) != assembler_directives.end()) {
        return true;
    } else {
        return false;
    }
}

//void sicxe_asm::add_to_symtab(string address, string label, string operand) {
//    symtab symbol;
//    symbol.add_symbol(string(label),int(0));
//
//}

