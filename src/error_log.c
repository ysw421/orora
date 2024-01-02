#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "Lexer/lexer.h"

void error(char* error_message, Parser* parser, Token* token)
{
  long int nu = token->col_first + 1;
  char* nu_str = malloc(100 * sizeof(char));
  sprintf(nu_str, "%ld", nu);

  char* blank = malloc((nu + 1) * sizeof(char));
  blank[0] = '\0';
  for (int i = 0; i < 6 - strlen(nu_str); i ++)
    strcat(blank, " ");

  int before_error_length = 1;
  char* before_error = malloc(sizeof(char));
  before_error[0] = '\0';
  for (int i = 0; i < parser->row_size; i ++)
  {
    Token* token = parser->row_tokens[i];
    before_error_length += token->length;
    before_error = realloc(before_error, before_error_length * sizeof(char));

    strcat(before_error, token->value);
    if (i == parser->row_size - 1)
      continue;
    for (int j = 0;
        j < parser->row_tokens[i + 1]->row_char_first - token->row_char;
        j ++)
    {
      before_error_length ++;
      before_error = realloc(before_error, before_error_length * sizeof(char));
      strcat(before_error, " ");
    }
  }

  int after_error_length = 1;
  char* after_error = malloc(sizeof(char));
  after_error[0] = '\0';
  
  parser->prev_token = parser->token;
  parser->token = parser->next_token;
  parser->next_token = lexer_get_token(parser->lexer);

  Token* cheked_token = parser->token;
  while (cheked_token && token->col == cheked_token->col_first)
  {
    after_error_length += cheked_token->length;
    after_error = realloc(after_error, after_error_length * sizeof(char));

    strcat(after_error, cheked_token->value);

    if (!parser->next_token)
      break;
    for (int i = 0;
        i < parser->next_token->row_char_first - cheked_token->row_char;
        i ++)
    {
      after_error_length ++;
      after_error = realloc(after_error, after_error_length * sizeof(char));
      strcat(after_error, " ");
    }

    parser->prev_token = parser->token;
    parser->token = parser->next_token;
    parser->next_token = lexer_get_token(parser->lexer);

    cheked_token  = parser->token;
  }

  printf("%s\n", error_message);
  printf("%s%ld | %s\033[0;31m%s\033[0m%s\n       |\n",
      blank, nu, before_error, token->value, after_error);

  free(nu_str);
  exit(1);
}

// e.g.
// int required =
//               snprintf(NULL, 0, "에러, %s가 무엇이죠??::type: %d",
//                   token->value, token->type);
//             char* error_message = malloc((required + 1) * sizeof(char));
//             snprintf(error_message, required + 1,
//                 "에러, %s가 무엇이죠??::type: %d",
//                 token->value, token->type);
//             error(error_message, parser, token);
