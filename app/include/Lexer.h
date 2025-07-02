#pragma once

#include <vector>
#include "Token.h"

class Lexer{

private:
    vector<Token>tokens;
    string code;

public:
    Lexer(string code);
    vector<Token> tokenize();
};
