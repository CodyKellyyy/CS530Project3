/*opcode_error_exception.h
 *Samuel Akinmulero
 * 10/4/17
 * CS530 Riggins, Fall 2017
 * Program 2
 */

#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H

#include <string>

using namespace std;



class opcode_error_exception {

public:
    opcode_error_exception(string s) {
        message = s;
    }

    opcode_error_exception() {
        message = "An error has occurred. Opcode cannot be found.";
    }

    string getMessage() {
        return message;
    }
private:
    string message;
};

#endif