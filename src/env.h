#ifndef ENV_H
#define ENV_H

#include <stddef.h>

typedef struct env_variable_t
{
  char* name;
  size_t length;

  enum
  {
    ENV_VARIABLE_INT,
    ENV_VARIABLE_FLOAT,
    ENV_VARIABLE_STRING
  } type;

  union
  {
    struct ast_int_t* int_v;
    struct ast_float_t* float_v;
    struct ast_string_t* string_v;
  } value;

  size_t satisfy_size;
  struct ast_value_t** satisfy;

  struct env_variable_t* next;
} Env_variable;

typedef struct env_t
{
  size_t variable_size;
  struct env_variable_t* variables;

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

Env_variable* init_env_variable(char* name, size_t length);
Env* init_env();
Envs* init_envs(Env* global, Env* local);

#endif
