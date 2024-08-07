#include <string.h>
#include <math.h>
#include "visitor/visitor.h"
#include "loader/main.h"
#include "parser/parser_value.h"
#include "loader/error_log.h"
#include "utilities/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include "syslib/print.h"

void visitor_print_function(Envs* envs, AST* ast);

GET_VISITOR_ENV* init_get_visitor_env();
orora_value_type* get_single_value_type(int ast_type);

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_get_function(Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable
(Envs* envs, AST_variable* ast_variable);

// AST_value_stack* visitor_function_value
// (Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_function_define(Envs* envs, AST_function* ast_function);

AST_value_stack* visitor_get_value(Envs* envs, AST_value* ast_value);

AST_value_stack* visitor_operator_plus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_product_(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_dot_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_div(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_equal(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_less(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_greater(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_power(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);

AST_value_stack* get_variable_from_Env_variable
(Envs* envs, AST_value_stack* ast);
Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable);

void visitor_nondefine_variable_error(AST_variable* ast_variable);
void visitor_nondefine_function_error(AST_function* ast_function);

bool is_true(AST_value_stack* value);
AST_value_stack* get_deep_copy_ast_value_stack
(AST_value_stack* ast_value_stack);
bool visitor_check_satisfy(Envs* envs, Env_variable* env_variable);

Envs* visitor_get_envs_from_function
(Envs* envs, AST_function* ast_function, Env_function* env_function);

Envs* visitor_merge_envs(Envs* envs);

AST_value_stack* visitor_get_value_from_ast
(Envs* envs, AST* condition);
AST_value_stack* visitor_get_value_from_function
(Envs* envs, AST_function* ast_function);
AST_value_stack* visitor_get_value_from_variable
(Envs* envs, Env_variable* env_variable);
AST_value_stack* visitor_get_value_from_cases
(Envs* envs, AST_cases* ast_cases);

GET_VISITOR_ENV* visitor_run_while(Envs* envs, AST_while* ast_while);
GET_VISITOR_ENV* visitor_run_if(Envs* envs, AST_if* ast_if);
GET_VISITOR_ENV* visitor_run_for(Envs* envs, AST_for* ast_for);
GET_VISITOR_ENV* visitor_run_code(Envs* envs, AST_compound* ast_code);
GET_VISITOR_ENV* visitor_run_cases(Envs* envs, AST_cases* ast_cases);


AST_value_stack* visitor_get_value_from_code
(Envs* envs, AST_compound* ast_code);

GET_VISITOR_ENV* init_get_visitor_env()
{
  GET_VISITOR_ENV* new_env = 
    (GET_VISITOR_ENV*)malloc(sizeof(struct get_visitor_env_t));

  new_env->is_break = false;
  new_env->is_continue = false;
  new_env->is_return = (void*) 0;
  new_env->output = (void*) 0;

  return new_env;
}

GET_VISITOR_ENV* visitor_visit(Envs* envs, AST* ast)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  switch (ast->type)
  {
    case AST_VARIABLE:
      AST_variable* ast_variable = ast->value.variable_v;
      Env_variable* env_variable = visitor_variable(envs, ast_variable);

      if (INTERACTIVE_MODE && ast_variable->ast_type == AST_VARIABLE_VALUE)
        get_visitor_env->output = 
          visitor_get_value_from_variable(envs, env_variable);
      else
        visitor_get_value_from_variable(envs, env_variable);
      break;

    case AST_FUNCTION:
      if (!strcmp(ast->value.function_v->name, "print"))
      {
        visitor_print_function(envs, ast);
        get_visitor_env->output = init_ast_value_stack(AST_VALUE_NULL, (void*) 0);
        break;
      }
      else
      {
        AST_function* ast_function = ast->value.function_v;
        switch (ast_function->ast_type)
        {
          case AST_FUNCTION_VALUE:
            if (INTERACTIVE_MODE)
              get_visitor_env->output =
                visitor_get_value_from_function(envs, ast_function);
            else
              visitor_get_value_from_function(envs, ast_function);
  //           visitor_function_value(envs, ast_function);
            break;

          case AST_FUNCTION_DEFINE:
            visitor_function_define(envs, ast_function);
            break;

          default:
#ifdef DEVELOP_MODE
            printf("에러,내가 ast function 설정 잘못함...\n");
            exit(1);
#endif
            break;
        }
      }
      break;

    case AST_WHILE:
      AST_while* ast_while = ast->value.while_v;
      get_visitor_env = 
        visitor_run_while(envs, ast_while);
      break;

    case AST_FOR:
      AST_for* ast_for = ast->value.for_v;
      get_visitor_env =
        visitor_run_for(envs, ast_for);
      break;

    case AST_CODE:
      get_visitor_env = 
        visitor_run_code(envs, ast->value.code_v->code);
      if (INTERACTIVE_MODE)
        get_visitor_env->output = get_visitor_env->is_return;
      break;

    case AST_CASES:
      get_visitor_env = 
        visitor_run_cases(envs, ast->value.cases_v);
      if (INTERACTIVE_MODE)
        get_visitor_env->output = get_visitor_env->is_return;
      break;

    case AST_VALUE:
      if (INTERACTIVE_MODE)
      {
        get_visitor_env->output = 
          visitor_get_value(envs, ast->value.value_v);
      }
      else 
        visitor_get_value(envs, ast->value.value_v);
      break;

    case AST_IF:
      AST_if* ast_if = ast->value.if_v;
      get_visitor_env = 
        visitor_run_if(envs, ast_if);
      break;

    case AST_BREAK:
      get_visitor_env->is_break = true;
      break;

    case AST_CONTINUE:
      get_visitor_env->is_continue = true;
      break;

    case AST_RETURN:
      get_visitor_env->is_return = 
        visitor_get_value_from_ast(envs, ast->value.return_v->value);
      break;

    default:
      printf("!!! %d\n", ast->type);
      return get_visitor_env;
  }

  return get_visitor_env;
}

// AST_value_stack* visitor_function_value
// (Envs* envs, AST_function* ast_function)
// {
//   Env_function* env_function =
//     visitor_get_function(envs, ast_function);
//   if (!env_function)
//   {
//     visitor_nondefine_function_error(ast_function);
//   }
// 
//   Envs* new_envs =
//     visitor_get_envs_from_function(envs, ast_function, env_function);
// 
//   AST_value_stack* result = 
//     visitor_get_value_from_ast(new_envs, env_function->code);
// 
//   free(new_envs);
//   
//   return result;
// }

Env_variable* visitor_variable
(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* env_variable;

  switch (ast_variable->ast_type)
  {
    case AST_VARIABLE_VALUE:
      env_variable = visitor_get_variable(envs, ast_variable);
      if (!env_variable)
      {
        visitor_nondefine_variable_error(ast_variable);
      }
      break;

    case AST_VARIABLE_DEFINE:
      env_variable = visitor_variable_define(envs, ast_variable);

      visitor_check_satisfy(envs, env_variable);
      break;

    case AST_VARIABLE_SATISFY:
      env_variable = visitor_variable_satisfy(envs, ast_variable);
          
      visitor_check_satisfy(envs, env_variable);
      break;

    default:
#ifdef DEVELOP_MODE
      printf("내가 ast variable 설정 잘못함...: %s\n", ast_variable->name);
      exit(1);
#endif
      break;
  }

  return env_variable;
}

bool visitor_check_satisfy(Envs* envs, Env_variable* env_variable)
{
  Env_value_list* value_list = env_variable->satisfy;
  for (int i = 0; i < env_variable->satisfy_size; i ++)
  {
    AST_value_stack* value =
      visitor_get_value(envs, value_list->value);

    if (!is_true(value))
    {
      printf("에러, satisfy 조건에 만족하지 아니함\n");
      exit(1);
    }

    value_list = value_list->next;
  }

  return true;
}

bool is_true(AST_value_stack* value)
{
  switch (value->type)
  {
    case AST_VALUE_INT:
      if (value->value.int_v->value == 0)
        return false;
      break;
    
    case AST_VALUE_FLOAT:
      if (value->value.float_v->value == 0)
        return false;
      break;

    case AST_VALUE_NULL:
      return false;
      break;

    case AST_VALUE_BOOL:
      if (!value->value.bool_v->value)
        return false;
      break;

    default:
      return true;
      break;
  }

  return true;
}

void visitor_nondefine_variable_error(AST_variable* ast_variable)
{
//   printf("에러, 정의되지 않은 변수: %s\n", ast_variable->name);
//   exit(1);
  const char* error_log = "에러 정의되지 않은 변수: ";
  error_log = const_strcat(error_log, ast_variable->name);
  orora_error(error_log, (void*) 0);
}

void visitor_nondefine_function_error(AST_function* ast_function)
{
  const char* error_log = "에러 정의되지 않은 함수: ";
  error_log = const_strcat(error_log, ast_function->name);
  orora_error(error_log, (void*) 0);
//   printf("에러, 정의되지 않은 함수: %s\n", ast_function->name);
//   exit(1);
}

Env_function* visitor_get_function(Envs* envs, AST_function* ast_function)
{
  Env_function* check_function = envs->local->functions;

  Env_function* snode = (void*) 0;
  while (check_function)
  {
    if (!strcmp(check_function->name, ast_function->name))
    {
      if (snode)
      {
        snode->next = check_function->next;
        check_function->next = envs->local->functions;
        envs->local->functions = check_function;
      }
      return check_function;
    }
    if (snode)
    {
      snode = snode->next;
    }
    else
    {
      snode = envs->local->functions;
    }
    if (!check_function->next)
      break;

    check_function = check_function->next;
  }

  if (!envs->global)
    return (void*) 0;

  return visitor_get_function(envs->global, ast_function);
}

AST_value_stack* visitor_get_value_from_ast
(Envs* envs, AST* condition)
{
  AST_value_stack* condition_value;
  switch (condition->type)
  {
    case AST_VALUE:
      condition_value = 
        visitor_get_value(envs, condition->value.value_v);
      break;

    case AST_CODE:
      Envs* new_envs = visitor_merge_envs(envs);

      condition_value = 
        visitor_get_value_from_code(
            new_envs, 
            condition->value.code_v->code
        );
      free(new_envs);
      break;

    case AST_CASES:
      condition_value = 
        visitor_get_value_from_cases(
            envs,
            condition->value.cases_v
        );

      break;

    case AST_VARIABLE:
      Env_variable* env_variable =
        visitor_get_variable(envs, condition->value.variable_v);
      if (!env_variable)
      {
        visitor_nondefine_variable_error(condition->value.variable_v);
      }

      condition_value = 
        visitor_get_value_from_variable(envs, env_variable);
      break;

    case AST_FUNCTION:
//       Env_function* env_function =
//         visitor_get_function(envs, condition->value.function_v);
//       if (!env_function)
//       {
//         visitor_nondefine_function_error(
//              condition->value.function_v
//         );
//       }

      condition_value = 
        visitor_get_value_from_function(
            envs,
            condition->value.function_v
        );
      break;

    default:
      const char* error_message = "에러, while문의 조건이 잘못됨: ";
      error_message = const_strcat(error_message, int_to_string(condition->type));
      orora_error(error_message, (void*) 0);
//       printf("에러, while문의 조건이 잘못됨: %d\n", condition->type);
//       exit(1);
      break;
  }

  return condition_value;
}

AST_value_stack* visitor_get_value_from_cases
(Envs* envs, AST_cases* ast_cases)
{
  int size = ast_cases->size;
  bool is_have_otherwise = ast_cases->is_have_otherwise;
  AST** codes = ast_cases->codes;
  AST** conditions = ast_cases->conditions;

  Envs* new_envs = visitor_merge_envs(envs);
  for (int i = 0; i < size; i ++)
  {
    if (i + 1 == size && is_have_otherwise)
    {
       return visitor_get_value_from_ast(new_envs, codes[i]);
    }
    else if (is_true(visitor_get_value_from_ast(new_envs, conditions[i])))
    {
      return visitor_get_value_from_ast(new_envs, codes[i]);
    }
  }
  free(new_envs);
  
  return init_ast_value_stack(AST_VALUE_NULL, (void*) 0);
}

AST_value_stack* visitor_get_value_from_code
(Envs* envs, AST_compound* ast_code)
{
  for (int i = 0; i < ast_code->size; i ++)
  {
    AST* selected_ast = ast_code->items[i];
    if (selected_ast->type == AST_RETURN)
      return visitor_get_value_from_ast(
                envs, 
                selected_ast->value.return_v->value 
              );

    GET_VISITOR_ENV* get_visitor_env = 
      visitor_visit(
          envs, 
          selected_ast 
        );

    if (get_visitor_env->is_return)
      return get_visitor_env->is_return;
  }

  return init_ast_value_stack(AST_VALUE_NULL, (void*) 0);
}

AST_value_stack* visitor_get_value_from_function
(Envs* envs, AST_function* ast_function)
{
//   if (!env_function)
//   {
//     visitor_nondefine_function_error(ast_function);
//   }

  // Set New Environment: new_envs
//   Envs* new_envs =
//     visitor_get_envs_from_function(envs, ast_function, env_function);

//   return visitor_function_value(envs, ast_function);

  Env_function* env_function =
    visitor_get_function(envs, ast_function);
  if (!env_function)
  {
    if (!strcmp(ast_function->name, "print"))
    {
      AST* ast = init_ast(AST_FUNCTION, (void*) 0, (void*) 0);
      ast->value.function_v = ast_function;
      visitor_print_function(envs, ast);
      return init_ast_value_stack(AST_VALUE_NULL, (void*) 0);
    }
    visitor_nondefine_function_error(ast_function);
  }

  Envs* new_envs =
    visitor_get_envs_from_function(envs, ast_function, env_function);

  AST_value_stack* result = 
    visitor_get_value_from_ast(new_envs, env_function->code);

  free(new_envs);
  
  return result;
}

AST_value_stack* visitor_get_value_from_variable
(Envs* envs, Env_variable* env_variable)
{
//   if (!env_variable)
//   {
//     visitor_nondefine_variable_error(text->value.variable_v);
//   }

  AST_value_stack* new_value_stack = malloc(sizeof(AST_value_stack));
  orora_value_type* p = value_type_list;
  do
  {
    if (p->env_variable_type_id == env_variable->type)
      break;
    p = p->next;
  } while (p);

  if (p)
    new_value_stack =
      p->visitor_set_value_AST_value_stack_from_Env_variable
        (new_value_stack, env_variable);
  else
  {
    orora_error("에러, 변수에 저장하는 것은 값이어야함", (void*) 0);
//     printf("에러, 변수에 저장하는 것은 값이어야함\n");
//     exit(1);
  }

  return new_value_stack;
}

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* check_variable = envs->local->variables;

  Env_variable* snode = (void*) 0;
  
  while (check_variable)
  {
    if (!strcmp(check_variable->name, ast_variable->name))
    {
      if (snode)
      {
        snode->next = check_variable->next;
        check_variable->next = envs->local->variables;
        envs->local->variables = check_variable;
      }
      return check_variable;
    }
    if (snode)
      snode = snode->next;
    else
      snode = envs->local->variables;

    if (!check_variable->next)
      break;

    check_variable = check_variable->next;
  }

  if (!envs->global)
    return (void*) 0;

  return visitor_get_variable(envs->global, ast_variable);
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

Env_function* visitor_function_define(Envs* envs, AST_function* ast_function)
{
  Env_function* env_function =
    visitor_get_function(envs, ast_function);

  if (env_function)
  {
    get_env_function_from_ast_function(env_function, ast_function);
  }
  else
  {
    env_function = init_env_function(ast_function);

    Env* local_env = envs->local;
    env_function->next = local_env->functions;
    local_env->function_size ++;
    local_env->functions = env_function;
  }

  return env_function;
}

Env_variable* visitor_variable_define_from_value
(Envs* envs, AST_variable* ast_variable, AST_value* ast_value)
{
  bool is_defined_variable;
  if (ast_value->size == 1)
  {
    orora_value_type* variable_type =
      get_single_value_type(ast_value->stack->type);
    if (variable_type)
    {
      Env_variable* env_variable =
        visitor_get_variable(envs, ast_variable);
          
      if (env_variable)
      {
        env_variable =
          variable_type->visitor_set_value_Env_variable_from_AST_value_stack(
                env_variable, ast_value->stack);
      }
      else
      {
        env_variable = init_env_variable(ast_variable->name,
                                          ast_variable->name_length);
        env_variable =
          variable_type->visitor_set_value_Env_variable_from_AST_value_stack(
              env_variable, ast_value->stack);

        Env* local_env = envs->local;
        env_variable->next = local_env->variables;
        local_env->variable_size ++;
        local_env->variables = env_variable;
      }

      return env_variable;
    }
    else
    {
      orora_error("에러, 변수에는 값을 저장해야함", (void*) 0);
//       printf("에러, 변수에는 값을 저장해야함\n");
//       exit(1);
    }
  }
  else
  {
    Env_variable* env_variable =
      visitor_get_variable(envs, ast_variable);
    if (env_variable)
    {
      is_defined_variable = true;
    }
    else
    {
      is_defined_variable = false;
      env_variable =
        init_env_variable(ast_variable->name,
            ast_variable->name_length);
    }

    AST_value_stack* new_value =
      visitor_get_value(envs, ast_value);

    orora_value_type* p = value_type_list;
    do
    {
      if (p->ast_value_type_id == new_value->type)
        break;
      p = p->next;
    } while (p);

    if (p)
      env_variable =
        p->visitor_set_value_Env_variable_from_AST_value_stack
          (env_variable, new_value);
    else
    {
      orora_error("에러, 변수에 저장하는 것은 값이어야함", (void*) 0);
//       printf("에러, 변수에 저장하는 것은 값이어야함\n");
//       exit(1);
    }

    if (!is_defined_variable)
    {
      Env* local_env = envs->local;
      env_variable->next = local_env->variables;
      local_env->variable_size ++;
      local_env->variables = env_variable;
    }

    return env_variable;
  }

  return (void*) 0;
}

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable)
{
  bool is_defined_variable;
  switch (ast_variable->value->type)
  {
    case AST_VARIABLE:
      Env_variable* env_variable;
      Env* local_env = envs->local;
      Env_variable* value_variable;
      switch (ast_variable->value->value.variable_v->ast_type)
      {
        case AST_VARIABLE_VALUE:
          env_variable = visitor_get_variable(envs, ast_variable);
          if (env_variable)
          {
            is_defined_variable = true;
          }
          else
          {
            is_defined_variable = false;
            env_variable =
              init_env_variable(ast_variable->name,
                    ast_variable->name_length);
          }

          value_variable =
            visitor_get_variable(envs,
                ast_variable->value->value.variable_v);
          if (!value_variable)
          {
            visitor_nondefine_variable_error(
                ast_variable->value->value.variable_v);
          }

          env_variable->type = value_variable->type;
          env_variable->value = value_variable->value;

          if (!is_defined_variable)
          {
            env_variable->next = local_env->variables;
            local_env->variable_size ++;
            local_env->variables = env_variable;
          }

          return env_variable;
          break;

        case AST_VARIABLE_DEFINE:
          env_variable = visitor_get_variable(envs, ast_variable);
          if (env_variable)
          {
            is_defined_variable = true;
          }
          else
          {
            is_defined_variable = false;
            env_variable =
              init_env_variable(ast_variable->name,
                    ast_variable->name_length);
          }

          value_variable =
            visitor_variable_define(envs,
                ast_variable->value->value.variable_v);
          env_variable->type = value_variable->type;
          env_variable->value = value_variable->value;

          if (!is_defined_variable)
          {
            env_variable->next = local_env->variables;
            local_env->variable_size ++;
            local_env->variables = env_variable;
          }

          return env_variable;
          break;

        default:
          orora_error("에러, 변수에 저장하는 것은 값이어야함", (void*) 0);
//           printf("에러, 정의되지 않은 변수의 활용\n");
//           exit(1);
      }
      break;

    default:
      AST_value* ast_get_value = init_ast_value();
      ast_get_value->size = 1;
      ast_get_value->stack = 
        visitor_get_value_from_ast(envs, ast_variable->value);

      return visitor_variable_define_from_value(
                envs,
                ast_variable,
                ast_get_value
              );
      break;
  }
  return (void*) 0;
}

Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* get_variable =
    visitor_get_variable(envs, ast_variable);
  if (!get_variable)
  {
    visitor_nondefine_variable_error(ast_variable);
  }

  get_variable->satisfy_size += ast_variable->satisfy_size;
  for (size_t i = 0; i < ast_variable->satisfy_size; i ++)
  {
    Env_value_list* value_list =
      init_env_value_list(ast_variable->satisfy[i]->value.value_v);
    value_list->next = get_variable->satisfy;
    get_variable->satisfy = value_list;
  }
  return get_variable;
}

AST_value_stack* get_deep_copy_ast_value_stack
(AST_value_stack* ast_value_stack)
{

  AST_value_stack* new_value_stack = malloc(sizeof(AST_value));
  new_value_stack->type = ast_value_stack->type;
  new_value_stack->value = ast_value_stack->value;
  if (ast_value_stack->next)
    new_value_stack->next = ast_value_stack->next;

//   new_value_stack->col = ast_value_stack->col;
//   new_value_stack->col_first = ast_value_stack->col_first;
//   new_value_stack->row = ast_value_stack->row;
//   new_value_stack->row_char = ast_value_stack->row_char;
//   new_value_stack->row_char_first = ast_value_stack->row_char_first;

  return new_value_stack;
}

AST_value_stack* visitor_get_value(Envs* envs, AST_value* ast_value)
{
  AST_value* stack = init_ast_value();
  AST_value_stack* text;
  int max_cnt = ast_value->size;
  AST_value_stack** text_array =
    malloc(max_cnt * sizeof(AST_value_stack*));
  AST_value_stack* p = ast_value->stack;
  for (int i = max_cnt - 1; i >= 0; i --)
  {
    text_array[i] = get_deep_copy_ast_value_stack(p);
    
    p = p->next;
  }

  int i = 0;
  while (i < max_cnt)
  {

    text = text_array[i];
    i ++;
    if (parser_precedence(text->type) == -1)
    {
      AST_value_stack* new_value_stack;
      switch (text->type)
      {
        case AST_VALUE_VARIABLE:
          Env_variable* env_variable =
            visitor_get_variable(envs, text->value.variable_v);
          if (!env_variable)
          {
            visitor_nondefine_variable_error(text->value.variable_v);
          }

          new_value_stack =
            visitor_get_value_from_variable(envs, env_variable);
          if (!strcmp(env_variable->name, "T"))
            new_value_stack->is_T = true;

          parser_push_value(stack, new_value_stack);
          break;

        case AST_VALUE_FUNCTION:
          AST_function* ast_function = text->value.function_v;
//           Env_function* env_function =
//             visitor_get_function(envs, text->value.function_v);
//           if (!env_function)
//           {
//             visitor_nondefine_function_error(text->value.function_v);
//           }

          new_value_stack =
            visitor_get_value_from_function(envs, ast_function);

          parser_push_value(stack, new_value_stack);
          break;

        case AST_VALUE_CASES:
          parser_push_value(
              stack, 
              visitor_get_value_from_cases(envs, text->value.cases_v) 
            );
          break;

        case AST_VALUE_CODE:
          parser_push_value(
              stack, 
              visitor_get_value_from_code(envs, text->value.code_v->code) 
            );
          break;

        case AST_VALUE_MATRIX:
          AST_matrix* matrix_value = text->value.matrix_v;
          for (
               int i = 0; 
               i < matrix_value->row_size * matrix_value->col_size;
               i ++
              )
          {
            AST* matrix_each_value = 
              init_ast(AST_VALUE, (void*) 0, (void*) 0);
            matrix_each_value->value.value_v = init_ast_value();
            matrix_each_value->value.value_v->size = 1;
            matrix_each_value->value.value_v->stack = 
              visitor_get_value_from_ast(envs, matrix_value->value[i]);
            free(matrix_value->value[i]);
            matrix_value->value[i] = 
              matrix_each_value;
          }
          AST_value_stack* new_matrix_value = 
            init_ast_value_stack(AST_VALUE_MATRIX, (void*) 0);
          new_matrix_value->value.matrix_v = matrix_value;
          parser_push_value(stack, new_matrix_value);
          break;

        default:
          parser_push_value(stack, text);
          break;
      }
    }
    else
    {
      AST_value_stack* operand1;
      AST_value_stack* operand2;
      operand2 = parser_pop_value(stack);

      if (!is_operator_use_one_value(get_token_type(text->type))
          || text->type == AST_VALUE_MINUS)
      {
        operand1 = parser_pop_value(stack);
      }

      AST_value_stack* result = malloc(sizeof(AST_value_stack));
      switch (text->type)
      {
        case AST_VALUE_PLUS:
          result = visitor_operator_plus(result, operand1, operand2);
          break;

        case AST_VALUE_MINUS:
          result = visitor_operator_minus(result, operand1, operand2);
          break;

        case AST_VALUE_PRODUCT:
          result = visitor_operator_product(result, operand1, operand2);
          break;

        case AST_VALUE_DOT_PRODUCT:
          result = visitor_operator_dot_product(result, operand1, operand2);
          break;

        case AST_VALUE_DIV:
          result = visitor_operator_div(result, operand1, operand2);
          break;

        case AST_VALUE_CIRCUMFLEX:
          result = visitor_operator_power(result, operand1, operand2);
          break;

        case AST_VALUE_EQUAL:
          result = visitor_operator_equal(result, operand1, operand2);
          break;

        case AST_VALUE_NOTEQUAL:
          result = visitor_operator_equal(result, operand1, operand2);
          result->value.bool_v->value = !result->value.bool_v->value;
          break;

        case AST_VALUE_LESS:
          result = visitor_operator_less(result, operand1, operand2);
          break;

        case AST_VALUE_GREATER:
          result = visitor_operator_greater(result, operand1, operand2);
          break;

        case AST_VALUE_LESSEQUAL:
          result->type = AST_VALUE_BOOL;
          result->value.bool_v = malloc(sizeof(struct ast_bool_t));

          result->value.bool_v->value = 
            visitor_operator_less(result, operand1, operand2)
              ->value.bool_v->value
                || visitor_operator_equal(result, operand1, operand2)
                   ->value.bool_v->value;
          break;

        case AST_VALUE_GREATEREQUAL:
          result->type = AST_VALUE_BOOL;
          result->value.bool_v = malloc(sizeof(struct ast_bool_t));

          result->value.bool_v->value = 
            visitor_operator_greater(result, operand1, operand2)
              ->value.bool_v->value
                || visitor_operator_equal(result, operand1, operand2)
                   ->value.bool_v->value;
          break;

        case AST_VALUE_OR:
          result->type = AST_VALUE_BOOL;
          result->value.bool_v = malloc(sizeof(struct ast_bool_t));

          result->value.bool_v->value = 
            is_true(operand1) || is_true(operand2);
          break;

        case AST_VALUE_AND:
          result->type = AST_VALUE_BOOL;
          result->value.bool_v = malloc(sizeof(struct ast_bool_t));

          result->value.bool_v->value = 
            is_true(operand1) && is_true(operand2);
          break;

        case AST_VALUE_NEG:
          result->type = AST_VALUE_BOOL;
          result->value.bool_v = malloc(sizeof(struct ast_bool_t));

          result->value.bool_v->value = !is_true(operand2);
          break;

        default:
          printf("에러, 연산자를 추가하지 아니하였음\n");
          exit(1);
      }

      parser_push_value(stack, result);
//       free(text);
//       if (!is_operator_use_one_value(get_token_type(text->type))
//           || text->type == AST_VALUE_MINUS)
//       {
//         free(operand1);
//       }
//       free(operand2);
    }
  }

  if (stack->size != 1)
  {
    orora_error("에러, 연산 불가함", (void*) 0);
//     printf("에러, 연산 불가함\n");
//     exit(1);
  }

  free(text_array);

  AST_value_stack* return_value = parser_pop_value(stack);
  free(stack);
  return return_value;
}

Env_variable* get_deep_copy_env_variable
(Env_variable* env_variable)
{
  Env_variable* new_env_variable = malloc(sizeof(Env_variable));

  new_env_variable->name = env_variable->name;
  new_env_variable->length = env_variable->length;
  new_env_variable->type = env_variable->type;
  new_env_variable->value = env_variable->value;
  new_env_variable->satisfy_size = env_variable->satisfy_size;
  new_env_variable->satisfy = env_variable->satisfy;
  new_env_variable->next = env_variable->next;

  return new_env_variable;
}

Env_function* get_deep_copy_env_funtion
(Env_function* env_function)
{
  Env_function* new_env_funtion = malloc(sizeof(Env_function));

  new_env_funtion->name = env_function->name;
  new_env_funtion->length = env_function->length;
  new_env_funtion->args = env_function->args;
  new_env_funtion->args_size = env_function->args_size;
  new_env_funtion->code = env_function->code;
  new_env_funtion->next = env_function->next;

  return new_env_funtion;
}

Envs* visitor_merge_envs(Envs* envs)
{
  Envs* new_envs = init_envs(envs, init_env());

  return new_envs;
}

Envs* visitor_get_envs_from_function
(Envs* envs, AST_function* ast_function, Env_function* env_function)
{
  Envs* new_envs = visitor_merge_envs(envs);

  // Get value from arguments...
  if (ast_function->args_size > env_function->args_size)
  {
    orora_error("에러, 함수의 매개변수 개수가 다름", (void*) 0);
//     printf("에러, 함수의 매개변수 개수가 다름\n");
//     exit(1);
  }

  for (int i = 0; i < env_function->args_size; i ++)
  {
    AST* ast = ast_function->args[i];
    if (i < ast_function->args_size)
    {
      AST_variable* ast_variable =
        env_function->args[i]->value.variable_v;
      Env_variable* env_variable =
        init_env_variable(ast_variable->name,
                          ast_variable->name_length);

      if (ast->type == AST_VARIABLE)
      {
        switch (ast->value.variable_v->ast_type)
        {
          case AST_VARIABLE_VALUE:
            Env_variable* value_variable =
              visitor_get_variable(new_envs,
                  ast->value.variable_v);
            if (!value_variable)
            {
              visitor_nondefine_variable_error(
                  ast->value.variable_v);
            }

            env_variable->type = value_variable->type;
            env_variable->value = value_variable->value;
            break;
            
          case AST_VARIABLE_DEFINE:
            value_variable =
              visitor_variable_define(new_envs,
                  ast->value.variable_v);
            env_variable->type = value_variable->type;
            env_variable->value = value_variable->value;
            break;

          default:
            orora_error("에러, 해당 에러는 발생 불가함", (void*) 0);
//             printf("에러, 해당 에러는 발생 불가함\n");
//             exit(1);
        }
      }
      else
      {
        AST_value_stack* new_value = 
          visitor_get_value_from_ast(new_envs, ast);
        orora_value_type* variable_type =
          get_single_value_type(new_value->type);
        env_variable =
          variable_type
            ->visitor_set_value_Env_variable_from_AST_value_stack(
                env_variable,
                new_value
              );
      }
//       switch (ast->type)
//       {
//         case AST_VALUE:
//           if (ast->value.value_v->size == 1)
//           {
//             orora_value_type* variable_type =
//               get_single_value_type(ast->value.value_v->stack->type);
// 
//             if (variable_type)
//             {
//               env_variable =
//                 variable_type
//                   ->visitor_set_value_Env_variable_from_AST_value_stack(
//                       env_variable,
//                       ast->value.value_v->stack
//                     );
//             }
//             else
//             {
//               printf("에러, 변수에는 값을 저장해야함1\n");
//               exit(1);
//             }
//           }
//           else
//           {
//             AST_value_stack* new_value =
//               visitor_get_value(new_envs, ast->value.value_v);
// 
//             orora_value_type* p = value_type_list;
//             do
//             {
//               if (p->ast_value_type_id == new_value->type)
//                 break;
//               p = p->next;
//             } while (p);
// 
//             if (p)
//               env_variable =
//                 p->visitor_set_value_Env_variable_from_AST_value_stack
//                   (env_variable, new_value);
//             else
//             {
//               printf("에러, 변수에 저장하는 것은 값이어야함\n");
//               exit(1);
//             }
//           }
//           break;
// 
//         case AST_VARIABLE:
//           switch (ast->value.variable_v->ast_type)
//           {
//             case AST_VARIABLE_VALUE:
//               Env_variable* value_variable =
//                 visitor_get_variable(new_envs,
//                     ast->value.variable_v);
//               if (!value_variable)
//               {
//                 visitor_nondefine_variable_error(
//                     ast->value.variable_v);
//               }
// 
//               env_variable->type = value_variable->type;
//               env_variable->value = value_variable->value;
//               break;
//               
//             case AST_VARIABLE_DEFINE:
//               value_variable =
//                 visitor_variable_define(new_envs,
//                     ast->value.variable_v);
//               env_variable->type = value_variable->type;
//               env_variable->value = value_variable->value;
//               break;
// 
//             default:
//               printf("에러, 해당 에러는 발생 불가함\n");
//               exit(1);
//           }
// 
//           break;
// 
//         case AST_FUNCTION:
//           Env_function* env_function =
//             visitor_get_function(envs, ast->value.function_v);
//           if (!env_function)
//           {
//             visitor_nondefine_function_error(
//                 ast_variable->value->value.function_v
//             );
//           }
//           AST_value_stack* ast_value_stack
//             = visitor_get_value_from_function(
//                   envs,
//                   ast->value.function_v,
//                   env_function
//                 );
//           orora_value_type* variable_type =
//             get_single_value_type(ast_value_stack->type);
// 
//           if (variable_type)
//           {
//             env_variable =
//               variable_type
//                 ->visitor_set_value_Env_variable_from_AST_value_stack(
//                     env_variable,
//                     ast_value_stack
//                   );
//           }
//           else
//           {
//             printf("에러, 변수에는 값을 저장해야함1\n");
//             exit(1);
//           }
//           break;
// 
//         default:
//           printf("에러, 정의되지 아니한 타입\n");
//           exit(1);
//       }

      Env* local_env = new_envs->local;
      env_variable->next = local_env->variables;
      local_env->variable_size ++;
      local_env->variables = env_variable;
    }
    else
    {
      if (env_function->args[i]
          ->value.variable_v->ast_type == AST_VARIABLE_DEFINE)
      {
        visitor_variable_define(new_envs,
            env_function->args[i]->value.variable_v);
      }
      else
      {
        orora_error("에러, 함수의 매개변수 개수가 다름", (void*) 0);
//         printf("에러, 함수의 매개변수 개수가 다름\n");
//         exit(1);
      }
    }
  }

  return new_envs;
}

AST_value_stack* get_variable_from_Env_variable(Envs* envs, AST_value_stack* ast)
{
  Env_variable* env_variable =
    visitor_get_variable(envs, ast->value.variable_v);
  if (!env_variable)
  {
    visitor_nondefine_variable_error(ast->value.variable_v);
  }

  AST_value_stack* new_value_stack = malloc(sizeof(AST_value_stack));
  orora_value_type* p = value_type_list;
  do
  {
    if (p->env_variable_type_id == env_variable->type)
      break;
    p = p->next;
  } while (p);

  if (p)
    new_value_stack =
      p->visitor_set_value_AST_value_stack_from_Env_variable
        (new_value_stack, env_variable);
  else
  {
    orora_error("에러, 변수에 저장하는 것은 값이어야함", (void*) 0);
//     printf("에러, 변수에 저장하는 것은 값이어야함\n");
//     exit(1);
  }

  return new_value_stack;
}


AST_value_stack* visitor_operator_plus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  if (op1 == AST_VALUE_NULL || op2 == AST_VALUE_NULL)
  {
    orora_error("에러, null은 + 연산이 불가함", (void*) 0);
//     printf("에러, null은 + 연산이 불가함\n");
//     exit(1);
  }
  else if (op1 == AST_VALUE_STRING || op2 == AST_VALUE_STRING)
  {
    orora_error("에러, string은 + 연산이 불가함", (void*) 0);
//     printf("에러, string은 + 연산이 불가함\n");
//     exit(1);
  }
  switch (op1 + op2 * 100)
  {
    case AST_VALUE_INT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        operand1->value.int_v->value + operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_INT)
        result->value.float_v->value =
          operand1->value.int_v->value + operand2->value.float_v->value;
      else
        result->value.float_v->value =
          operand1->value.float_v->value + operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      if (op1 == AST_VALUE_INT)
        result->value.int_v->value =
          operand1->value.int_v->value + (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value =
          (operand1->value.bool_v->value ? 1 : 0) + operand2->value.int_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      result->value.float_v->value =
        operand1->value.float_v->value + operand2->value.float_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_FLOAT)
        result->value.int_v->value = 
          operand1->value.float_v->value + (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value = 
          (operand1->value.bool_v->value ? 1 : 0) + operand2->value.float_v->value;
      break;

    case AST_VALUE_BOOL + AST_VALUE_BOOL * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        (operand1->value.bool_v->value ? 1 : 0) 
          + (operand2->value.bool_v->value ? 1 : 0);
      break;

    case AST_VALUE_MATRIX + AST_VALUE_MATRIX * 100:
      AST_matrix* matrix_value = operand1->value.matrix_v;
      if (
          matrix_value->row_size != operand2->value.matrix_v->row_size
          || matrix_value->col_size != operand2->value.matrix_v->col_size
         )
      {
        orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
//         printf("에러, 행렬 덧셈시 크기가 같아야 함\n");
//         exit(1);
      }

      int matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_plus(
            new_value->value.value_v->stack, 
            matrix_value->value[i]->value.value_v->stack, 
            operand2->value.matrix_v->value[i]->value.value_v->stack
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    case AST_VALUE_MATRIX + AST_VALUE_INT * 100:
    case AST_VALUE_INT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_BOOL + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_BOOL * 100:
      matrix_value = 
        op1 == AST_VALUE_MATRIX ?
          operand1->value.matrix_v
          : operand2->value.matrix_v;
      matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_plus(
            new_value->value.value_v->stack, 
            op1 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand1, 
            op2 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand2
          );
//         free(matrix_value->value[i]->value.value_v);
        result->value.matrix_v->value[i] = new_value;
      }
//       free(matrix_value->value);
//       free(matrix_value);
//       free(op1 == AST_VALUE_MATRIX ? operand2 : operand1);
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  return result;
}

AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  if (op1 == AST_VALUE_NULL || op2 == AST_VALUE_NULL)
  {
    orora_error("에러, null은 + 연산이 불가함", (void*) 0);
//     printf("에러, null은 + 연산이 불가함\n");
//     exit(1);
  }
  else if (op1 == AST_VALUE_STRING || op2 == AST_VALUE_STRING)
  {
    orora_error("에러, string은 + 연산이 불가함", (void*) 0);
//     printf("에러, string은 + 연산이 불가함\n");
//     exit(1);
  }
  switch (op1 + op2 * 100)
  {
    case AST_VALUE_INT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        operand1->value.int_v->value - operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_INT)
        result->value.float_v->value =
          operand1->value.int_v->value - operand2->value.float_v->value;
      else
        result->value.float_v->value =
          operand1->value.float_v->value - operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      if (op1 == AST_VALUE_INT)
        result->value.int_v->value =
          operand1->value.int_v->value - (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value =
          (operand1->value.bool_v->value ? 1 : 0) - operand2->value.int_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      result->value.float_v->value =
        operand1->value.float_v->value - operand2->value.float_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_FLOAT)
        result->value.int_v->value = 
          operand1->value.float_v->value - (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value = 
          (operand1->value.bool_v->value ? 1 : 0) - operand2->value.float_v->value;
      break;

    case AST_VALUE_BOOL + AST_VALUE_BOOL * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        (operand1->value.bool_v->value ? 1 : 0) 
          - (operand2->value.bool_v->value ? 1 : 0);
      break;

    case AST_VALUE_MATRIX + AST_VALUE_MATRIX * 100:
      AST_matrix* matrix_value = operand1->value.matrix_v;
      if (
          matrix_value->row_size != operand2->value.matrix_v->row_size
          || matrix_value->col_size != operand2->value.matrix_v->col_size
         )
      {
        orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
//         printf("에러, 행렬 덧셈시 크기가 같아야 함\n");
//         exit(1);
      }

      int matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_minus(
            new_value->value.value_v->stack, 
            matrix_value->value[i]->value.value_v->stack, 
            operand2->value.matrix_v->value[i]->value.value_v->stack
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    case AST_VALUE_MATRIX + AST_VALUE_INT * 100:
    case AST_VALUE_INT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_BOOL + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_BOOL * 100:
      matrix_value = 
        op1 == AST_VALUE_MATRIX ?
          operand1->value.matrix_v
          : operand2->value.matrix_v;
      matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_minus(
            new_value->value.value_v->stack, 
            op1 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand1, 
            op2 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand2
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  return result;
}

AST_value_stack* visitor_matrix_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix1 = operand1->value.matrix_v;
  AST_matrix* matrix2 = operand2->value.matrix_v;

  if (matrix1->row_size != matrix2->col_size)
  {
    orora_error("에러, 행렬의 product시 첫 행렬의 행과 두번째 행렬의 열의 크기가 같아야 함", (void*) 0);
    return (void*) 0;
  }

  int m = matrix1->col_size;
  int n = matrix2->row_size;
  int matrix_size = m*n;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->col_size = m;
  result->value.matrix_v->row_size = n;

  result->value.matrix_v->value = 
    malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
    {
      AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
      new_value->value.value_v = init_ast_value();
      new_value->value.value_v->size = 1;
      new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
      AST_value_stack* init_stack = init_ast_value_stack(AST_VALUE_INT, (void*) 0);
      init_stack->value.int_v = malloc(sizeof(AST_int));
      init_stack->value.int_v->value = 0;

      for (int k = 0; k < matrix1->row_size; k++)
      {
        AST_value_stack* product_stack = malloc(sizeof(AST_value_stack));

        visitor_operator_dot_product(
            product_stack,
            matrix1->value[i*m + k]->value.value_v->stack,
            matrix2->value[k*n + j]->value.value_v->stack
          );

        if (k == 0)
          visitor_operator_plus(
              new_value->value.value_v->stack,
              init_stack,
              product_stack
            );
        else
        {
          AST_value_stack* plus_stack = malloc(sizeof(AST_value_stack));
          visitor_operator_plus(
              plus_stack,
              new_value->value.value_v->stack,
              product_stack
            );
          new_value->value.value_v->stack = plus_stack;
        }
        free(product_stack);
      }
      result->value.matrix_v->value[i*n + j] = new_value;
    }
  }

  return result;
}

AST_value_stack* visitor_operator_product_(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  if (op1 == AST_VALUE_NULL || op2 == AST_VALUE_NULL)
  {
    orora_error("에러, null은 + 연산이 불가함", (void*) 0);
//     printf("에러, null은 + 연산이 불가함\n");
//     exit(1);
  }
  switch (op1 + op2 * 100)
  {
    case AST_VALUE_STRING + AST_VALUE_STRING * 100:
      result->type = AST_VALUE_STRING;
      AST_string* string_v = malloc(sizeof(struct ast_string_t));
      string_v->value_length =
        operand1->value.string_v->value_length +
        operand2->value.string_v->value_length;
      string_v->real_value =
        malloc((string_v->value_length - 3) * sizeof(char));
      string_v->real_value[0] = '\0';

      strcat(string_v->real_value, operand1->value.string_v->real_value);
      strcat(string_v->real_value, operand2->value.string_v->real_value);

      string_v->value =
        malloc((string_v->value_length - 1) * sizeof(char));
      string_v->value[0] = '\0';
      strcat(string_v->value, "\"");
      strcat(string_v->value, string_v->real_value);
      strcat(string_v->value, "\"");
      result->value.string_v = string_v;
      break;

    case AST_VALUE_INT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        operand1->value.int_v->value * operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_INT)
        result->value.float_v->value =
          operand1->value.int_v->value * operand2->value.float_v->value;
      else
        result->value.float_v->value =
          operand1->value.float_v->value * operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      if (op1 == AST_VALUE_INT)
        result->value.int_v->value =
          operand1->value.int_v->value * (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value =
          (operand1->value.bool_v->value ? 1 : 0) * operand2->value.int_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      result->value.float_v->value =
        operand1->value.float_v->value * operand2->value.float_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_FLOAT)
        result->value.int_v->value = 
          operand1->value.float_v->value * (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value = 
          (operand1->value.bool_v->value ? 1 : 0) * operand2->value.float_v->value;
      break;

    case AST_VALUE_BOOL + AST_VALUE_BOOL * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        (operand1->value.bool_v->value ? 1 : 0) 
          * (operand2->value.bool_v->value ? 1 : 0);
      break;

    case AST_VALUE_MATRIX + AST_VALUE_INT * 100:
    case AST_VALUE_INT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_BOOL + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_BOOL * 100:
      AST_matrix* matrix_value = 
        op1 == AST_VALUE_MATRIX ?
          operand1->value.matrix_v
          : operand2->value.matrix_v;
      int matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_product(
            new_value->value.value_v->stack, 
            op1 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand1, 
            op2 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand2
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }
  
  return result;
}

AST_value_stack* visitor_operator_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_MATRIX
      && operand2->type == AST_VALUE_MATRIX)
  {
    return visitor_matrix_product(result, operand1, operand2);
//     AST_matrix* matrix_value = operand1->value.matrix_v;
//     if (
//         matrix_value->row_size != operand2->value.matrix_v->row_size
//         || matrix_value->col_size != operand2->value.matrix_v->col_size
//        )
//     {
//       orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
// //         printf("에러, 행렬 덧셈시 크기가 같아야 함\n");
// //         exit(1);
//     }
// 
//     int matrix_size = 
//       matrix_value->row_size * matrix_value->col_size;
// 
//     result->type = AST_VALUE_MATRIX;
//     result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
//     result->value.matrix_v->row_size = matrix_value->row_size;
//     result->value.matrix_v->col_size = matrix_value->col_size;
// 
//     result->value.matrix_v->value = 
//       malloc(matrix_size * sizeof(struct ast_t));
// 
//     for (int i = 0; i < matrix_size; i ++)
//     {
//       AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
//       new_value->value.value_v = init_ast_value();
//       new_value->value.value_v->size = 1;
//       new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
//       visitor_operator_product(
//           new_value->value.value_v->stack, 
//           matrix_value->value[i]->value.value_v->stack, 
//           operand2->value.matrix_v->value[i]->value.value_v->stack
//         );
//       result->value.matrix_v->value[i] = new_value;
//     }
// 
//     return result;
  }
  return visitor_operator_product_(result, operand1, operand2);
}

AST_value_stack* visitor_operator_dot_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_MATRIX
      && operand2->type == AST_VALUE_MATRIX)
  {
    return visitor_matrix_product(result, operand1, operand2);
  }
  return visitor_operator_product_(result, operand1, operand2);
}

