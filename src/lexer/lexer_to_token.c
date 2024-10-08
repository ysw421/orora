#include "lexer/lexer.h"
#include <stdlib.h>
#include <string.h>

// Starting with '\' like '\times'
int get_special_string_type(int length, char* string)
{
  switch (length)
  {
    case 9:
      if (!strcmp(string, "otherwise")) return TOKEN_OTHERWISE;
      break;

    case 8:
      if (!strcmp(string, "continue")) return TOKEN_CONTINUE; 
      break;

    case 7:
      if (!strcmp(string, "satisfy")) return TOKEN_SATISFY; 
      break;

    case 6:
      if (!strcmp(string, "return")) return TOKEN_RETURN; 
      break;

    case 5:
      if (!strcmp(string, "break")) return TOKEN_BREAK;
      if (!strcmp(string, "false")) return TOKEN_BOOL;
      break;

    case 4:
      if (!strcmp(string, "NULL")) return TOKEN_NULL; 
      if (!strcmp(string, "true")) return TOKEN_BOOL; 
      break;
  }

  if (string[0] != '\\')
    return -1;

  switch (length)
  {
    case 11:
      if (!strcmp(string, "\\newcommand")) return TOKEN_NEWENVIRONMENT;
      break;

    case 10:
      if (!strcmp(string, "\\otherwise")) return TOKEN_OTHERWISE;
      if (!strcmp(string, "\\leftarrow")) return TOKEN_DEFINE;
      if (!strcmp(string, "\\subjectto")) return TOKEN_SATISFY;
      break;

    case 9:
      if (!strcmp(string, "\\leqslant")) return TOKEN_LESSEQUAL;
      if (!strcmp(string, "\\geqslant")) return TOKEN_GREATEREQUAL;
      break;

    case 8:
      if (!strcmp(string, "\\satisty")) return TOKEN_SATISFY;
      break;

    case 6:
      if (!strcmp(string, "\\begin")) return TOKEN_BEGIN;
      if (!strcmp(string, "\\times")) return TOKEN_STAR;
      if (!strcmp(string, "\\right")) return TOKEN_RIGHT;
      break;

    case 5:
      if (!strcmp(string, "\\cdot")) return TOKEN_CDOT;
      if (!strcmp(string, "\\left")) return TOKEN_LEFT;
      if (!strcmp(string, "\\land")) return TOKEN_AND;
      if (!strcmp(string, "\\over")) return TOKEN_SLASH;
      break;

    case 4:
      if (!strcmp(string, "\\end")) return TOKEN_END;
      if (!strcmp(string, "\\div")) return TOKEN_SLASH;
      if (!strcmp(string, "\\neq")) return TOKEN_NOTEQUAL;
      if (!strcmp(string, "\\lor")) return TOKEN_OR;
      if (!strcmp(string, "\\neg")) return TOKEN_NEG;
      if (!strcmp(string, "\\mod")) return TOKEN_MOD;
      if (!strcmp(string, "\\leq")) return TOKEN_LESSEQUAL;
      if (!strcmp(string, "\\geq")) return TOKEN_GREATEREQUAL;
      break;

    case 3:
      if (!strcmp(string, "\\in")) return TOKEN_IN;
      break;

    default: 
      return -1; 
      break;
  }

  return -1;
}

int get_one_char_type(char c)
{
  switch (c)
  {
    case '=': return TOKEN_EQUAL;       break;
    case ',': return TOKEN_COMMA;       break;
    case '+': return TOKEN_PLUS;        break;
    case '-': return TOKEN_MINUS;       break;
    case '*': return TOKEN_STAR;        break;
    case '/': return TOKEN_SLASH;       break;
    case '(': return TOKEN_LPAR;        break;
    case ')': return TOKEN_RPAR;        break;
    case '{': return TOKEN_LBRACE;      break;
    case '}': return TOKEN_RBRACE;      break;
    case '[': return TOKEN_LSQB;        break;
    case ']': return TOKEN_RSQB;        break;
    case '$': return TOKEN_DOLLAR;      break;
    case ':': return TOKEN_COLON;       break;
    case '&': return TOKEN_AMPER;       break;
    case '<': return TOKEN_LESS;        break;
    case '>': return TOKEN_GREATER;     break;
    case '!': return TOKEN_NEG;         break;
    case '^': return TOKEN_CIRCUMFLEX;  break;
    case '_': return TOKEN_UNDER;       break;
    default: return -1; break;
  }

  return -1;
}

INT_STRING_T* get_two_char_type(Lexer* lexer)
{
  if (lexer->length - 2 < lexer->pointer)
  {
    INT_STRING_T* result = malloc(sizeof(INT_STRING_T));
    result->int_v = -1;
    return result;
  }

  INT_STRING_T* result = malloc(sizeof(INT_STRING_T));

  char* value = (char*) malloc(3 * sizeof(char));
  value[0] = lexer->c;
  value[1] = lexer->contents[lexer->pointer + 1];
  value[2] = '\0';
  result->string_v = value;

  if (!strcmp(value, ":="))
    result->int_v = TOKEN_DEFINE;
  else if (!strcmp(value, "<="))
    result->int_v = TOKEN_LESSEQUAL;
  else if (!strcmp(value, ">="))
    result->int_v = TOKEN_GREATEREQUAL;
  else if (!strcmp(value, "!="))
    result->int_v = TOKEN_NOTEQUAL;
  else if (!strcmp(value, "||"))
    result->int_v = TOKEN_OR;
  else if (!strcmp(value, "&&"))
    result->int_v = TOKEN_AND;
  else if (!strcmp(value, "::"))
    result->int_v = TOKEN_DOUBLECOLON;
  else
  {
    result->string_v = (void*) 0;
    result->int_v = -1;
    free(value);
  }
  return result;
}

Token* lexer_to_token_two_char(Lexer* lexer)
{
  INT_STRING_T* cheker = get_two_char_type(lexer);

  int token_type = cheker->int_v;
  char* value = cheker->string_v;
  free(cheker);

  if (token_type == -1)
    return (void*) 0;

  lexer_advance(lexer);
  lexer_advance(lexer);

  return init_token(lexer, token_type, value);
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

