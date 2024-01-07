#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "Lexer/lexer.h"

void error(char* error_message, Parser* parser)
{
  Token* saved_token = parser->token;
  int root_num;

  long int nu = saved_token->col_first + 1;
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

    if (!parser->row_tokens[i + 1])
    {
      root_num = saved_token->row_char_first
                  - parser->row_tokens[parser->row_size - 1]->row_char;
      before_error_length += root_num;
      before_error =
        realloc(before_error, before_error_length * sizeof(char));
      for (int i = 0; i < root_num; i ++)
        strcat(before_error, " ");

      continue;
    }

    root_num = parser->row_tokens[i + 1]->row_char_first - token->row_char;
    before_error_length += root_num;
    before_error = realloc(before_error, before_error_length * sizeof(char));
    for (int j = 0; j < root_num; j ++)
      strcat(before_error, " ");
  }

  char* blank2 = malloc((nu + 1) * sizeof(char));
  blank2[0] = '\0';
  for (int i = 0; i < before_error_length; i ++)
    strcat(blank2, " ");

  char* underbars = malloc(saved_token->length * sizeof(char));
  underbars[0] = '\0';
  for (int i = 0; i < saved_token->length - 1; i ++)
    strcat(underbars, "-");

  int after_error_length = 1;
  char* after_error = malloc(sizeof(char));
  after_error[0] = '\0';
  
  parser->prev_token = parser->token;
  parser->token = parser->next_token;
  parser->next_token = lexer_get_token(parser->lexer);

  Token* cheked_token = parser->token;
  while (cheked_token && saved_token->col == cheked_token->col_first)
  {
    root_num = cheked_token->row_char_first - parser->prev_token->row_char;
    after_error_length += root_num;
    after_error = realloc(after_error, after_error_length * sizeof(char));
    for (int i = 0; i < root_num; i ++)
      strcat(after_error, " ");

    after_error_length += cheked_token->length;
    after_error = realloc(after_error, after_error_length * sizeof(char));

    strcat(after_error, cheked_token->value);

    parser->prev_token = parser->token;
    parser->token = parser->next_token;
    parser->next_token = lexer_get_token(parser->lexer);

    cheked_token  = parser->token;
  }

  printf("%s%ld | %s\033[0;31m%s\033[0m%s\n       |%s\033[0;31m^%s\033[0m\n",
      blank, nu, before_error, saved_token->value,
      after_error, blank2, underbars);
  printf("%s\n", error_message);

  free(nu_str);
  exit(1);
}

void error_prev_token(char* error_message, Parser* parser)
{
  Token* saved_token = parser->prev_token;
  int root_num;

  long int nu = saved_token->col_first + 1;
  char* nu_str = malloc(100 * sizeof(char));
  sprintf(nu_str, "%ld", nu);

  char* blank = malloc((nu + 1) * sizeof(char));
  blank[0] = '\0';
  for (int i = 0; i < 6 - strlen(nu_str); i ++)
    strcat(blank, " ");

  int before_error_length = 1;
  char* before_error = malloc(sizeof(char));
  before_error[0] = '\0';
  for (int i = 0; i < parser->row_size - 1; i ++)
  {
    Token* token = parser->row_tokens[i];
    before_error_length += token->length;
    before_error = realloc(before_error, before_error_length * sizeof(char));

    strcat(before_error, token->value);

    if (!parser->row_tokens[i + 1])
    {
      root_num = saved_token->row_char_first
                  - parser->row_tokens[parser->row_size - 1]->row_char;
      before_error_length += root_num;
      before_error =
        realloc(before_error, before_error_length * sizeof(char));
      for (int i = 0; i < root_num; i ++)
        strcat(before_error, " ");

      continue;
    }

    root_num = parser->row_tokens[i + 1]->row_char_first - token->row_char;
    before_error_length += root_num;
    before_error = realloc(before_error, before_error_length * sizeof(char));
    for (int j = 0; j < root_num; j ++)
      strcat(before_error, " ");
  }

  char* blank2 = malloc((nu + 1) * sizeof(char));
  blank2[0] = '\0';
  for (int i = 0; i < before_error_length; i ++)
    strcat(blank2, " ");

  char* underbars = malloc(saved_token->length * sizeof(char));
  underbars[0] = '\0';
  for (int i = 0; i < saved_token->length - 1; i ++)
    strcat(underbars, "-");

  int after_error_length = 1;
  char* after_error = malloc(sizeof(char));
  after_error[0] = '\0';

  root_num = parser->token->row_char_first - parser->prev_token->row_char;
  after_error_length += root_num;
  after_error = realloc(after_error, after_error_length * sizeof(char));
  for (int i = 0; i < root_num; i ++)
    strcat(after_error, " ");

  after_error_length += parser->token->length;
  after_error = realloc(after_error, after_error_length * sizeof(char));
  strcat(after_error, parser->token->value);

  parser->prev_token = parser->token;
  parser->token = parser->next_token;
  parser->next_token = lexer_get_token(parser->lexer);

  Token* cheked_token = parser->token;
  while (cheked_token && saved_token->col == cheked_token->col_first)
  {
    for (int i = 0;
        i < cheked_token->row_char_first - parser->prev_token->row_char;
        i ++)
    {
      after_error_length ++;
      after_error = realloc(after_error, after_error_length * sizeof(char));
      strcat(after_error, " ");
    }

    after_error_length += cheked_token->length;
    after_error = realloc(after_error, after_error_length * sizeof(char));

    strcat(after_error, cheked_token->value);

    parser->prev_token = parser->token;
    parser->token = parser->next_token;
    parser->next_token = lexer_get_token(parser->lexer);

    cheked_token  = parser->token;
  }

  printf("%s%ld | %s\033[0;31m%s\033[0m%s\n       |%s\033[0;31m^%s\033[0m\n",
      blank, nu, before_error, saved_token->value,
      after_error, blank2, underbars);
  printf("%s\n", error_message);

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
