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
#include "file_parser.h"
#include "symtab_exception.h"

using namespace std;

class symtab {

public:

    //create table
    symtab();

    //add symbol/value pair to table
    void add_symbol(string,string);

    //check if symbol is in table
    bool symbol_exists(string);

    //get and return value associated with symbol
    int get_value(string);

    //delete symbol/value pair
    void delete_symbol(string);

private:
    map<string,int> m;
    map<string,int>::iterator m_iter;




};

#endif