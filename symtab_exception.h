/* symtab_exception.h
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program3
*/

#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_ERROR_EXCEPTION_H

#include <string>

using namespace std;

class symtab_exception {

public:
    symtab_exception(string s) {
        message = s;
    }

    symtab_exception() {
        message = "Symtab has encountered an error.";
    }

    string getMessage() {
        return message;
    }

private:
    string message;
};

#endif