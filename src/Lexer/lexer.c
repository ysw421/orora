#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Lexer* update_first_col_row_when_changed_line(Lexer* lexer)
{
  lexer->col_first = lexer->col + 1;
  lexer->row_char_first = 0;

  return lexer;
}

Lexer* update_first_row(Lexer* lexer)
{
  lexer->row_char_first = lexer->pointer - lexer->col_first_pointer - 1;

  return lexer;
}

Lexer* init_lexer(char* contents, unsigned long int* length)
{
  Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
  lexer->pointer = 0;
  lexer->contents = contents;
  lexer->length = length == (void*) 0 ? strlen(contents) : *length;
  lexer->c = contents[lexer->pointer];
  lexer->col = 0;
  lexer->col_first = 0;
  lexer->col_first_pointer = 0;
  lexer->row = 0;
  lexer->row_char = 0;
  lexer->row_char_first = 0;
  
  return lexer;
}

Lexer* lexer_skip_whitespaces(Lexer* lexer)
{
  while (!IS_LEXER_CONTENTS_END(lexer) && IS_WHITESPACE(lexer->c))
    lexer_advance(lexer);
  update_first_row(lexer);
  return lexer;
}

Lexer* lexer_skip_comment(Lexer* lexer)
{
  while (!IS_LEXER_CONTENTS_END(lexer) && lexer->c != '\n')
    lexer_advance(lexer);
  return lexer;
}

void lexer_advance_by_token(Lexer* lexer, unsigned int length)
{
  for (unsigned int i = 0; i < length; i ++)
    lexer_advance(lexer);
}

// Basic Code: change lexer pointer
bool lexer_advance(Lexer* lexer)
{
  if (IS_LEXER_CONTENTS_END(lexer))
  {
    printf("에러, lexer.c=>lexer_advance\n");
    exit(1);
//     return false;
  }

  if (lexer->c == '\n')
  {
    lexer->row_char = 0;
    lexer->row = 0;
    lexer->col ++;
    lexer->col_first_pointer = lexer->pointer;
  }
  else lexer->row_char ++;

  lexer->pointer ++;
  lexer->c = lexer->contents[lexer->pointer];
  return true;
}

Token* lexer_get_token(Lexer* lexer)
{
  while (!IS_LEXER_CONTENTS_END(lexer))
  {
    if (IS_WHITESPACE(lexer->c))    // Without '\n'
    {
      lexer_skip_whitespaces(lexer);
      continue;
    } else if (lexer->c == '%')     // Comment
    {
      lexer_skip_comment(lexer);
      continue;
    }

    /* Get token... Like '=', ',' ... */
    Token* one_char = lexer_to_token_one_char(lexer);
    if (one_char)
      return one_char;
    else
      free(one_char);
    /* ============================= */
    /* Get String */
    if (lexer->c == '"' || lexer->c == '\'')
      return lexer_get_string(lexer, lexer->c);
    /* ========== */
    /* Get number without sign */
    if (IS_NUMBER_CHAR(lexer->c))
      return lexer_get_number(lexer);
    /* ======================= */

    if (lexer->c == '\n')
    {
      update_first_col_row_when_changed_line(lexer);
      lexer_advance(lexer);
      continue;
    }

    /* For test */
    /* ToDo: I will make method for get symbols inclouding 2 char.
     * Maybe, It similar with the function (get_one_char_type)
     *        in lexer_to_token.c
     * And this new function including other symbols...
     *      e.g) :=
     * But, This symbols will be have differences with other
     *      programming languges symbols like +=...
     * Becuse It is a ORORA 😁
     *                that similar with mathmatic's expression. */
    if (lexer->c == '\\'
        && lexer->pointer + 1 <= lexer->length
        && lexer->contents[lexer->pointer + 1] == '\\')
    {
      char* symbol = (char*) malloc(3 * sizeof(char));
      symbol[0] = '\\';
      symbol[1] = '\\';
      symbol[2] = '\0';
      lexer_advance(lexer);
      lexer_advance(lexer);
      
      return init_token(lexer, TOKEN_DOUBLE_BACKSLASH, symbol);
    }
    /* ======== */

    return lexer_get_id(lexer);
  }
  return (void*) 0;
}