AST_value_stack* visitor_operator_div(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  if (op1 == AST_VALUE_NULL || op2 == AST_VALUE_NULL)
  {
    orora_error("에러, null은 + 연산이 불가함", (void*) 0);
//     printf("에러, null은 + 연산이 불가함\n");
//     exit(1);
  }
  else if (op1 == AST_VALUE_STRING || op2 == AST_VALUE_STRING)
  {
    orora_error("에러, string은 + 연산이 불가함", (void*) 0);
//     printf("에러, string은 + 연산이 불가함\n");
//     exit(1);
  }
  switch (op1 + op2 * 100)
  {
    case AST_VALUE_INT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        operand1->value.int_v->value / operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_INT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_INT)
        result->value.float_v->value =
          operand1->value.int_v->value / operand2->value.float_v->value;
      else
        result->value.float_v->value =
          operand1->value.float_v->value / operand2->value.int_v->value;
      break;

    case AST_VALUE_INT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_INT * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      if (op1 == AST_VALUE_INT)
        result->value.int_v->value =
          operand1->value.int_v->value / (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value =
          (operand1->value.bool_v->value ? 1 : 0) / operand2->value.int_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      result->value.float_v->value =
        operand1->value.float_v->value / operand2->value.float_v->value;
      break;

    case AST_VALUE_FLOAT + AST_VALUE_BOOL * 100:
    case AST_VALUE_BOOL + AST_VALUE_FLOAT * 100:
      result->type = AST_VALUE_FLOAT;
      result->value.float_v = malloc(sizeof(struct ast_float_t));
      if (op1 == AST_VALUE_FLOAT)
        result->value.int_v->value = 
          operand1->value.float_v->value / (operand2->value.bool_v->value ? 1 : 0);
      else
        result->value.int_v->value = 
          (operand1->value.bool_v->value ? 1 : 0) / operand2->value.float_v->value;
      break;

    case AST_VALUE_BOOL + AST_VALUE_BOOL * 100:
      result->type = AST_VALUE_INT;
      result->value.int_v = malloc(sizeof(struct ast_int_t));
      result->value.int_v->value =
        (operand1->value.bool_v->value ? 1 : 0) 
          / (operand2->value.bool_v->value ? 1 : 0);
      break;

    case AST_VALUE_MATRIX + AST_VALUE_MATRIX * 100:
      AST_matrix* matrix_value = operand1->value.matrix_v;
      if (
          matrix_value->row_size != operand2->value.matrix_v->row_size
          || matrix_value->col_size != operand2->value.matrix_v->col_size
         )
      {
        orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
//         printf("에러, 행렬 덧셈시 크기가 같아야 함\n");
//         exit(1);
      }

      int matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_div(
            new_value->value.value_v->stack, 
            matrix_value->value[i]->value.value_v->stack, 
            operand2->value.matrix_v->value[i]->value.value_v->stack
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    case AST_VALUE_MATRIX + AST_VALUE_INT * 100:
    case AST_VALUE_INT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_FLOAT * 100:
    case AST_VALUE_FLOAT + AST_VALUE_MATRIX * 100:
    case AST_VALUE_BOOL + AST_VALUE_MATRIX * 100:
    case AST_VALUE_MATRIX + AST_VALUE_BOOL * 100:
      matrix_value = 
        op1 == AST_VALUE_MATRIX ?
          operand1->value.matrix_v
          : operand2->value.matrix_v;
      matrix_size = 
        matrix_value->row_size * matrix_value->col_size;

      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = matrix_value->row_size;
      result->value.matrix_v->col_size = matrix_value->col_size;

      result->value.matrix_v->value = 
        malloc(matrix_size * sizeof(struct ast_t));

      for (int i = 0; i < matrix_size; i ++)
      {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        visitor_operator_div(
            new_value->value.value_v->stack, 
            op1 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand1, 
            op2 == AST_VALUE_MATRIX ? 
              matrix_value->value[i]->value.value_v->stack
              : operand2
          );
        result->value.matrix_v->value[i] = new_value;
      }
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  return result;
}

AST_value_stack* visitor_operator_power(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  if (op1 == AST_VALUE_MATRIX && operand2->is_T == 1)
  {
    AST_matrix* matrix_value = operand1->value.matrix_v;
    int matrix_size = 
      matrix_value->row_size * matrix_value->col_size;

    result->type = AST_VALUE_MATRIX;
    result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
    result->value.matrix_v->row_size = matrix_value->col_size;
    result->value.matrix_v->col_size = matrix_value->row_size;

    result->value.matrix_v->value = 
      malloc(matrix_size * sizeof(struct ast_t));

    for (int i = 0; i < matrix_size; i ++)
    {
      AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
      new_value->value.value_v = init_ast_value();
      new_value->value.value_v->size = 1;
      new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
      visitor_operator_power(
          new_value->value.value_v->stack, 
          matrix_value->value[i]->value.value_v->stack, 
          operand2
        );
      result->value.matrix_v->value[i] = new_value;
    }

    return result;
  }
  else if (op1 == AST_VALUE_MATRIX && op2 == AST_VALUE_INT
      && operand2->value.int_v->value == -1)
  {
    // inverse matrix
    // ToDo...
    orora_error("에러, 행렬의 역행렬은 아직 지원하지 않음", (void*) 0);
  }
  else if (op1 == AST_VALUE_STRING && op2 == AST_VALUE_INT
      && operand2->value.int_v->value >= 0)
  {
    result->type = AST_VALUE_STRING;
    AST_string* string_v = malloc(sizeof(struct ast_string_t));
    
    int repeat_count = operand2->value.int_v->value;
    size_t original_length = strlen(operand1->value.string_v->real_value);
    string_v->value_length = (original_length * repeat_count) + 3;    

    string_v->real_value = malloc(string_v->value_length - 3);
    string_v->real_value[0] = '\0';
    
    for (int i = 0; i < repeat_count; i++)
      strcat(string_v->real_value, operand1->value.string_v->real_value);
    
    string_v->value = malloc(string_v->value_length);
    string_v->value[0] = '\0';
    
    strcat(string_v->value, "\"");
    strcat(string_v->value, string_v->real_value);
    strcat(string_v->value, "\"");
    
    result->value.string_v = string_v;
    return result;
  }
  if (!(op1 == AST_VALUE_INT || op1 == AST_VALUE_FLOAT)
      || !(op2 == AST_VALUE_INT || op2 == AST_VALUE_FLOAT))
  {
    orora_error("에러, ^연산자는 정수 또는 실수간의 연산 또는 ", (void*) 0);
//     printf("에러, 정수나 실수만 지원함\n");
//     exit(1);
  }
  if (op2 == AST_VALUE_INT && operand2->value.int_v->value == 0)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value = 1;
    return result;
  }
  if (op1 == AST_VALUE_INT && operand2->value.int_v->value == 0)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value = 0;
    return result;
  }
//   if (op1 == AST_VALUE_INT && operand2->value.int_v->value == 1)
//   {
//     result->type = AST_VALUE_INT;
//     result->value.int_v = malloc(sizeof(struct ast_int_t));
//     result->value.int_v->value = 1;
//     return result;
//   }

  if (op1 == AST_VALUE_INT && op2 == AST_VALUE_INT && operand2->value.int_v->value >= 0)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value = (int) pow(operand1->value.int_v->value,
                                            operand2->value.int_v->value);
  }
  else
  {
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    result->value.float_v->value = 
      pow(
          op1 == AST_VALUE_INT ?
            operand1->value.int_v->value
            : operand1->value.float_v->value,
          op2 == AST_VALUE_INT ?
            operand2->value.int_v->value
            : operand2->value.float_v->value
        );
  }

  return result;
}

