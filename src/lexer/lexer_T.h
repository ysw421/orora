#ifndef LEXER_T_H
#define LEXER_T_H

#include <stddef.h>

typedef struct
{
// 	unsigned long int pointer;
	size_t pointer;
	char* contents;
// 	unsigned long int length;
	size_t length;
	char c;
	size_t col;
  size_t col_first;
  size_t col_first_pointer;
	size_t row;	// Split by token
	size_t row_char;	// Split by char
  size_t row_char_first;
} Lexer;

#endif
