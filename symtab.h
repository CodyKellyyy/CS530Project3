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

    void add_symbol(string, string, bool);
    


private:

    map<string, pair<string, bool> > m;
    map<string, pair<string, bool> >::iterator m_iter;


};

#endif