#ifndef MAIN_H
#define MAIN_H

#include "Parser/Ast/ast.h"

#define EXTENTION ".orora"

// type tree...
extern orora_value_type* value_type_list;

void error(char* error_message, Parser* parser);
void error_prev_token(char* error_message, Parser* parser);

// For develop...
#ifndef DEVELOP_MODE
#define DEVELOP_MODE
void print_variable(AST_variable* node);
void print_value(AST* ast);
void print_function(AST_function* checked_function);
#endif

#endif

