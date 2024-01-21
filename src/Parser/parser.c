#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"
#include "string.h"

AST* parser_get_satisfy(Parser* parser, AST* ast);

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
  parser->tokens = malloc(sizeof(Token*));
  int parser_size = 0;
  while (true)
  {
    Token* new_token = lexer_get_token(lexer);
    if (new_token)
    {
      parser_size ++;
      parser->tokens = realloc(parser->tokens, parser_size * sizeof(Token*));
      parser->tokens[parser_size - 1] = new_token;
      continue;
    }
    break;
  }
  parser->size = parser_size;
  parser->pointer = 0;

  parser->lexer = lexer;
  parser->prev_token = (void*) 0;
//   parser->prev_token = parser->token;
  parser->token = parser->tokens[0];
  parser->next_token = parser->tokens[1];
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
  parser->pointer ++;
  if (parser->pointer + 2 <= parser->size)
    parser->next_token = parser->tokens[parser->pointer + 1];
  else
    parser->next_token = (void*) 0;

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
  ast->value.compound_v = init_ast_compound();

  Token* token = parser->token;

  while (token != (void*) 0)
  {
    if (!compound_env->is_in_parentheses && parser->prev_token &&
        parser->prev_token->col == token->col_first)
    {
      printf("에러, 각 명령어는 줄바꿈으로 구분됨:: %s 전:: 줄: %ld\n",
          token->value, token->col_first + 1);
      exit(1);
    }

    // check value
    Token* stoken = parser->token;
    GET_VALUE_ENV* new_get_value_env = init_get_value_env();
    if (compound_env->is_in_parentheses)
      new_get_value_env->is_in_parentheses = true;
    AST* value_node =
      parser_get_value(&parser, ast, token, new_get_value_env);
    token = parser->token;
    if (value_node)
    {
      if (value_node->value.value_v->size == 1
          && value_node->value.value_v->stack->type == AST_VALUE_VARIABLE)
      {
        ast_compound_add(ast->value.compound_v,
            parser_parse_variable(parser, ast, parser->prev_token));
        token = parser->token;
      }
      else if (value_node->value.value_v->size == 1
          && value_node->value.value_v->stack->type == AST_VALUE_FUNCTION)
      {
        ast_compound_add(ast->value.compound_v,
            parser_parse_function(parser, ast, stoken,
                value_node->value.value_v->stack->value.function_v));
        token = parser->token;
      }
      else
      {
        free(stoken);
        ast_compound_add(ast->value.compound_v, value_node);
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
        break;

      case TOKEN_SATISFY:
        AST* new_satisfy_ast = parser_get_satisfy(parser, ast);
        if (new_satisfy_ast)
        {
          ast_compound_add(ast->value.compound_v, new_satisfy_ast);
          token = parser->token;
          
          continue;
        }
        else
        {
          free(new_satisfy_ast);
          printf("satisfy 사용법: 'satisfy (variable name):(condition1), (condition2)\n");
          exit(1);
        }
        break;

      case TOKEN_DEFINE:
        printf("에러, '%s'의 주어가 존재하지 않음\n", token->value);
        exit(1);
        break;

      default:
        // For develop
        switch (token->type)
        {
          default:
            printf("@@@ %s\n", token->value);
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

AST* parser_get_satisfy(Parser* parser, AST* ast)
{
  Token* token = parser->token;
  parser = parser_advance(parser, TOKEN_SATISFY);
  GET_VALUE_ENV* value_env = init_get_value_env();
  int satisfy_col = parser->prev_token->col;

  AST* new_ast_node =
      init_ast(AST_VARIABLE, ast, parser->token);

  AST_value* main_value =
    parser_get_value(&parser, ast, token, value_env)->value.value_v;
  token = parser->token;
  if (main_value->size == 1
      && main_value->stack->type == AST_VALUE_VARIABLE
      && satisfy_col == parser->prev_token->col_first
      && satisfy_col == token->col_first
      && parser->token && token->type == TOKEN_COLON)
  {
    parser = parser_advance(parser, TOKEN_COLON);
    token = parser->token;

    if (!token || satisfy_col != token->col_first)
    {
      return (void*) 0;
    }

    AST_variable* main_variable = main_value->stack->value.variable_v;
    do
    {
      AST* condition =
        parser_get_value(&parser, ast, token, value_env);
      token = parser->token;
      if (!condition)
        break;
      main_variable->satisfy_size ++;
      main_variable->satisfy =
        realloc(main_variable->satisfy,
            main_variable->satisfy_size * sizeof(struct ast_t*));
      main_variable->satisfy[main_variable->satisfy_size - 1]
        = condition;
      if (!token || satisfy_col != token->col_first
          || token->type != TOKEN_COMMA)
      {
        break;
      }
      else
      {
        parser = parser_advance(parser, TOKEN_COMMA);
        token = parser->token;
      }
    } while (token);
    token = parser->token;

    new_ast_node->value.variable_v = main_variable;
    new_ast_node->value.variable_v->ast_type = AST_VARIABLE_SATISFY;

    return new_ast_node;
  }

  free(new_ast_node);
  return (void*) 0;
}
