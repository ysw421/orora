#ifndef ENV_H
#define ENV_H

#include <stddef.h>
#include "./Parser/Ast/ast.h"

typedef struct env_function_t
{
  char* name;
  int length;

  struct ast_t** args;
  int args_size;

  struct ast_t* codes;

  enum
  {
    ENV_FUNCTION_TYPE_SINGLE,  // f(x)=3x+2
    ENV_FUNCTION_TYPE_NULL = 99
  } type;

  struct env_function_t* next;
} Env_function;

typedef struct env_value_list_t
{
  struct ast_value_t* value;
  struct env_value_list_t* next;
} Env_value_list;

typedef struct env_variable_t
{
  char* name;
  int length;

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
  struct env_value_list_t* satisfy;

  struct env_variable_t* next;
} Env_variable;

typedef struct env_t
{
  size_t variable_size;
  struct env_variable_t* variables;

  size_t function_size;
  struct env_function_t* functions;

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

Env_function* get_env_function_from_ast_function
(Env_function* env_function, AST_function* ast_function);
Env_function* init_env_function(AST_function* ast_function);
Env_variable* init_env_variable(char* name, size_t length);
Env_value_list* init_env_value_list(struct ast_value_t* ast_value);
Env* init_env();
Envs* init_envs(Env* global, Env* local);

#endif
