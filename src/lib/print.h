#ifndef PRINT_H
#define PRINT_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include "visitor/visitor.h"
// #include "parser/ast/ast.h"
// #include "lexer/lexer.h"

const char* visitor_print_function_value(AST_value_stack* new_value);
void visitor_print_function_variable(Env_variable* new_value);
const char* visitor_print_function(Envs* envs, AST* ast);

#endif
