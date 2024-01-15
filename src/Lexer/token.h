#ifndef TOKEN_H
#define TOKEN_H

#include "lexer_T.h"

typedef struct
{
  char* value;
  
  enum {
    TOKEN_ID,                 // 00: Unknown string,
                              //      e.g) variable... function name...
    TOKEN_EQUAL,              // 01: =
    TOKEN_COMMA,              // 02: ,
    TOKEN_STRING,             // 03: In ' ' or " "...
    TOKEN_PLUS,               // 04: +
    TOKEN_MINUS,              // 05: -
    TOKEN_STAR,               // 06: *
    TOKEN_SLASH,              // 07: /
    TOKEN_LPAR,               // 08: (
    TOKEN_RPAR,               // 09: )
    TOKEN_LBRACE,             // 10: {
    TOKEN_RBRACE,             // 11: }
    TOKEN_LSQB,               // 12: [
    TOKEN_RSQB,               // 13: ]
    TOKEN_DOLLAR,             // 14: $
    TOKEN_DOUBLE_BACKSLASH,   // 15: '\\'
    TOKEN_FLOAT,              // 16: e.g. 3.14
    TOKEN_INT,                // 17: e.g. 421
    TOKEN_SATISFY,            // 18: satisfy
    TOKEN_BEGIN,              // 19: \begin
    TOKEN_END,                // 20: \end
    TOKEN_LEFT,               // 21: \left
    TOKEN_RIGHT,              // 22: \right
    TOKEN_DEFINE,             // 23: :=, \leftarrow
    TOKEN_COLON               // 24: :
    // TOKEN_EXPONENTIAL      // 17: exponenetial notation number,
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
