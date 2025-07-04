#include <string>
#include <initializer_list>
#include "../include/Helpers.h"

using namespace std;

bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isNumber(string s) {
    for(char i:s)
        if(!isDigit(i))
            return false;
    return true;
}
bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
bool isAlphaNum(char c) { return isDigit(c) || isAlpha(c); }
bool isArithmetic(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }
bool isComparison(char c) { return c == '=' || c == '<'; }
bool isParenthesis(char c) { return c == '(' || c == ')'; }

bool isReserved(string s)
{
    string a[] = {"if", "else", "then", "repeat", "until", "read", "write", "end"};
    for (string x : a)
        if (s == x)
            return true;
    return false;
}