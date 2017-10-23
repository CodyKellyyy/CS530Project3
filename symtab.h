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
    void add_symbol(string sym, int val);

    //check if symbol is in table
    bool check_symbol(string sym);

    //get and return value associated with symbol
    string get_value(string sym);

    //delete symbol/value pair
    void delete_symbol(string sym, int val);

    //print table
    void print();
    


private:

    map<string, int> > m;
    map<string, int> >::iterator m_iter;

    string to_upper_string(string);
    string hex_to_dec(string);
    string dec_to_hex(string);
    string int_to_string(string);
    string string_to_int(string);

};

#endif