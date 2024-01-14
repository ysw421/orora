#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"
#include "string.h"

GET_COMPOUND_ENV* init_get_compound_env()
{
  GET_COMPOUND_ENV* new_env = malloc(sizeof(GET_COMPOUND_ENV));
  new_env->is_in_parentheses = false;
  new_env->is_usefull_comma = false;

  return new_env;
}

Parser* after_get_parser(Parser* parser)
{
  Token* token = parser->token;
  Token* next_token = parser->next_token;

  if (token->type == TOKEN_LEFT && next_token->type == TOKEN_LPAR)
  {
    if (token->col_first != next_token->col_first)
    {
      printf("에러, '\\left'와 '('가 다른 줄에 존재함.");
      exit(1);
    }
    int new_value_length = 6 + next_token->row_char_first - token->row_char;
    char* new_value = malloc((new_value_length + 1) * sizeof(char));
    new_value[0] = '\0';
    strcat(new_value, "\\left");
    for (int i = 0; i < new_value_length - 6; i ++)
    {
      strcat(new_value, " ");
    }
    strcat(new_value, "(");
    parser->token->value = new_value;
    parser->token->length = new_value_length;
    parser->token->type = TOKEN_LPAR;
    parser->token->col = next_token->col;
    parser->token->row_char = next_token->row_char;

    parser->next_token = lexer_get_token(parser->lexer);
  }

  if (token->type == TOKEN_RIGHT && next_token->type == TOKEN_RPAR)
  {
    if (token->col_first != next_token->col_first)
    {
      printf("에러, '\\right'와 ')'가 다른 줄에 존재함.");
      exit(1);
    }
    int new_value_length = 7 + next_token->row_char_first - token->row_char;
    char* new_value = malloc((new_value_length + 1) * sizeof(char));
    new_value[0] = '\0';
    strcat(new_value, "\\right");
    for (int i = 0; i < new_value_length - 7; i ++)
    {
      strcat(new_value, " ");
    }
    strcat(new_value, ")");
    parser->token->value = new_value;
    parser->token->length = new_value_length;
    parser->token->type = TOKEN_RPAR;
    parser->token->col = next_token->col;
    parser->token->row_char = next_token->row_char;

    parser->next_token = lexer_get_token(parser->lexer);
  }

  return parser;
}

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*) malloc(sizeof(struct parser_t));
  parser->lexer = lexer;
  parser->prev_token = (void*) 0;
//   parser->prev_token = parser->token;
  parser->token = lexer_get_token(lexer);
  parser->next_token = lexer_get_token(lexer);
  parser->row_size = 0;
  parser->row_tokens = malloc(sizeof(Token*));
  parser->row_tokens[0] = malloc(sizeof(Token));

  if (!parser->next_token)
    return parser;

  return after_get_parser(parser);
}

Parser* parser_advance(Parser* parser, int type)
{
  if (parser->token->type != type)
  {
    printf(
        "에러, %s의 타입은 %d가 아닌 %d여야함\nprev: %s, next: %s\n",
        parser->token->value,
        parser->token->type,
        type,
        parser->prev_token ? parser->prev_token->value : "",
        parser->next_token ? parser->next_token->value : ""
    );
    exit(1);
  }
//   free(parser->prev_token);
  parser->prev_token = parser->token;
  parser->token = parser->next_token;
  parser->next_token = lexer_get_token(parser->lexer);

  if (!parser->token)
    return parser;

  if (parser->prev_token->col == parser->token->col_first)
  {
    parser->row_size ++;
    parser->row_tokens = realloc(parser->row_tokens,
        parser->row_size * sizeof(Token*));
    parser->row_tokens[parser->row_size - 1] = parser->prev_token;
  }
  else
  {
    free(parser->row_tokens);
    parser->row_size = 0;
    parser->row_tokens = malloc(sizeof(Token*));
  }

  if (!parser->next_token)
    return parser;

  return after_get_parser(parser);
}

AST* parser_get_compound_end(AST* ast, GET_COMPOUND_ENV* compound_env)
{
  free(compound_env);
  return ast;
}

AST* parser_get_compound(Parser* parser, GET_COMPOUND_ENV* compound_env)
{
  AST* ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
  ast->compound_v = init_ast_compound();

  Token* token = parser->token;

  while (token != (void*) 0)
  {
    if (!compound_env->is_in_parentheses && parser->prev_token &&
        parser->prev_token->col == token->col_first)
    {
      printf("에러, 각 명령어는 줄바꿈으로 구분됨:: %s와 %s 사이:: 줄: %ld\n",
          parser->prev_token->value, token->value, token->col_first + 1);
      exit(1);
    }

    // check value
    AST* value_node =
      parser_get_value(parser, ast, token, init_get_value_env());
    token = parser->token;
    if (value_node && !(value_node->value_v->size == 1
        && (value_node->value_v->stack->type == AST_VALUE_FUNCTION)))
    {
      if (value_node->value_v->size == 1
          && value_node->value_v->stack->type == AST_VALUE_VARIABLE)
      {
        ast_compound_add(ast->compound_v,
            parser_get_id(parser, ast, parser->prev_token));
        token = parser->token;
      }
      else
      {
        ast_compound_add(ast->compound_v, value_node);
        token = parser->token;
      }
      continue;
    }
    free(value_node);
    // -----------

    switch (token->type)
    {
      case TOKEN_COMMA:
        if (compound_env->is_usefull_comma)
          return parser_get_compound_end(ast, compound_env);
        break;

      case TOKEN_RPAR:
        if (compound_env->is_in_parentheses)
          return parser_get_compound_end(ast, compound_env);
        // !!!!!! FOR TEST !!!!!!!
        else
        {
          parser = parser_advance(parser, token->type);
          token = parser->token;
        }
        // !!!!!!!!!!!!!!!!!!!!!!!
        break;

      case TOKEN_DEFINE:
        printf("에러, '%s'의 주어가 존재하지 않음\n", token->value);
        exit(1);
        break;

      case TOKEN_ID:
        ast_compound_add(ast->compound_v, parser_get_id(parser, ast, token));
        token = parser->token;
        continue;
        break;

      default:
        // For develop
        switch (token->type)
        {
          case TOKEN_LPAR:
          case TOKEN_RPAR:
          case TOKEN_LEFT:
          case TOKEN_RIGHT:
          case TOKEN_MINUS:
            parser = parser_advance(parser, token->type);
            token = parser->token;
            break;
          default:
            int required =
              snprintf(NULL, 0, "에러, %s가 무엇이죠??",
                  token->value);
            char* error_message = malloc((required + 1) * sizeof(char));
            snprintf(error_message, required + 1,
                "에러, %s가 무엇이죠??",
                token->value);
            printf("@@@%s\n", parser->token->value);
            error_prev_token(error_message, parser);
            break;
        }
        // End for develop

        // Free Condition
//         parser = parser_advance(parser, token->type);
//         token = parser->token;
        // End free condition
        break;
    }
  }

  return parser_get_compound_end(ast, compound_env);
}

AST* parser_parse(Parser* parser)
{
  AST* ast = parser_get_compound(parser, init_get_compound_env());
  // Root AST...

  return ast;
}
