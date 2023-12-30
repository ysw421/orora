#include "parser.h"
#include <stdio.h>
#include "../Lexer/token.h"

AST* parser_get_new_int_ast(AST* ast, Token* token);
AST* parser_get_new_float_ast(AST* ast, Token* token);
AST* parser_get_new_string_ast(AST* ast, Token* token);

Parser* parser_get_int(Parser* parser, AST* ast, Token* token);
Parser* parser_get_float(Parser* parser, AST* ast, Token* token);
Parser* parser_get_string(Parser* parser, AST* ast, Token* token);
