#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*) malloc(sizeof(struct parser_t));
  parser->lexer = lexer;
//   parser->prev_token = parser->token;
  parser->prev_token = (void*) 0;
  parser->token = lexer_get_token(lexer);

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
  parser->token = lexer_get_token(parser->lexer);

  return parser;
}

AST* parser_parse(Parser* parser, Envs* envs)
{
  AST* ast = init_ast(AST_COMPOUND, (void*) 0, 0, 0, 0, 0, 0);
  ast->compound_v = init_ast_compound();

  Token* token = parser->token;
  while (token != (void*) 0)
  {
    if (token->type == INT)
    {
      AST* new_ast_node =
        init_ast(AST_INT, ast, token->col, token->col_first,
                  token->row, token->row_char, token->row_char_first);
      new_ast_node->int_v = init_ast_int(atoi(token->value));
      ast_compound_add(ast->compound_v, new_ast_node);

      parser = parser_advance(parser, INT);
      token = parser->token;
      continue;
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
    parser = parser_advance(parser, token->type);
    token = parser->token;
  }

  return ast;
}
