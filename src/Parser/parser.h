#ifndef PARSER_H
#define PARSER_H

#include "../env.h"
#include "Ast/ast.h"
#include "parser_T.h"
#include "../Lexer/lexer.h"

Parser* init_parser(Lexer* lexer);
Parser* parser_advance(Parser* parser, int type);
AST* parser_parse(Parser* parser);

#endif
