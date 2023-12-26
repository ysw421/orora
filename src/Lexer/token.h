#ifndef TOKEN_H
#define TOKEN_H

#include "lexer_T.h"

typedef struct
{
  char* value;
  
  enum {
    ID,                 // 00: Unknown string,
                        //      e.g) variable... function name...
    EQEQUAL,            // 01: =
    COMMA,              // 02: ,
    STRING,             // 03: In ' ' or " "...
    PLUS,               // 04: +
    MINUS,              // 05: -
    STAR,               // 06: *
    SLASH,              // 07: /
    LPAR,               // 08: (
    RPAR,               // 09: )
    LBRACE,             // 10: {
    RBRACE,             // 11: }
    LSQB,               // 12: [
    RSQB,               // 13: ]
    DOLLAR,             // 14: $
    DOUBLE_BACKSLASH,   // 15: '\\'
    FLOAT,              // 16: e.g. 3.14
    INT,                // 17: e.g. 421
    // EXPONENTIAL      // 17: exponenetial notation number,
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
