#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"
#include "string.h"

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*) malloc(sizeof(struct parser_t));
  parser->lexer = lexer;
//   parser->prev_token = parser->token;
  parser->prev_token = (void*) 0;
  parser->token = lexer_get_token(lexer);
  parser->next_token = lexer_get_token(lexer);

  return parser;
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

  return parser;
}

AST* parser_get_compound(Parser* parser)
{
  AST* ast = init_ast(AST_COMPOUND, (void*) 0, 0, 0, 0, 0, 0);
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
        parser = p->parser_get(parser, ast, token);
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
        parser = parser_get_id(parser, ast, token);
        token = parser->token;
        continue;
      } break;

      default:
      {
        parser = parser_advance(parser, token->type);
        token = parser->token;
      } break;
    }
//     if (parser->prev_token != (void*) 0)
//     {
//       if (parser->prev_token->type == ID && parser->token->type == LPAR)
//       {
//         parser = parser_advance(parser, LPAR);
//         char* printed_string = "";
//         if (parser->token->type == STRING)
//         {
//           printed_string = parser->token->value;
//         }
//         parser = parser_advance(parser, STRING);
//         parser = parser_advance(parser, RPAR);
//         printf("%s\n", printed_string);
//       }
//     }
  }

  return ast;
}

AST* parser_parse(Parser* parser)
{
  AST* ast = parser_get_compound(parser);
  // Root AST...

  return ast;
}
