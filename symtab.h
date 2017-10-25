/* symtab.h
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#ifndef SYMTAB_H
#define SYMTAB_H
#include <iostream>
#include <map>
#include <string>
#include "symtab_exception.h"

using namespace std;

class symtab {

public:

    //create table
    symtab();

    //add symbol/value pair to table
    void add_symbol(string sym, string val);

    //check if symbol is in table
    bool find_symbol(string sym);

    //get and return value associated with symbol
    string get_value(string sym);

    //delete symbol/value pair
    void delete_symbol(string sym, string val);

private:

    map<string, string>> m;
    map<string, string>>::iterator m_iter;

    string to_upper_string(string);
    int hex_to_int(string);
    int string_to_int(string);
    string int_to_hex(string);
    string int_to_string(string);

    //print table
    void print();
};

#endif