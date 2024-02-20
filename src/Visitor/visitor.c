#include <string.h>
#include "visitor.h"
#include "../main.h"
#include "../Parser/parser_value.h"

#ifdef DEVELOP_MODE
#include <stdlib.h>
#include <stdio.h>

void visitor_print_function(Envs* envs, AST* ast);
#endif

GET_VISITOR_ENV* init_get_visitor_env();
orora_value_type* get_single_value_type(int ast_type);

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_get_function(Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable
(Envs* envs, AST_variable* ast_variable);

AST_value_stack* visitor_function_value
(Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_function_define(Envs* envs, AST_function* ast_function);

AST_value_stack* visitor_get_value(Envs* envs, AST_value* ast_value);

AST_value_stack* visitor_operator_plus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_product(AST_value_stack* result,
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
(Envs* envs, AST_function* ast_function, Env_function* env_function);
AST_value_stack* visitor_get_value_from_variable
(Envs* envs, Env_variable* env_variable);
AST_value_stack* visitor_get_value_from_cases
(Envs* envs, AST_cases* ast_cases);

GET_VISITOR_ENV* visitor_run_while(Envs* envs, AST_while* ast_while);
GET_VISITOR_ENV* visitor_run_if(Envs* envs, AST_if* ast_if);

AST_value_stack* visitor_get_value_from_code
(Envs* envs, AST_compound* ast_code);

GET_VISITOR_ENV* init_get_visitor_env()
{
  GET_VISITOR_ENV* new_env = 
    (GET_VISITOR_ENV*)malloc(sizeof(struct get_visitor_env_t));

  new_env->is_break = false;
  new_env->is_continue = false;
  new_env->is_return = (void*) 0;

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
      Env_variable* env_variable =
        visitor_variable(envs, ast_variable);
      break;

    case AST_FUNCTION:
#ifdef DEVELOP_MODE
      visitor_print_function(envs, ast);
#endif

      AST_function* ast_function = ast->value.function_v;
      Env_function* env_function;
      switch (ast_function->ast_type)
      {
        case AST_FUNCTION_VALUE:
#ifdef DEVELOP_MODE
          if (strcmp(ast_function->name, "print"))
          {
#endif
          visitor_function_value(envs, ast_function);
#ifdef DEVELOP_MODE
          }
#endif
          break;

        case AST_FUNCTION_DEFINE:
          visitor_function_define(envs, ast_function);
          break;

        default:
#ifdef DEVELOP_MODE
          printf("내가 ast function 설정 잘못함...\n");
          exit(1);
#endif
          break;
      }
      break;

    case AST_WHILE:
      AST_while* ast_while = ast->value.while_v;
      get_visitor_env = 
        visitor_run_while(envs, ast_while);
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
  }

  return get_visitor_env;
}

AST_value_stack* visitor_function_value
(Envs* envs, AST_function* ast_function)
{
  Env_function* env_function =
    visitor_get_function(envs, ast_function);
  if (!env_function)
  {
    visitor_nondefine_function_error(ast_function);
  }

  Envs* new_envs =
    visitor_get_envs_from_function(envs, ast_function, env_function);

  AST_value_stack* result = 
    visitor_get_value_from_ast(new_envs, env_function->code);

  free(new_envs);
  
  return result;
}

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
  }

  return true;
}

void visitor_nondefine_variable_error(AST_variable* ast_variable)
{
  printf("에러, 정의되지 않은 변수: %s\n", ast_variable->name);
  exit(1);
}