AST_value_stack* visitor_operator_equal(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  bool result_value = false;

  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      if (operand2->type == AST_VALUE_NULL)
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_STRING:
      if (operand2->type == AST_VALUE_STRING
          && !strcmp(operand1->value.string_v->value,
                     operand2->value.string_v->value))
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_INT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.int_v->value 
              == operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.int_v->value 
              == operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.int_v->value
              == operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_FLOAT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.float_v->value 
              == operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.float_v->value 
              == operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.float_v->value
              == operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_BOOL:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.bool_v->value 
              == operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.bool_v->value 
              == operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.bool_v->value
              == operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_MATRIX:
      if (
          operand2->type == AST_VALUE_MATRIX
          && operand1->value.matrix_v->row_size 
             == operand2->value.matrix_v->row_size
          && operand1->value.matrix_v->col_size 
             == operand2->value.matrix_v->col_size
         )
      {
        int matrix_size = 
          operand1->value.matrix_v->row_size
            * operand1->value.matrix_v->col_size;

        bool is_used = false;
        for (int i = 0; i < matrix_size; i ++)
        {
          AST_value_stack* check_value 
            = malloc(sizeof(AST_value_stack));
          visitor_operator_equal(
              check_value, 
              operand1->value.matrix_v->value[i]->value.value_v->stack, 
              operand2->value.matrix_v->value[i]->value.value_v->stack
            );
          if (!check_value->value.bool_v->value)
          {
            free(check_value);
            is_used = true;
            result_value = false;
            break;
          }
          free(check_value);
        }
        if (!is_used)
          result_value = true;
      }
      else
        result_value = false;
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  result->value.bool_v->value = result_value;

  return result;
}

