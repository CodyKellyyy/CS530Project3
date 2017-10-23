//
// Created by Edwin Coronado on 10/4/17.
//
#include <cstdlib>
#include <sstream>
#include "opcodetab.h"
#include "opcode_error_exception.h"

opcodetab::opcodetab() {
    try {
        if((get_codes_size() == get_bytes_size()) && (get_codes_size() == get_opcodes_size())) {
            for (int i = 0; i < get_codes_size(); i++)
                m.insert(pair<string, pair<int, int> >(codes[i], pair<int, int>(opcodes[i], bytes[i])));
        } else {
            throw opcode_error_exception("Array sizes for codes, opcodes, and bytes should be the same size.");
        }
    } catch (opcode_error_exception& e) {
        cerr << e.getMessage() << endl;
    }
}

string opcodetab::get_machine_code(string code) {
    try {
        code = remove_first_plus_sign(code);
        code = code = to_upper_string(code);
        if (opcode_exists(code)) {
            stringstream stream;
            stream << hex << m[code].first;
            string result(stream.str());
            return result;
        }
    } catch (opcode_error_exception& e) {
        cerr << e.getMessage() << endl;
    }

}

int opcodetab::get_instruction_size(string code) {
    try {
        code = remove_first_plus_sign(code);
        code = to_upper_string(code);
        if (opcode_exists(code)) {
            return m[code].first;
        }
    } catch (opcode_error_exception& e) {
        cerr << e.getMessage() << endl;
    }

}

int opcodetab::get_bytes_size() {
    return sizeof(bytes)/ sizeof(*bytes);
}

int opcodetab::get_codes_size() {
    return sizeof(codes)/ sizeof(*codes);
}

int opcodetab::get_opcodes_size(){
    return sizeof(opcodes)/ sizeof(*opcodes);
}

void opcodetab::print_map() {
    for (m_iter = m.begin(); m_iter != m.end(); m_iter++) {
        cout << "Code: " << m_iter->first << "\tOpcode: " << m_iter->second.first << "\tBytes: " << m_iter -> second.second << endl;
    }
}

bool opcodetab::opcode_exists(string code) {
    if (m.find(code) == m.end())
        throw opcode_error_exception("Opcode " + code + " was not found.");
    return true;
}

string opcodetab::to_upper_string(string s) {
    string temp = "";
    for (int i=0; i<s.length(); i++) {
        temp += toupper(s[i]);
    }
    return temp;
}

string opcodetab::remove_first_plus_sign(string in) {
    if (in.length() > 0 && in[0] == '+') {
        return in.substr(1, in.length());
    } else {
        return in;
    }
}