void visitor_nondefine_function_error(AST_function* ast_function)
{
  printf("에러, 정의되지 않은 함수: %s\n", ast_function->name);
  exit(1);
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
      Env_function* env_function =
        visitor_get_function(envs, condition->value.function_v);
      if (!env_function)
      {
        visitor_nondefine_function_error(
             condition->value.function_v
        );
      }

      condition_value = 
        visitor_get_value_from_function(
            envs,
            condition->value.function_v,
            env_function
        );
      break;

    default:
      printf("에러, while문의 조건이 잘못됨: %d\n", condition->type);
      exit(1);
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
    switch (selected_ast->type)
    {
      case AST_RETURN:
        return visitor_get_value_from_ast(
                  envs, 
                  selected_ast->value.return_v->value 
                );
        break;
    }

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
(Envs* envs, AST_function* ast_function, Env_function* env_function)
{
  if (!env_function)
  {
    visitor_nondefine_function_error(ast_function);
  }
  AST_value_stack* new_value_stack;

  // Set New Environment: new_envs
  Envs* new_envs =
    visitor_get_envs_from_function(envs, ast_function, env_function);

  return visitor_function_value(envs, ast_function);
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
    printf("에러, 변수에 저장하는 것은 값이어야함\n");
    exit(1);
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
      printf("에러, 변수에는 값을 저장해야함\n");
      exit(1);
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
      printf("에러, 변수에 저장하는 것은 값이어야함\n");
      exit(1);
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

  size_t s_satisfy_size = get_variable->satisfy_size;
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
//   if (!ast_value_stack
//       || !ast_value_stack->type
//       || !ast_value_stack->next
//      )
//     return (void*) 0;

  AST_value_stack* new_value_stack = malloc(sizeof(AST_value));
  if (ast_value_stack->type)
    new_value_stack->type = ast_value_stack->type;
  new_value_stack->value = ast_value_stack->value;
  if (ast_value_stack->next)
    new_value_stack->next = ast_value_stack->next;
  else
    new_value_stack->next = (void*) 0;

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

          parser_push_value(stack, new_value_stack);
          break;

        case AST_VALUE_FUNCTION:
          AST_function* ast_function = text->value.function_v;
          Env_function* env_function =
            visitor_get_function(envs, text->value.function_v);
          if (!env_function)
          {
            visitor_nondefine_function_error(text->value.function_v);
          }

          new_value_stack =
            visitor_get_value_from_function(envs, ast_function, env_function);

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

        case AST_VALUE_DIV:
          result = visitor_operator_div(result, operand1, operand2);
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
      }

      parser_push_value(stack, result);
      free(text);
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
    printf("에러, 연산 불가함\n");
    exit(1);
  }

  free(text_array);

  return parser_pop_value(stack);
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
  Env* new_global_env = init_env();

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
    printf("에러, 함수의 매개변수 개수가 다름\n");
    exit(1);
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

      switch (ast->type)
      {
        case AST_VALUE:
          if (ast->value.value_v->size == 1)
          {
            orora_value_type* variable_type =
              get_single_value_type(ast->value.value_v->stack->type);

            if (variable_type)
            {
              env_variable =
                variable_type
                  ->visitor_set_value_Env_variable_from_AST_value_stack(
                      env_variable,
                      ast->value.value_v->stack
                    );
            }
            else
            {
              printf("에러, 변수에는 값을 저장해야함1\n");
              exit(1);
            }
          }
          else
          {
            AST_value_stack* new_value =
              visitor_get_value(new_envs, ast->value.value_v);

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
              printf("에러, 변수에 저장하는 것은 값이어야함\n");
              exit(1);
            }
          }
          break;

        case AST_VARIABLE:
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
          }

          break;

        case AST_FUNCTION:
          Env_function* env_function =
            visitor_get_function(envs, ast->value.function_v);
          if (!env_function)
          {
            visitor_nondefine_function_error(
                ast_variable->value->value.function_v
            );
          }
          AST_value_stack* ast_value_stack
            = visitor_get_value_from_function(
                  envs,
                  ast->value.function_v,
                  env_function
                );
          orora_value_type* variable_type =
            get_single_value_type(ast_value_stack->type);

          if (variable_type)
          {
            env_variable =
              variable_type
                ->visitor_set_value_Env_variable_from_AST_value_stack(
                    env_variable,
                    ast_value_stack
                  );
          }
          else
          {
            printf("에러, 변수에는 값을 저장해야함1\n");
            exit(1);
          }
          break;
      }

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
        printf("에러, 함수의 매개변수 개수가 다름\n");
        exit(1);
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
    printf("에러, 변수에 저장하는 것은 값이어야함\n");
    exit(1);
  }

  return new_value_stack;
}