AST_value_stack* visitor_operator_less(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  bool result_value = false;

  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      // ToDo...
      break;

    case AST_VALUE_STRING:
      // ToDo...
      break;

    case AST_VALUE_INT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.int_v->value 
              < operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.int_v->value 
              < operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.int_v->value
              < operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_FLOAT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.float_v->value 
              < operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.float_v->value 
              < operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.float_v->value
              < operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_BOOL:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.bool_v->value 
              < operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.bool_v->value 
              < operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.bool_v->value
              < operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  result->value.bool_v->value = result_value;

  return result;
}

AST_value_stack* visitor_operator_greater(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  bool result_value = false;

  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      // ToDo...
      break;

    case AST_VALUE_STRING:
      // ToDo...
      break;

    case AST_VALUE_INT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.int_v->value 
              > operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.int_v->value 
              > operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.int_v->value
              > operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_FLOAT:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.float_v->value 
              > operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.float_v->value 
              > operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.float_v->value
              > operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    case AST_VALUE_BOOL:
      if ((operand2->type == AST_VALUE_INT
           && operand1->value.bool_v->value 
              > operand2->value.int_v->value)
          ||
          (operand2->type == AST_VALUE_FLOAT
           && operand1->value.bool_v->value 
              > operand2->value.float_v->value)
          ||
          (operand2->type == AST_VALUE_BOOL
           && operand1->value.bool_v->value
              > operand2->value.bool_v->value)
         )
        result_value = true;
      else
        result_value = false;
      break;

    default:
      orora_error("에러, 정의되지 않은 연산", (void*) 0);
//       printf("에러, 정의되지 않은 연산\n");
//       exit(1);
      break;
  }

  result->value.bool_v->value = result_value;

  return result;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_int
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_INT;
  env_variable->value.int_v = malloc(sizeof(struct ast_int_t));
  env_variable->value.int_v = new_value->value.int_v;

  return env_variable;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_float
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_FLOAT;
  env_variable->value.float_v = malloc(sizeof(struct ast_float_t));
  env_variable->value.float_v = new_value->value.float_v;

  return env_variable;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_string
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_STRING;
  env_variable->value.string_v = malloc(sizeof(struct ast_string_t));
  env_variable->value.string_v = new_value->value.string_v;

  return env_variable;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_null
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_NULL;

  return env_variable;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_bool
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_BOOL;
  env_variable->value.bool_v = malloc(sizeof(struct ast_bool_t));
  env_variable->value.bool_v = new_value->value.bool_v;

  return env_variable;
}

