#include "env.h"
#include <stdio.h>
#include <stdlib.h>

Env* init_env()
{
  Env* env = (Env*) malloc(sizeof(struct env_t));

  return env;
}

Envs* init_envs(Env* global, Env* local)
{
  Envs* envs = (Envs*) malloc(sizeof(struct envs_t));
  envs->global = global;
  envs->local = local;

  return envs;
}
