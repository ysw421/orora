#include "env.h"
#include <stdio.h>
#include <stdlib.h>

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
  env->variables = malloc(sizeof(struct env_variable_t*));

  return env;
}

Envs* init_envs(Env* global, Env* local)
{
  Envs* envs = (Envs*) malloc(sizeof(struct envs_t));
  envs->global = global;
  envs->local = local;

  return envs;
}
