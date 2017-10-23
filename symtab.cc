/* symtab.cc
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/

#include <cstdlib>
#include <sstream>
#include "symtab.h"
#include "symtab_exception.h"

symtab::symtab(){

}

void symtab::add_symbol(string sym, int val) {
    if(!find_symbol(sym))
        m.insert(sym, val);
    else
        throw symtab_exception("Symbol has already been defined.");
}

bool symtab::find_symbol(string sym) {
    if(m.find(sym) == m.end())
        return false;
    return true;
}

string symtab::get_value(string sym) {

}

void symtab::delete_symbol(string sym, int val) {
    if(find_symbol(sym))
        m.erase(sym);
    else
        throw symtab_exception("Symbol does not exist.");

}

void symtab::print() {
    for(auto m_iter = m.begin(); m_iter != m.end(); ++m_iter)
        std::cout << m_iter->first << " " << m_iter->second << '\n';
}

//Converts from a string to an integer value
int symtab::string_to_int(string str) {
    istringstream inpstr(str);
    int h;
    inpstr >> h;
    return h;
}

string symtab::to_upper_string(string) {

}