AST_value_stack* visitor_operator_plus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      printf("에러, null은 + 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_STRING:
      printf("에러, string은 + 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_INT:
      int int_value = operand1->value.int_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value + operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            int_value + operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value + (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_FLOAT:
      float float_value = operand1->value.float_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value + operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value + operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.int_v->value =
            float_value + (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_BOOL:
      int bool_value = operand1->value.bool_v->value ? 1 : 0;
      switch (operand2->type)
      {
        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            bool_value + (operand2->value.bool_v->value ? 1 : 0);
          break;

        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value = 
            bool_value + operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value = 
            bool_value + operand2->value.float_v->value;
          break;

        default:
          // ToDo...
          break;
      }
      break;

    default:
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
      break;
  }

  return result;
}

AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      printf("에러, null은 - 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_STRING:
      printf("에러, string은 - 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_INT:
      int int_value = operand1->value.int_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value - operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            int_value - operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value - (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_FLOAT:
      float float_value = operand1->value.float_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value - operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value - operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.int_v->value =
            float_value - (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_BOOL:
      int bool_value = operand1->value.bool_v->value ? 1 : 0;
      switch (operand2->type)
      {
        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            bool_value - (operand2->value.bool_v->value ? 1 : 0);
          break;

        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value = 
            bool_value - operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value = 
            bool_value - operand2->value.float_v->value;
          break;

        default:
          // ToDo...
          break;
      }
      break;

    default:
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
      break;
  }
    
  return result;
}

AST_value_stack* visitor_operator_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      printf("에러, null은 + 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_STRING:
      switch (operand2->type)
      {
        case AST_VALUE_STRING:
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

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_INT:
      int int_value = operand1->value.int_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value * operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            int_value * operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value * (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_FLOAT:
      float float_value = operand1->value.float_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value * operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value * operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.int_v->value =
            float_value * (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_BOOL:
      int bool_value = operand1->value.bool_v->value ? 1 : 0;
      switch (operand2->type)
      {
        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            bool_value * (operand2->value.bool_v->value ? 1 : 0);
          break;

        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value = 
            bool_value * operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value = 
            bool_value * operand2->value.float_v->value;
          break;

        default:
          // ToDo...
          break;
      }
      break;

    default:
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
      break;
  }
  
  return result;
}

AST_value_stack* visitor_operator_div(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  switch (operand1->type)
  {
    case AST_VALUE_NULL:
      printf("에러, null은 - 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_STRING:
      printf("에러, string은 - 연산이 불가함\n");
      exit(1);
      break;

    case AST_VALUE_INT:
      int int_value = operand1->value.int_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value / operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            int_value / operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            int_value / (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_FLOAT:
      float float_value = operand1->value.float_v->value;
      switch (operand2->type)
      {
        case AST_VALUE_INT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value / operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value =
            float_value / operand2->value.float_v->value;
          break;

        case AST_VALUE_BOOL:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.int_v->value =
            float_value / (operand2->value.bool_v->value ? 1 : 0);
          break;

        default:
          // ToDo...
          break;
      }
      break;

    case AST_VALUE_BOOL:
      int bool_value = operand1->value.bool_v->value ? 1 : 0;
      switch (operand2->type)
      {
        case AST_VALUE_BOOL:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value =
            bool_value / (operand2->value.bool_v->value ? 1 : 0);
          break;

        case AST_VALUE_INT:
          result->type = AST_VALUE_INT;
          result->value.int_v = malloc(sizeof(struct ast_int_t));
          result->value.int_v->value = 
            bool_value / operand2->value.int_v->value;
          break;

        case AST_VALUE_FLOAT:
          result->type = AST_VALUE_FLOAT;
          result->value.float_v = malloc(sizeof(struct ast_float_t));
          result->value.float_v->value = 
            bool_value / operand2->value.float_v->value;
          break;

        default:
          // ToDo...
          break;
      }
      break;

    default:
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
      break;
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

    default:
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
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
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
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
      printf("에러, 정의되지 않은 연산\n");
      exit(1);
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
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_float
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_FLOAT;
  new_value_stack->value.float_v = malloc(sizeof(struct ast_float_t));
  new_value_stack->value.float_v = env_variable->value.float_v;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_string
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_STRING;
  new_value_stack->value.string_v = malloc(sizeof(struct ast_string_t));
  new_value_stack->value.string_v = env_variable->value.string_v;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_null
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_NULL;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_bool
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_BOOL;
  new_value_stack->value.bool_v = malloc(sizeof(struct ast_bool_t));
  new_value_stack->value.bool_v = env_variable->value.bool_v;
}

AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable_matrix
(AST_value_stack* new_value_stack, Env_variable* env_variable)
{
  if (!new_value_stack)
    return (void*) 0;

  new_value_stack->type = AST_VALUE_MATRIX;
  new_value_stack->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  new_value_stack->value.matrix_v = env_variable->value.matrix_v;
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
      bool is_used = false;
      switch (selected_ast->type)
      {
        case AST_BREAK:
          get_visitor_env = init_get_visitor_env();
          is_used = true;
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
          is_used = true;
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
      bool is_used = false;
      switch (selected_ast->type)
      {
        case AST_BREAK:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_break = true;
          is_used = true;
          break;

        case AST_CONTINUE:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_continue = true;
          is_used = true;
          break;

        case AST_RETURN:
          get_visitor_env = init_get_visitor_env();
          get_visitor_env->is_return = 
            visitor_get_value_from_ast(
                envs, 
                selected_ast->value.return_v->value 
              );
          is_used = true;
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

