# Tiny Language

## Overview
This is an implementation of Tiny programming language compiler

## Character Classes
- **Digit**: `[0-9]`
- **Letter**: `[a-zA-Z]`
- **Any character**: `.`

## Token Types

### Identifiers
- **Pattern**: `letter(letter|digit)*`
- **Description**: Variable names and identifiers

### Numbers
- **Pattern**: `digit(digit)*`
- **Description**: Integer values

### Reserved Words
- **Keywords**: `if`, `then`, `else`, `repeat`, `until`, `end`, `read`, `write`

### Operators
#### Arithmetic Operators
- `+`, `-`, `*`, `/`

#### Comparison Operators
- `<`, `=`

### Special Symbols
- **Parentheses**: `(`,`)`
- **Assignment Operator**: `:=`
- **Statement Terminator**: `;`

### Comments and Strings
- **Comments**: `{[^}].*}` any combination of characters between `{}` except `}`
- **Strings**: `"[^"].*"` any combination of characters between `""` except `"`

## Complete Regular Expression
The complete regular expression pattern for Tiny language tokens is:

```
  letter(letter|digit)* | 
  digit(digit)* | 
  if|then|else|repeat|until|end|read|write | 
  +|-|*|/ | 
  <|= | 
  (|) | 
  := | 
  ; | 
  {[^}].*} | 
  "[^"].*"
```

## Token Categories
Tiny language tokens can be one of the following:
1. Identifiers
2. Numbers
3. Reserved words
4. Arithmetic operators
5. Comparison operators
6. Parentheses
7. Assignment operator
8. Semicolon
9. Comments
10. Strings
