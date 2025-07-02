#pragma once

#include <string>
using namespace std;

class ErrorMessages {
public:
    inline static const string EXPECTED_STATEMENT = "Expected statement (if, repeat, read, write, or assignment)";
    inline static const string EXPECTED_THEN = "Expected 'then' keyword after condition in if statement";
    inline static const string EXPECTED_END_ELSE = "Expected 'end' or 'else' keyword after if statement body";
    inline static const string EXPECTED_END = "Expected 'end' keyword to close if-else statement";
    inline static const string EXPECTED_REPEAT = "Expected 'repeat' keyword";
    inline static const string EXPECTED_UNTIL = "Expected 'until' keyword in repeat statement";
    inline static const string EXPECTED_SEMICOLON = "Expected ';' after statement";
    inline static const string EXPECTED_COMPARISON_OP = "Expected comparison operator (< or =) in condition";
    inline static const string EXPECTED_RPAREN = "Expected right parenthesis!";
    inline static const string EXPECTED_EXPRESSION = "Expected expression";
    inline static const string EXPECTED_NUMBER = "Expected number";
    inline static const string EXPECTED_ID = "Expected identifier";
    inline static const string EXPECTED_ID_NUMBER = "Expected identifier or number";
    inline static const string UNEXPECTED_IDENTIFIER = "Unexpected identifier";
    inline static const string EXPECTED_ASSIGN = "Expected ':=' in assignment statement";
    inline static const string EXPECTED_READ = "Expected 'read' keyword";
    inline static const string EXPECTED_WRITE = "Expected 'write' keyword";
    inline static const string EXPECTED_COMMA = "Expected comma!";
    inline static const string EXPECTED_ARITHMETIC_OP = "Expected arithmetic operator";
};
