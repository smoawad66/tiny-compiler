#include <vector>
#include <stack>
#include "../include/Token.h"
#include "../include/TokenTypes.h"
#include "../include/ErrorMessages.h"
#include "../include/Lexemes.h"
#include "../include/Parser.h"
#include "../include/Helpers.h"

Parser::Parser(const vector<Token>& tokens) : tokens(tokens), currentToken(0) {}


Token Parser::current() {
    if (currentToken < tokens.size()) {
        return tokens[currentToken];
    }
    return Token("", "EOF", -1, -1);
}


void Parser::advance() {
    if (currentToken < tokens.size()) {
        currentToken++;
    }
}


bool Parser::match(const string& type) {
    return current().type == type;
}


bool Parser::consume(const string& type) {
    if (match(type)) {
        return advance(), true;
    }
    return false;
}


bool Parser::consumeLexeme(const string& lexeme) {
    if (current().lexeme == lexeme) {
        return advance(), true;
    }
    return false;
}


bool Parser::skipCheck() {
    if (match(TokenTypes::RESERVED) || match(TokenTypes::IDENTIFIER) || current().lexeme == ";") {
        return false; // don't skip
    }

    return true; // skip
}


void Parser::error(const string& message, const char& whichLine = 'c', bool skip = true) {
    
    int line, column = current().column;

    if (column == -1) {
        column = tokens.back().column + tokens.back().lexeme.length();
    }
    
    if(whichLine == 'c') {
        line = current().line;
        if (line == -1)line = tokens.back().line;
    }
    
    else if (whichLine == 'p') {
        line = tokens[currentToken-1].line;
        column = tokens[currentToken-1].column + tokens[currentToken-1].lexeme.length();
    }
    
    errors.push_back("Line " + to_string(line) + ", Column " + to_string(column) +  ": " + message);
    
    if (!skip) return;
    
    while(currentToken < tokens.size() && skipCheck()) advance();
    
    consumeLexeme(";");

}


void Parser::parseProgram() {

    if (!scopes.empty()) {
        while(inArray(current().lexeme, {Lexemes::THEN, Lexemes::ELSE, Lexemes::END}) && scopes.top() != Lexemes::IF || current().lexeme == Lexemes::UNTIL && scopes.top() != Lexemes::REPEAT)
            error(ErrorMessages::EXPECTED_STATEMENT, 'c', false), advance();
        
        if (inArray(current().lexeme, {Lexemes::THEN, Lexemes::ELSE, Lexemes::END}) && scopes.top() == Lexemes::IF || current().lexeme == Lexemes::UNTIL && scopes.top() == Lexemes::REPEAT)
            return;
    }

    parseStatement();
    
    if (currentToken < tokens.size()) {
        parseProgram();
    }
}


void Parser::parseStatement() {
    
    if (match(TokenTypes::NUMBER) && currentToken+1 < tokens.size()) {
        Token c = current(), n = tokens[currentToken+1];           
        if(n.type == TokenTypes::IDENTIFIER && c.column+c.lexeme.length() == n.column) {
            currentToken+=2;
            error(ErrorMessages::UNEXPECTED_IDENTIFIER, 'p', true);
        }
    }

    string lexeme = current().lexeme;

    if (lexeme == Lexemes::IF) parseIf();
    else if (lexeme == Lexemes::REPEAT) parseRepeat();
    else if (lexeme == Lexemes::READ) parseRead();
    else if (lexeme == Lexemes::WRITE) parseWrite();
    else if (match(TokenTypes::IDENTIFIER)) parseAssignment();
    else if (match(TokenTypes::UNTERMINATED_COMMENT)) error(current().type);
    else error(ErrorMessages::EXPECTED_STATEMENT);
}


void Parser::parseIf() {
    
    scopes.push(Lexemes::IF);

    advance();

    parseCondition();
    
    if (!consumeLexeme(Lexemes::THEN)) {
        error(ErrorMessages::EXPECTED_THEN);
    }
    
    parseProgram();

    
    parseOption();

    scopes.pop();
}


void Parser::parseOption() {

    if (consumeLexeme(Lexemes::ELSE)) {
        parseProgram();
        if (!consumeLexeme(Lexemes::END)) {
            error(ErrorMessages::EXPECTED_END);
        }
    } 
    else if (!consumeLexeme(Lexemes::END)) {
        error(ErrorMessages::EXPECTED_END_ELSE);
    }
}


