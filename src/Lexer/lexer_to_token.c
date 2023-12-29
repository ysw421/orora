#include "lexer.h"
#include <stdlib.h>
#include <string.h>

int get_one_char_type(char c)
{
  switch (c)
  {
    case '=': return EQEQUAL; break;
    case ',': return COMMA; break;
    case '+': return PLUS; break;
    case '-': return MINUS; break;
    case '*': return STAR; break;
    case '/': return SLASH; break;
    case '(': return LPAR; break;
    case ')': return RPAR; break;
    case '{': return LBRACE; break;
    case '}': return RBRACE; break;
    case '[': return LSQB; break;
    case ']': return RSQB; break;
    case '$': return DOLLAR; break;
    default: return -1; break;
  }

  return -1;
}

Token* lexer_to_token_one_char(Lexer* lexer)
{
  char* value = (char*) malloc(2 * sizeof(char));
  value[0] = lexer->c;
  value[1] = '\0';

  int token_type = get_one_char_type(lexer->c);
  if (token_type != -1)
  {
    lexer_advance(lexer);
//     lexer_advance_by_token(lexer, (unsigned int) strlen(value));
    return init_token(lexer, token_type, value);
  }

  free(value);
  return (void*) 0;
}

// Starting with '\' like '\times'
int get_special_string_type(int length, char* string)
{
  switch (length)
  {
    case 7:
      if (!strcmp(string, "satisfy")) return SATISFY; break;
  }

  if (string[0] != '\\')
    return -1;

  switch (length)
  {
    case 6:
      if (!strcmp(string, "\\begin")) return BEGIN;
      if (!strcmp(string, "\\times")) return STAR;
      break;
    case 4:
      if (!strcmp(string, "\\end")) return END;
      if (!strcmp(string, "\\div")) return SLASH;
      break;
    default: return -1; break;
  }

  return -1;
}