Env_variable* visitor_set_value_Env_variable_from_AST_value_stack_matrix
(Env_variable* env_variable, AST_value_stack* new_value)
{
  if (!env_variable)
    return (void*) 0;

  env_variable->type = ENV_VARIABLE_MATRIX;
  env_variable->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  env_variable->value.matrix_v = new_value->value.matrix_v;

  return env_variable;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_int
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_INT;
  new_value_stack->value.int_v = malloc(sizeof(struct ast_int_t));
  new_value_stack->value.int_v = env_variable->value.int_v;

  return new_value_stack;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_float
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_FLOAT;
  new_value_stack->value.float_v = malloc(sizeof(struct ast_float_t));
  new_value_stack->value.float_v = env_variable->value.float_v;
  
  return new_value_stack;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_string
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_STRING;
  new_value_stack->value.string_v = malloc(sizeof(struct ast_string_t));
  new_value_stack->value.string_v = env_variable->value.string_v;
  
  return new_value_stack;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_null
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_NULL;
  
  return new_value_stack;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_bool
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_BOOL;
  new_value_stack->value.bool_v = malloc(sizeof(struct ast_bool_t));
  new_value_stack->value.bool_v = env_variable->value.bool_v;
  
  return new_value_stack;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_matrix
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_MATRIX;
  new_value_stack->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  new_value_stack->value.matrix_v = env_variable->value.matrix_v;
  
  return new_value_stack;
}