void Parser::parseRepeat() {

    scopes.push(Lexemes::REPEAT);

    if (!consumeLexeme(Lexemes::REPEAT)) {
        error(ErrorMessages::EXPECTED_REPEAT);
    }
    
    parseProgram();
            
    if (!consumeLexeme(Lexemes::UNTIL)) {
        error(ErrorMessages::EXPECTED_UNTIL);
    }

    parseCondition();

    if (!consumeLexeme(";") && tokens[currentToken-1].lexeme!=";") {
        error(ErrorMessages::EXPECTED_SEMICOLON, 'p');
    }
    scopes.pop();
}


void Parser::parseCondition() {
    parseExpression();
    
    if (consumeLexeme("<")) {
        parseExpression();
    } else if (consumeLexeme("=")) {
        parseExpression();
    } else {
        char w = tokens[currentToken-1].line != current().line ? 'p' : 'c';
        error(ErrorMessages::EXPECTED_COMPARISON_OP, w);
    }
}


void Parser::parseExpression() {

    if (consumeLexeme("(")) {
        parseExpression();
        if (!consumeLexeme(")")) 
            error(ErrorMessages::EXPECTED_RPAREN);
    }

    else if (match(TokenTypes::IDENTIFIER) || match(TokenTypes::NUMBER)) {
        parseTerm();
    }

    else{
        error(ErrorMessages::EXPECTED_EXPRESSION);
    }
    
    
    if (match(TokenTypes::ARITHMETIC_OP)) {
        advance();
        parseExpression();
    }
}


void Parser::parseTerm() {
    if (match(TokenTypes::IDENTIFIER)) {
        parseIdentifier();
    } else if (match(TokenTypes::NUMBER)) {
        parseNumber();
    } else {
        error(ErrorMessages::EXPECTED_ID_NUMBER);
    }
}


void Parser::parseNumber() {
    if (!consume(TokenTypes::NUMBER)) {
        error(ErrorMessages::EXPECTED_NUMBER);
    }
}


void Parser::parseOperator() {
    if (!consume(TokenTypes::ARITHMETIC_OP)) {
        error(ErrorMessages::EXPECTED_ARITHMETIC_OP);
    }
}


void Parser::parseIdentifier() {
    if (!consume(TokenTypes::IDENTIFIER)) {
        error(ErrorMessages::EXPECTED_ID);
    }
}


void Parser::parseAssignment() {
    parseIdentifier();
    
    if (!consumeLexeme(":=")) {
        error(ErrorMessages::EXPECTED_ASSIGN, 'p', false);
        advance();
    }
    
    parseExpression();
    
    if (!consumeLexeme(";") && tokens[currentToken-1].lexeme!=";") {
        return error(ErrorMessages::EXPECTED_SEMICOLON, 'p');
    }

}


void Parser::parseRead() {
    if (!consumeLexeme(Lexemes::READ)) {
        error(ErrorMessages::EXPECTED_READ);
    }
    
    parseMultipleRead();
    
    if (!consumeLexeme(";") && tokens[currentToken-1].lexeme!=";") {
        error(ErrorMessages::EXPECTED_SEMICOLON, 'p');
    }
}
    

void Parser::parseMultipleRead() {
    parseIdentifier();

    if (match(TokenTypes::SEMICOLON) || current().line > tokens[currentToken-1].line) return;
    if (match(TokenTypes::IDENTIFIER)) error(ErrorMessages::EXPECTED_COMMA, 'c', false);

    consume(TokenTypes::COMMA);

    if (currentToken < tokens.size()) {
        parseMultipleRead();
    }
}

void Parser::parseMultipleWrite() {

    if (current().type == TokenTypes::STRING) advance();
    else if (current().type == TokenTypes::UNTERMINATED_STRING) error(current().type, 'c', false), advance();
    else parseIdentifier();

    if (match(TokenTypes::SEMICOLON) || current().line > tokens[currentToken-1].line || match(TokenTypes::RESERVED)) return;
    if (match(TokenTypes::IDENTIFIER)) error(ErrorMessages::EXPECTED_COMMA, 'c', false);

    consume(TokenTypes::COMMA);

    if (currentToken < tokens.size()) {
        parseMultipleWrite();
    }
    
}


void Parser::parseWrite() {
    if (!consumeLexeme(Lexemes::WRITE)) {
        error(ErrorMessages::EXPECTED_WRITE);
    }

    parseMultipleWrite();

    if (!consumeLexeme(";") && tokens[currentToken-1].lexeme!=";") {
        error(ErrorMessages::EXPECTED_SEMICOLON, 'p');
    }
}


vector<string> Parser::parse() {
    if(tokens.size()) {
        parseProgram();
    }
    return errors;
}