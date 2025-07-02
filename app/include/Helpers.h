#pragma once

#include <string>
#include <initializer_list>

using namespace std;

bool isDigit(char c);
bool isNumber(string s);
bool isAlpha(char c);
bool isAlphaNum(char c);
bool isArithmetic(char c);
bool isComparison(char c);
bool isParenthesis(char c);
bool isReserved(string s);

template<typename T>
bool inArray(const T& target, initializer_list<T> list) {
    for (const T& item : list)
        if (item == target)
            return true;
    return false;
}