#ifndef TOKEN_H
#define TOKEN_H

#include "lexer_T.h"

typedef struct
{
  char* value;
  
  enum {
    ID,                 // Unknown string,
                        // e.g) variable... function name...
    EQEQUAL,            // =
    COMMA,              // ,
    STRING,             // In ' ' or " "...
    PLUS,               // +
    MINUS,              // -
    STAR,               // *
    SLASH,              // /
    LPAR,               // (
    RPAR,               // )
    LBRACE,             // {
    RBRACE,             // }
    LSQB,               // [
    RSQB,               // ]
    DOLLAR,             // $
    DOUBLE_BACKSLASH,   // '\\'
    FLOAT,              // e.g. 3.14
    INT,                // e.g. 421
    // EXPONENTIAL      // exponenetial notation number, e.g. 3.2E+5
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
