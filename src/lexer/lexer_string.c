#include "lexer/lexer.h"
#include "loader/error_log.h"
#include <string.h>
#include <stdlib.h>
/* For error */
#include <stdio.h>
/* ========= */

void lexer_get_string_error(Lexer* lexer)
{
  orora_error("에러, string이 끝나지 않습니다.", (void*) 0);
//   printf("에러, string이 끝나지 않습니다.\n");
//   exit(1);
}

char* lexer_get_string_add_char(char* string, char* s)
{
  string = realloc(
             string,
             (strlen(string) + strlen(s) + 1) * sizeof(char)
           );
  strcat(string, s);

  return string;
}

Token* lexer_get_string(Lexer* lexer, char c)
{
//   char* string = (char*) malloc(sizeof(char));
//   string[0] = '\0';
  char* string = (char*) malloc(2 * sizeof(char));
  string[0] = c;
  string[1] = '\0';

  lexer_advance(lexer);   // First char, " or '
 
  bool is_next_backslash = false;

  while (!(lexer->c == c && is_next_backslash == false))
  {
    if (IS_LEXER_CONTENTS_END(lexer))   // ERROR
      lexer_get_string_error(lexer);    // |->
    
    if (is_next_backslash) // Special literal, e.g.) \n, \t, \r
    {
      is_next_backslash = false;
      char* s = (char*) malloc(2 * sizeof(char));
      s[1] = '\0';
      bool is_special_literal = true;
      switch (lexer->c)
      {
        case 'n': s[0] = '\n'; break;
        case 't': s[0] = '\t'; break;
        case 'r': s[0] = '\r'; break;
        case '\\': s[0] = '\\'; break;
        case '\'': s[0] = '\''; break;
        case '"': s[0] = '"'; break;
        default: is_special_literal = false; break;
      }
      if (is_special_literal)
      {
        string = lexer_get_string_add_char(string, s);
        lexer_advance(lexer);
        free(s);
        continue;
      }
      free(s);
    }

    switch (lexer->c)
    {
      case '\n': lexer_get_string_error(lexer); break;  // ERROR
      case '\\': is_next_backslash = true; break;
                 // Strating special literal
      default:
        char* s = (char*) malloc(2 * sizeof(char));
        s[0] = lexer->c;
        s[1] = '\0';

        string = lexer_get_string_add_char(string, s);
        free(s);
        break;
    }

    lexer_advance(lexer);
  }

  lexer_advance(lexer);   // Last char
  string = lexer_get_string_add_char(string, c == '\''? "'" : "\"");

  Token* token = init_token(lexer, TOKEN_STRING, string);

  return token;
}

