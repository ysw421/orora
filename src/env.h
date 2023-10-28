#ifndef ENV_H
#define ENV_H

typedef struct env_t
{

// include packages, variables,
//         and functions...
// It used in Global or Functions
//                        - In School
//
// I love cat, I love Goorm, I love icecream
// I am the King of the world 👑

} Env;

typedef struct envs_t
{
  struct env_t* global;
  struct env_t* local;
} Envs;

Env* init_env();
Envs* init_envs(Env* global, Env* local);

#endif
