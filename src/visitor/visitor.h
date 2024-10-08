#ifndef VISITOR_H
#define VISITOR_H

#include "parser/ast/ast.h"
#include "loader/env.h"
#include "loader/config.h"

typedef struct get_visitor_env_t
{
  bool is_break;
  bool is_continue;
  AST_value_stack* is_return;
  AST_value_stack* output;
} GET_VISITOR_ENV;

GET_VISITOR_ENV* visitor_visit(Envs* envs, AST* ast);

// ToDo: merge...
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_int
(Env_variable* env_variable, AST_value_stack* new_value);
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_float
(Env_variable* env_variable, AST_value_stack* new_value);
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_string
(Env_variable* env_variable, AST_value_stack* new_value);
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_null
(Env_variable* env_variable, AST_value_stack* new_value);
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_bool
(Env_variable* env_variable, AST_value_stack* new_value);
Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_matrix
(Env_variable* env_variable, AST_value_stack* new_value);

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_int
(AST_value_stack* new_value_stack, Env_variable* env_variable);
AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_float
(AST_value_stack* new_value_stack, Env_variable* env_variable);
AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_string
(AST_value_stack* new_value_stack, Env_variable* env_variable);
AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_null
(AST_value_stack* new_value_stack, Env_variable* env_variable);
AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_bool
(AST_value_stack* new_value_stack, Env_variable* env_variable);
AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_matrix
(AST_value_stack* new_value_stack, Env_variable* env_variable);

#ifdef DEVELOP_MODE
Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_get_function(Envs* envs, AST_function* ast_function);
Env_macro* visitor_get_macro(Envs* envs, AST_macro* ast_macro);

AST_value_stack* visitor_get_value(Envs* envs, AST_value* ast_value);

void visitor_nondefine_variable_error(AST_variable* ast_variable);
void visitor_nondefine_function_error(AST_function* ast_function);

AST_value_stack* visitor_get_value_from_function
(Envs* envs, AST_function* ast_function);
AST_value_stack* visitor_get_value_from_macro
(Envs* envs, AST_macro* ast_macro);
AST_value_stack* visitor_get_value_from_ast
(Envs* envs, AST* condition);
#endif

#endif