GET_VISITOR_ENV* visitor_run_while(Envs* envs, AST_while* ast_while)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  AST* condition = ast_while->condition;

  while (is_true(visitor_get_value_from_ast(envs, condition)))
  {
    Envs* new_envs = visitor_merge_envs(envs);

    AST* ast_tree = ast_while->code;

    bool is_break = false;
    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
    {
      AST* selected_ast = ast_tree->value.compound_v->items[i];
      bool is_used = true;
      switch (selected_ast->type)
      {
        case AST_BREAK:
          get_visitor_env = init_get_visitor_env();
          is_break = true;
          break;

        case AST_CONTINUE:
          get_visitor_env = init_get_visitor_env();
          is_used = true;
          break;

        case AST_RETURN:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_return = 
            visitor_get_value_from_ast(
                envs, 
                selected_ast->value.return_v->value 
              );
          is_break = true;
          break;

        default:
          is_used = false;
          break;
      }
      if (is_break)
        break;

      if (is_used)
        break;

      free(get_visitor_env);
      get_visitor_env = 
        visitor_visit(
            new_envs, 
            selected_ast 
          );

      if (get_visitor_env->is_break
          || get_visitor_env->is_return
         )
      {
        get_visitor_env->is_break = false;
        is_break = true;
        break;
      }
      if (get_visitor_env->is_continue)
      {
        get_visitor_env->is_continue = false;
        break;
      }
    }
    free(new_envs);
    if (is_break)
      break;
  }
  
  return get_visitor_env;
}

