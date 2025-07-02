#pragma once

#include <vector>
#include <string>
#include <stack>
#include "Token.h"

using namespace std;

class Parser {
    
private:
    vector<Token> tokens;
    vector<string> errors;
    stack<string> scopes;
    int currentToken;

    Token current();
    void advance();
    bool match(const string& type);
    bool consume(const string& type);
    bool consumeLexeme(const string& lexeme);
    bool skipCheck();
    void error(const string& message, const char& whichLine, bool skip);

    void parseProgram();
    void parseStatement();
    void parseIf();
    void parseOption();
    void parseRepeat();
    void parseCondition();
    void parseExpression();
    void parseTerm();
    void parseNumber();
    void parseOperator();
    void parseIdentifier();
    void parseAssignment();
    void parseRead();
    void parseMultipleRead();
    void parseMultipleWrite();
    void parseWrite();

public:
    Parser(const vector<Token>& tokens);
    vector<string> parse();
};
