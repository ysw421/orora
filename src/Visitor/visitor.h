#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/Ast/ast.h"
#include "../env.h"

void visitor_visit(Envs* envs, AST* ast);
Env_variable* visitor_set_env_variable_int(Env_variable* env_variable,
    AST_value_stack* ast);
Env_variable* visitor_set_env_variable_float(Env_variable* env_variable,
    AST_value_stack* ast);
Env_variable* visitor_set_env_variable_string(Env_variable* env_variable,
    AST_value_stack* ast);

#endif
