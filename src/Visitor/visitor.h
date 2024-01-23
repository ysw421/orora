#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/Ast/ast.h"
#include "../env.h"

void visitor_visit(Envs* envs, AST* ast);

// ToDo: merge...
Env_variable* visitor_set_env_variable_int(Env_variable* env_variable,
    AST_value_stack* ast);
Env_variable* visitor_set_env_variable_float(Env_variable* env_variable,
    AST_value_stack* ast);
Env_variable* visitor_set_env_variable_string(Env_variable* env_variable,
    AST_value_stack* ast);
Env_variable* visitor_variable_define_value_int(Env_variable* env_variable,
    AST_value_stack* new_value);
Env_variable* visitor_variable_define_value_float(Env_variable* env_variable,
    AST_value_stack* new_value);
Env_variable* visitor_variable_define_value_string(Env_variable* env_variable,
    AST_value_stack* new_value);


#endif
