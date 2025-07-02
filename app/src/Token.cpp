#include <string>
#include "../include/Token.h"
using namespace std;

Token::Token(string lexeme, string type, int line, int column) {
    this->lexeme = lexeme;
    this->type = type;
    this->line = line;
    this->column = column;
}
