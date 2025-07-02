#pragma once

#include<string>
using namespace std;

struct Token {
    
    string lexeme, type;
    int line, column;

    Token(string lexeme, string type, int line, int column);
};
