#ifndef DEVELOP_MODE
#define DEVELOP_MODE

#include "filesys/file.h"
#include "parser/ast/ast.h"
#include "visitor/visitor.h"
#include "lexer/lexer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void print_function(AST_function* checked_function);
void print_value(AST* ast);
void print_variable(AST_variable* node);
void print_ast_tree(AST* ast_tree);
void print_file(File* file);
void print_tokens(Lexer* root);
// const char* visitor_print_function_value(AST_value_stack* new_value);
// void visitor_print_function_variable(Env_variable* new_value);
// const char* visitor_print_function(Envs* envs, AST* ast);

#endif
