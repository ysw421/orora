#include "env.h"
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

  env_function->type = ast_function->type;

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
  env->variables = malloc(sizeof(struct env_variable_t));

  env->function_size = 0;
  env->functions = malloc(sizeof(struct env_function_t));

  return env;
}

Envs* init_envs(Env* global, Env* local)
{
  Envs* envs = (Envs*) malloc(sizeof(struct envs_t));
  envs->global = global;
  envs->local = local;

  return envs;
}