GET_VISITOR_ENV* visitor_run_for(Envs* envs, AST_for* ast_for)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  AST* init = ast_for->init;
  AST* condition = ast_for->condition;
  AST* update = ast_for->update;

  Envs* new_envs = visitor_merge_envs(envs);
  visitor_visit(
      new_envs,
      init
  );

  while (is_true(visitor_get_value_from_ast(new_envs, condition)))
  {
    AST* ast_tree = ast_for->code;

    bool is_break = false;
    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
    {
      AST* selected_ast = ast_tree->value.compound_v->items[i];
      bool is_used = true;
      switch (selected_ast->type)
      {
        case AST_BREAK:
          get_visitor_env = init_get_visitor_env();
          is_break = true;
          break;

        case AST_CONTINUE:
          get_visitor_env = init_get_visitor_env();
          is_used = true;
          break;

        case AST_RETURN:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_return = 
            visitor_get_value_from_ast(
                envs, 
                selected_ast->value.return_v->value 
              );
          is_break = true;
          break;

        default:
          is_used = false;
          break;
      }
      if (is_break)
        break;

      if (is_used)
        break;

      free(get_visitor_env);
      get_visitor_env = 
        visitor_visit(
            new_envs, 
            selected_ast 
          );

      if (get_visitor_env->is_break
          || get_visitor_env->is_return
         )
      {
        get_visitor_env->is_break = false;
        is_break = true;
        break;
      }
      if (get_visitor_env->is_continue)
      {
        get_visitor_env->is_continue = false;
        break;
      }
    }

    visitor_visit(
        new_envs,
        update
    );

    if (is_break)
      break;
  }
  free(new_envs);

  return get_visitor_env;
}

