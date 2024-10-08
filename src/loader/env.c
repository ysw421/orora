#include "loader/env.h"
#include <stdio.h>
#include <stdlib.h>

Env_function* get_env_function_from_ast_function
(Env_function* env_function, AST_function* ast_function)
{
//   env_function->name = ast_function->name;
//   env_function->length = ast_function->name_length;

  env_function->args = ast_function->args;
  env_function->args_size = ast_function->args_size;

  env_function->code = ast_function->code;

  return env_function;
}

Env_function* init_env_function(AST_function* ast_function)
{
  Env_function* env_function =
    (Env_function*) malloc(sizeof(struct env_function_t));

  get_env_function_from_ast_function(env_function, ast_function);
  env_function->name = ast_function->name;
  env_function->length = ast_function->name_length;
  env_function->next = (void*) 0;

  return env_function;
}

Env_macro* init_env_macro(const char* name, int args_size, AST* code)
{
  Env_macro* env_macro =
    (Env_macro*) malloc(sizeof(struct env_macro_t));

  get_env_macro_from_ast_macro(env_macro, args_size, code);
  env_macro->name = (char*) name;
  env_macro->length = strlen(name);
  env_macro->next = (void*) 0;

  return env_macro;
}

Env_macro* get_env_macro_from_ast_macro(
    Env_macro* env_macro, int args_size, AST* code)
{
  env_macro->args_size = args_size;
  env_macro->code = code;

  return env_macro;
}

Env_variable* init_env_variable(char* name, size_t length)
{
  Env_variable* env_variable =
    (Env_variable*) malloc(sizeof(struct env_variable_t));
  env_variable->name = name;
  env_variable->length = length;
  env_variable->satisfy_size = 0;
  env_variable->satisfy = (void*) 0;
  env_variable->next = (void*) 0;

  return env_variable;
}

Env_value_list* init_env_value_list(struct ast_value_t* ast_value)
{
  Env_value_list* value_list = malloc(sizeof(Env_value_list));
  value_list->value = ast_value;
  value_list->next = (void*) 0;

  return value_list;
}

Env* init_env()
{
  Env* env = (Env*) malloc(sizeof(struct env_t));
  env->variable_size = 0;
//   env->variables = malloc(sizeof(struct env_variable_t));
//   env->variables->name = (void*) 0;
//   env->variables->length = 0;
//   env->variables->satisfy_size = 0;
//   env->variables->satisfy = (void*) 0;
//   env->variables->next = (void*) 0;
  env->variables = (void*) 0;

  env->function_size = 0;
//   env->functions = malloc(sizeof(struct env_function_t));
//   env->functions->name = (void*) 0;
//   env->functions->length = 0;
//   env->functions->next = (void*) 0;
  env->functions = (void*) 0;

  env->macro_size = 0;
  env->macros = (void*) 0;

  return env;
}

Envs* init_envs(Envs* global, Env* local)
{
  Envs* envs = (Envs*) malloc(sizeof(struct envs_t));
  envs->global = global;
  envs->local = local;

  return envs;
}

