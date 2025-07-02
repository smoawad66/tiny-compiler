#pragma once

#include <string>
using namespace std;

class TokenTypes {
public:
    inline static const string RESERVED = "Reserved word";
    inline static const string IDENTIFIER = "Identifier";
    inline static const string NUMBER = "Number";
    inline static const string ARITHMETIC_OP = "Arithmetic operator";
    inline static const string ASSIGNMENT_OP = "Assignment operator";
    inline static const string COMPARISON_OP = "Comparison operator";
    inline static const string UNTERMINATED_COMMENT = "Unterminated comment";
    inline static const string SEMICOLON = "Semicolon";
    inline static const string STRING = "String";
    inline static const string UNTERMINATED_STRING = "Unterminated string";
    inline static const string COMMA = "Comma";
    inline static const string PARENTHESIS = "Parenthesis";
    inline static const string UNKNOWN = "Unknown";
    inline static const string EOF_TOKEN = "EOF";
};
