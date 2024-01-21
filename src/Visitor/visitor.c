#include <string.h>
#include "visitor.h"
#include "../main.h"

#ifdef DEVELOP_MODE
#include <stdlib.h>
#include <stdio.h>
#endif

orora_value_type* get_single_value_type(int ast_type);
Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);

void visitor_visit(Envs* envs, AST* ast)
{
  switch (ast->type)
  {
    case AST_FUNCTION:
      AST_function* ast_function = ast->value.function_v;

#ifdef DEVELOP_MODE
      if (!strcmp(ast_function->name, "print"))
      {
        for (int i = 0; i < ast_function->args_size; i ++)
        {
          AST* ast_function_arg = ast_function->args[i];
          switch (ast_function_arg->type)
          {
            case AST_VALUE:
              AST_value* ast_function_arg_value =
                ast_function_arg->value.value_v;
              if (ast_function_arg_value->size == 1)
              {
                switch (ast_function_arg_value->stack->type)
                {
                  case AST_VALUE_STRING:
                    printf("%s",
                        ast_function_arg_value->stack->value.
                        string_v->real_value);
                    break;
                  case AST_VALUE_INT:
                    printf("%d",
                        ast_function_arg_value->stack->value.
                        int_v->value);
                    break;
                  case AST_VALUE_FLOAT:
                    printf("%f",
                        ast_function_arg_value->stack->value.
                        float_v->value);
                    break;
                }
              }
              break;
            case AST_VARIABLE:
              AST_variable* ast_function_arg_variable =
                ast_function_arg->value.variable_v;
              Env_variable* variable =
                visitor_get_variable(envs, ast_function_arg_variable);
              switch (variable->type)
              {
                case ENV_VARIABLE_STRING:
                  printf("%s", variable->value.string_v->real_value);
                  break;
                case ENV_VARIABLE_INT:
                  printf("%d", variable->value.int_v->value);
                  break;
                case ENV_VARIABLE_FLOAT:
                  printf("%f", variable->value.float_v->value);
                  break;
              }
              break;
          }
        }
      }
#endif
      break;

    case AST_VARIABLE:
      AST_variable* ast_variable = ast->value.variable_v;
      switch (ast_variable->ast_type)
      {
        case AST_VARIABLE_VALUE:
          break;

        case AST_VARIABLE_DEFINE:
          if (ast_variable->value->type == AST_VALUE
              && ast_variable->value->value.value_v->size == 1)
          {
            orora_value_type* variable_type =
              get_single_value_type(ast_variable->value->value.
                  value_v->stack->type);
            if (variable_type)
            {
              Env_variable* env_variable =
                init_env_variable(ast_variable->name, ast_variable->name_length);
              env_variable->type = variable_type->env_variable_type_id;
              env_variable =
                variable_type->visitor_set_env_variable(
                    env_variable,
                    ast_variable->value->value.value_v->stack
                  );
              Env* local_env = envs->local;
              env_variable->next = local_env->variables;
              local_env->variable_size ++;
              local_env->variables = env_variable;
            }
            else
            {}
          }
          break;

        case AST_VARIABLE_SATISFY:
          break;

        default:
#ifdef DEVELOP_MODE
          printf("내가 ast variable 설정 잘못함...");
          exit(1);
#endif
          break;
      }
      break;
  }
}

Env_variable* visitor_set_env_variable_int(Env_variable* env_variable,
    AST_value_stack* ast)
{
  env_variable->value.int_v = malloc(sizeof(struct ast_int_t));
  env_variable->value.int_v->value = ast->value.int_v->value;

  return env_variable;
}

Env_variable* visitor_set_env_variable_float(Env_variable* env_variable,
    AST_value_stack* ast)
{
  env_variable->value.float_v = malloc(sizeof(struct ast_float_t));
  env_variable->value.float_v->value = ast->value.float_v->value;

  return env_variable;
}

Env_variable* visitor_set_env_variable_string(Env_variable* env_variable,
    AST_value_stack* ast)
{
  env_variable->value.string_v = malloc(sizeof(struct ast_string_t));
  env_variable->value.string_v->value = ast->value.string_v->value;
  env_variable->value.string_v->real_value = ast->value.string_v->real_value;
  env_variable->value.string_v->value_length =
    ast->value.string_v->value_length;

  return env_variable;
}

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* check_variable = envs->local->variables;
  while (check_variable)
  {
    if (!strcmp(check_variable->name, ast_variable->name))
    {
      return check_variable;
    }
    check_variable = check_variable->next;
  }

  printf("에러, 정의되지 않은 변수");
  exit(1);
  return (void*) 0;
}

orora_value_type* get_single_value_type(int ast_type)
{
  orora_value_type* p = value_type_list;
  do
  {
    if (p->ast_value_type_id == ast_type)
      return p;
    p = p->next;
  } while (p);

  return (void*) 0;
}

void visitor_get_value()
{}
