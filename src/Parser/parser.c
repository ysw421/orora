#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"
#include "string.h"

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
//   parser->prev_token = parser->token;
  parser->prev_token = (void*) 0;
  parser->token = lexer_get_token(lexer);
  parser->next_token = lexer_get_token(lexer);

  if (!parser->next_token)
    return parser;

  return after_get_parser(parser);
}

Parser* parser_advance(Parser* parser, int type)
{
  if (parser->token->type != type)
  {
    printf(
        "에러, %s의 타입은 %d가 아닌 %d여야함",
        parser->token->value,
        parser->token->type,
        type
    );
    exit(1);
  }
  parser->prev_token = parser->token;
  parser->token = parser->next_token;
  parser->next_token = lexer_get_token(parser->lexer);

  if (!parser->next_token)
    return parser;

  return after_get_parser(parser);
}

AST* parser_get_compound(Parser* parser)
{
  AST* ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
  ast->compound_v = init_ast_compound();

  Token* token = parser->token;

  while (token != (void*) 0)
  {
    // check value
    bool is_checked_type = false;
    orora_value_type* p = value_type_list;
    do
    {
      if (token->type == p->token_id)
      {
        ast_compound_add(ast->compound_v, p->parser_get_new_ast(ast, token));
        parser = parser_advance(parser, p->token_id);
//         parser = p->parser_get(parser, ast, token);
        token = parser->token;

        is_checked_type = true;
        break;
      }

      p = p->next;
    } while (p);

    if (is_checked_type)
      continue;
    // -----------

    switch (token->type)
    {
      case TOKEN_EQUAL:
      {
        printf("에러, '='의 주어가 존재하지 않음");
        exit(1);
      } break;

      case TOKEN_ID:
      {
        parser_get_id(parser, ast, token);
        token = parser->token;
        continue;
      } break;

      default:
      {
        parser = parser_advance(parser, token->type);
        token = parser->token;
      } break;
    }
  }

  return ast;
}

AST* parser_parse(Parser* parser)
{
  AST* ast = parser_get_compound(parser);
  // Root AST...

  return ast;
}
