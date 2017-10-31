//
// Created by Edwin Coronado on 9/25/17.
//
#include "file_parser.h"
#include "file_parse_exception.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <iomanip>

using namespace std;

file_parser::file_parser(string in) {
    filename = in;
    length = 0;
    rows.clear();
}

file_parser::~file_parser(){}

void file_parser::read_file() {
    ifstream stream;
    try {
        string line = "";
        stream.open(filename.c_str(), ios::in);
        if (!stream)
            throw file_parse_exception("Could not open file: " + filename + "\nPlease make sure you are entering full path to the file.");
        while (getline(stream, line)) {
            try {
                replace_all(line, "\t", " ");
                row temp(line);
                rows.push_back(temp);
                ++length;
            } catch (file_parse_exception& e) {
                cerr << e.getMessage() << size()+1 << endl;
                exit(EXIT_FAILURE);
            }
        }
    } catch (file_parse_exception& e) {
        cerr << e.getMessage() << endl;
        exit(EXIT_FAILURE);
    }
}

string file_parser::get_token(unsigned int row, unsigned int column) {
    if (row >= size()) {
        throw file_parse_exception("Requested token at location from nonexistent line: " + to_string(row));
    }
    else if (column > 3) {
        throw file_parse_exception("Can only get token in columns 0-3. You have requested token at location");
    } else {
        return rows.at(row).getToken(column);
    }
}

void file_parser::print_file() {
    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < 4; j++) {
            try {
                cout <<   std::setw(35)<< std::setfill(' ')<< std::left  << get_token(i, j);
            } catch (file_parse_exception& e) {
                cerr << e.getMessage() << endl;
                exit(EXIT_FAILURE);
            }
        }
        cout << endl;
    }
}

int file_parser::size() {
    return length;
}

// Define "row" functions
file_parser::row::row(string line) {
    // TODO: here is where the parsing must happen.
    // Use row.addToken() (look at code below) to add what you parsed
    label = opcode = operands = comments = "";


    unsigned long space_index = line.find_first_not_of(" ", 0);
    unsigned long comment_index = line.length();
    //This is for the comment
    if (line.find('.') != string::npos) {
        comment_index = line.find_first_of('.', 0);
        addToken(3, line.substr(comment_index, line.length()));
    }

    // This is the start for Label extraction //
    unsigned long index = line.find_first_not_of(" ", 0);
    if (index < comment_index && space_index == 0) {
        unsigned long first_label = line.find_first_of(" ", index);
        index = addToken(0, line.substr(index, first_label - index));
    }

    //This is for the opcode extraction
    unsigned long opcode_index_first = line.find_first_not_of(" ", index);
    if (opcode_index_first < comment_index) {
        unsigned long opcode_index_last = line.find_first_of(" ", opcode_index_first);
        index = addToken(1, line.substr(opcode_index_first, opcode_index_last - opcode_index_first)) + opcode_index_first;
    }

    unsigned long operands_index_first = line.find_first_not_of(" ", index);

    if (operands_index_first < comment_index){
        long operands_index_last = line.find_first_of(" ", operands_index_first);
        if(line.find('\'', operands_index_first) != string ::npos){
            string temp = line.substr(operands_index_first, line.length());
            if (count(temp.begin(), temp.end(), '\'') != 2)
                throw file_parse_exception("Mismatched quote at line ");

            long quote_index_last = line.find_last_of("'", operands_index_first);
            index = addToken(2, line.substr(operands_index_first, quote_index_last - operands_index_first)) + operands_index_first;
            if (getToken(2).at(getToken(2).length()-1) != '\'' && getToken(2).at(getToken(2).length()-1) != ' ')
                throw file_parse_exception("Too many tokens at line ");
        } else {
            index = addToken(2, line.substr(operands_index_first, operands_index_last - operands_index_first)) + operands_index_first;
        }
        //Check that if there is a token after the operand, it's a comment token.
        long comment_test = line.find_first_not_of(" ", index);
        if (comment_test != string::npos && getToken(3).empty())
            throw file_parse_exception("Too many tokens at line ");
    }


}


file_parser::row::~row(){}

int file_parser::row::getSize() {
    return size;
}

string file_parser::row::getToken(unsigned int index) {
    switch (index) {
        case 0 : return label;
        case 1 : return opcode;
        case 2 : return operands;
        case 3 : return comments;
        default: {
            cerr << "Index for row should be between 0 and 3" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

unsigned long file_parser::row::addToken(unsigned int index, string s) {
    switch (index) {
        case 0 : {
            string temp = s;
            if (s.length() >= 8) {
                temp = s.substr(0,8);
            }
            if (isalpha(temp[0]) == 0 || string_is_alpha_num(temp) == 0) {
                throw file_parse_exception("Error while parsing Label on line ");
            } else{
                label = temp;
                return s.length();
            }
        }
        case 1 : {
            try {
                opcode = s;
                return opcode.length();
            } catch (file_parse_exception& e) {
                throw file_parse_exception("Error while parsing Opcode on line ");
            }
        }
        case 2 : {
            try {
                operands = s;
                return operands.length();
            } catch (file_parse_exception& e) {
                throw file_parse_exception("Error while parsing Operands on line ");
            }
        }
        case 3 : {
            try {
                comments = s;
                return comments.length();
            } catch (file_parse_exception& e) {
                throw file_parse_exception("Error while parsing Comments on line ");
            }
        }
        default: {
            cerr << "Index must be between 0 and 3" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

int file_parser::row::string_is_alpha_num(string str) {
    int temp = 1;
    int x = 0;
    for (int i=0; i<str.length() && i<7; i++) {
        temp = std::isalnum(str.at(i));
        if(temp == 0)
            x += 1;

    }
    if (x != 0)
        return 0;
    else
        return 1;
}

void file_parser::replace_all(string& source, string const& in, string const& out)
{
    for(string::size_type i = 0; (i = source.find(in, i)) != string::npos;)
    {
        source.replace(i, in.length(), out);
        i += out.length();
    }

string file_parser::to_string(int) {

}
}