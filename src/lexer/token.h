#ifndef TOKEN_H
#define TOKEN_H

#include "lexer/lexer_T.h"

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
    TOKEN_COLON,              // 24: :
    TOKEN_NULL,               // 25: NULL
    TOKEN_AMPER,              // 26: &
    TOKEN_OTHERWISE,          // 27: otherwise
    TOKEN_BREAK,              // 28: break
    TOKEN_CONTINUE,           // 29: continue
    TOKEN_RETURN,             // 30: return
    TOKEN_LESS,               // 31: <
    TOKEN_GREATER,            // 32: >
    TOKEN_BOOL,               // 33: e.g. true, false
    TOKEN_LESSEQUAL,          // 34: <=, \leqq, \leqslant
    TOKEN_GREATEREQUAL,       // 35: >=, \geqq, \geqslant
    TOKEN_NOTEQUAL,           // 36: !=, \neq
    TOKEN_OR,                 // 37: ||, \lor
    TOKEN_AND,                // 38: &&, \land
    TOKEN_NEG,                // 39: !, \neg
    TOKEN_TEXT,               // 40: \text
    TOKEN_CIRCUMFLEX,         // 41: ^
    TOKEN_NOOP                // 99:
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
