/* sicxe_asm.cpp
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include "sicxe_asm.h"

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

//void sicxe_asm::add_to_symtab(string address, string label, string operand) {
//    symtab symbol;
//    symbol.add_symbol(string(label),int(0));
//
//}

int main(int argc, char *argv[]){
    //string fileName = argv[1];
    symtab symtab1;
    symtab1.add_symbol("EDWIN", 17);
    symtab1.add_symbol("MCFLURRY", 24);

    symtab1.print_map();

    symtab1.delete_symbol("EDWIN");

    symtab1.print_map();



}