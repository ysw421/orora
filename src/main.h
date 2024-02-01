#ifndef MAIN_H
#define MAIN_H

#include "Parser/Ast/ast.h"

#define EXTENTION ".or"

// type tree...
extern orora_value_type* value_type_list;

void error(char* error_message, Parser* parser);
void error_prev_token(char* error_message, Parser* parser);

// For develop...
#ifndef DEVELOP_MODE
#define DEVELOP_MODE
#include "file.h"
void print_variable(AST_variable* node);
void print_value(AST* ast);
void print_function(AST_function* checked_function);
void print_ast_tree(AST* ast_tree);
void print_file(File* file);
void print_tokens(Lexer* root);
#endif

#endif

