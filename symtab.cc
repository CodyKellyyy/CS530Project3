/* symtab.cc
 * Thomas Valenta
 * 10/22/17
 * CS530 Riggins, Fall 2017
 * Program 3
*/
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <iomainip>
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
    if(!find_symbol(sym))
        throw symtab_exception("Value does not exist.");

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
//Converts string to uppercase
string symtab::to_upper_string(str) {
    transform(str.begin(), str.end(), str.begin(),::toupper);
    return string;
}
//Converts integers to hex values
string symtab::int_to_hex(int dec, int width ) {
    stringstream stream;
    stream << setfill('0') << setw(width) << hex << dec;
    return to_uppercase(stream.str());  //Returns uppercase hex letters


}
//Converts hexadecimal values to decimal values
int symtab:: hex_to_int(string str){
    int v;
    sscanf(c_str(), "%x", &v);
    return v;
}
//Converts integer to string
string symtab:: int_to_string(string num){
    ostringstream ss;
    ss << num;
    return ss.str();
}