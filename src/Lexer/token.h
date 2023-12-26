#ifndef TOKEN_H
#define TOKEN_H

#include "lexer_T.h"

typedef struct
{
  char* value;
  
  enum {
    ID,                 // 00: Unknown string,
                        // 01: e.g) variable... function name...
    EQEQUAL,            // 02: =
    COMMA,              // 03: ,
    STRING,             // 04: In ' ' or " "...
    PLUS,               // 05: +
    MINUS,              // 06: -
    STAR,               // 07: *
    SLASH,              // 08: /
    LPAR,               // 09: (
    RPAR,               // 10: )
    LBRACE,             // 11: {
    RBRACE,             // 12: }
    LSQB,               // 13: [
    RSQB,               // 14: ]
    DOLLAR,             // 15: $
    DOUBLE_BACKSLASH,   // 16: '\\'
    FLOAT,              // 17: e.g. 3.14
    INT,                // 18: e.g. 421
    // EXPONENTIAL      // 19: exponenetial notation number,
                        //      e.g. 3.2E+5
  } type;

  size_t col;
  size_t col_first;
  size_t row;
  size_t row_char;
  size_t row_char_first;
  size_t length;
} Token;

/* In token.c */
Token* init_token\
        (Lexer* lexer, int type, char* value);

/* In lexer.c */
Lexer* update_first_row(Lexer* lexer);

#endif
