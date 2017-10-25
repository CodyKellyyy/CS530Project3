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

void symtab::add_symbol(string key, int val) {
    m.insert(make_pair(key,val));
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
        return m.find(key)->second;
    }
}

void symtab::delete_symbol(string key) {
    if (symbol_exists(key)) {
        m.erase(key);
    }
}

void symtab::print_map() {
    for (m_iter = m.begin(); m_iter != m.end(); m_iter++)
        cout << "Symbol: " << m_iter->first << "\tAddress: " << m_iter->second << endl;
}