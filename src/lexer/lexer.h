#ifndef LEXER_H
#define LEXER_H

#include "lexer_T.h"
#include "stdbool.h"
#include "token.h"

#define IS_LEXER_CONTENTS_END(lexer)\
        (lexer->c == '\0'\
         || lexer->pointer > lexer->length)
#define IS_WHITESPACE(c)\
        (c == ' '\
         || c == '\t'\
         || c == '\r')
//          || c == 10)  // = '\n'
#define IS_NUMBER_CHAR(c) (c >= '0' && c <= '9')
 
/* In lexer.c */
Lexer* init_lexer(char* contents, unsigned long int* length);
Token* lexer_get_token(Lexer* lexer);
void lexer_advance_by_token(Lexer* lexer, unsigned int length);
bool lexer_advance(Lexer* lexer);

/* In lexer_to_token.c */
typedef struct
{
  int int_v;
  char* string_v;
} INT_STRING_T;

Token* lexer_to_token_one_char(Lexer* lexer);
INT_STRING_T* get_two_char_type(Lexer* lexer);
Token* lexer_to_token_two_char(Lexer* lexer);
int get_one_char_type(char c);
int get_special_string_type(int length, char* string);

/* In lexer_string.c */
Token* lexer_get_string(Lexer* lexer, char c);

/* In lexer_number.c */
Token* lexer_get_number(Lexer* lexer);

/* In lexer_id.c */
Token* lexer_get_id(Lexer* lexer);
#endif
