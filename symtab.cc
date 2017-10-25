/* symtab.cc
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/
#include <iostream>
#include <map>
#include <string>
#include "file_parser.h"
#include "symtab_exception.h"
#include "symtab.h"

symtab::symtab(){}

void symtab::add_symbol(string key, string val) {
    m.insert(key,val);
}

bool symtab::symbol_exists(string key) {
    if (m.find(key) == m.end()) {
        return false;
    } else {
        return true;
    }
}

int symtab::get_value(string key) {
    if (symbol_exists(key)) {
        return m[key];
    }
}

void symtab::delete_symbol(string key) {

}