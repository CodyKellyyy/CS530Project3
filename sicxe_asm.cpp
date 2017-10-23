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

int main(int argc, char *argv[]){
    string fileName = argv[1];

    sicxe_asm::write_to_file(fileName);

}