GET_VISITOR_ENV* visitor_run_if(Envs* envs, AST_if* ast_if)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  AST* condition = ast_if->condition;

  AST_value_stack* check_condition = 
    visitor_get_value_from_ast(envs, condition);
  bool is_condition = is_true(check_condition);
  free(check_condition);
  if (is_condition)
  {
    Envs* new_envs = visitor_merge_envs(envs);

    AST* ast_tree = ast_if->code;
    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
    {
      AST* selected_ast = ast_tree->value.compound_v->items[i];

      bool is_used = true;
      switch (selected_ast->type)
      {
        case AST_BREAK:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_break = true;
          break;

        case AST_CONTINUE:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_continue = true;
          break;

        case AST_RETURN:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_return = 
            visitor_get_value_from_ast(
                envs, 
                selected_ast->value.return_v->value 
              );
          break;

        default:
          is_used = false;
          break;
      }
      
      if (is_used)
        break;

      free(get_visitor_env);
      get_visitor_env = 
        visitor_visit(
            new_envs, 
            selected_ast 
          );

      if (get_visitor_env->is_break
          || get_visitor_env->is_continue
          || get_visitor_env->is_return
         )
        break;
    }
    free(new_envs);
  }

  return get_visitor_env;
}

GET_VISITOR_ENV* visitor_run_code(Envs* envs, AST_compound* ast_code)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  Envs* new_envs = visitor_merge_envs(envs);

  get_visitor_env->is_return = visitor_get_value_from_code(
                                  new_envs, 
                                  ast_code
                                );
  free(new_envs);

  return get_visitor_env;
}

GET_VISITOR_ENV* visitor_run_cases(Envs* envs, AST_cases* ast_cases)
{
  GET_VISITOR_ENV* get_visitor_env = 
    init_get_visitor_env();

  Envs* new_envs = visitor_merge_envs(envs);

  get_visitor_env->is_return = visitor_get_value_from_cases(
                                  new_envs,
                                  ast_cases
                               );
  free(new_envs);

  return get_visitor_env;
}
