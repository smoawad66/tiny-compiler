#include <vector>
#include "../include/Token.h"
#include "../include/Helpers.h"
#include "../include/Lexer.h"
#include "../include/TokenTypes.h"

#define eb emplace_back

Lexer::Lexer(string code):code(code){}

vector<Token> Lexer::tokenize() {
    
    code+='\\';
    string t;
    int l = code.length(), line = 1, column = 1;

    for (int i = 0; i < l-1 ; i++) {
        if(isDigit(code[i])) {
            while(isDigit(code[i]) && i < l-1) t+=code[i++];
            tokens.eb(t, TokenTypes::NUMBER, line, column), column+=t.length(), t="", i--;
        }
        
        else if(isAlpha(code[i])) {
            while(isAlphaNum(code[i]) && i < l-1) t+=code[i++];
            tokens.eb(t, isReserved(t) ? TokenTypes::RESERVED : TokenTypes::IDENTIFIER, line, column), column+=t.length(), t="", i--;
        }

        else {
            if(code[i] == '{') {
                t="{", i++;
                while(code[i] != '}' && i < l-1)t+=code[i++];
                if (code[i] != '}')tokens.eb(t, TokenTypes::UNTERMINATED_COMMENT, line, column);
                column += t.length() + (code[i] == '}');
            }
            
            else if (code[i]=='"') {
                t="\"", i++;
                while(code[i]!='"' && i < l-1)t+=code[i++];
                code[i] == '"' ? tokens.eb(t+"\"", TokenTypes::STRING, line, column) : tokens.eb(t, TokenTypes::UNTERMINATED_STRING, line, column);
                column += t.length() + (code[i] == '"');
            }
            
            else if(code[i]==':' && code[i+1]=='=') {
                tokens.eb(":=", TokenTypes::ASSIGNMENT_OP, line, column);
                i++;
                column +=2;
            }

            else {
                if(isArithmetic(code[i])) tokens.eb(string(1, code[i]), TokenTypes::ARITHMETIC_OP, line, column);
                else if(isComparison(code[i])) tokens.eb(string(1, code[i]), TokenTypes::COMPARISON_OP, line, column);
                else if(isParenthesis(code[i])) tokens.eb(string(1, code[i]), TokenTypes::PARENTHESIS, line, column);
                else if(code[i]==';') tokens.eb(";", TokenTypes::SEMICOLON, line, column);
                else if(code[i]==',') tokens.eb(",", TokenTypes::COMMA, line, column);
                else if(code[i]=='\n') line++, column=0;
                else if (code[i] == ' ') {column++; continue;}
                else tokens.eb(string(1, code[i]), TokenTypes::UNKNOWN, line, column);
                column++;
            }

            t="";
            continue;
        }
    }    

    return tokens;
}