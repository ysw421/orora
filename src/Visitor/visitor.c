#include <string.h>
#include "visitor.h"
#include "../main.h"
#include "../Parser/parser_value.h"

#ifdef DEVELOP_MODE
#include <stdlib.h>
#include <stdio.h>
#endif

orora_value_type* get_single_value_type(int ast_type);

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_get_function(Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable
(Envs* envs, Env_variable* env_variable, AST_variable* ast_variable);

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

AST_value_stack* get_variable_from_Env_variable
(Envs* envs, AST_value_stack* ast);
Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable);

void visitor_nondefine_variable_error(AST_variable* ast_variable);
void visitor_nondefine_function_error(AST_function* ast_function);

bool is_true(AST_value_stack* value);
AST_value_stack* get_deep_copy_ast_value_stack
(AST_value_stack* ast_value_stack);
bool visitor_check_satisfy(Envs* envs, Env_variable* env_variable);
Envs* visitor_merge_envs(Envs* envs);

AST_value_stack* visitor_get_value_from_function
(Envs* envs, AST_function* ast_function, Env_function* env_function);
AST_value_stack* visitor_get_value_from_variable
(Envs* envs, Env_variable* env_variable);

#ifdef DEVELOP_MODE
void visitor_print_function(Envs* envs, AST* ast)
{
  AST_function* ast_function = ast->value.function_v;

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
          else
          {
            AST_value_stack* new_value =
              visitor_get_value(envs, ast_function_arg_value);
            switch (new_value->type)
            {
              case AST_VALUE_STRING:
                printf("%s",
                    new_value->value.string_v->real_value);
                break;
              case AST_VALUE_INT:
                printf("%d",
                    new_value->value.int_v->value);
                break;
              case AST_VALUE_FLOAT:
                printf("%f",
                    new_value->value.float_v->value);
                break;
            }
          }
          break;

        case AST_VARIABLE:
          AST_variable* ast_function_arg_variable =
            ast_function_arg->value.variable_v;
          Env_variable* variable =
            visitor_get_variable(envs, ast_function_arg_variable);
          if (!variable)
          {
            visitor_nondefine_variable_error(ast_function_arg_variable);
          }

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
}
#endif

void visitor_visit(Envs* envs, AST* ast)
{
  switch (ast->type)
  {
    case AST_VARIABLE:
      AST_variable* ast_variable = ast->value.variable_v;
      Env_variable* env_variable;
      visitor_variable(envs, env_variable, ast_variable);
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
          break;

        case AST_FUNCTION_DEFINE:
          visitor_function_define(envs, ast_function);
          break;

        default:
#ifdef DEVELOP_MODE
          printf("내가 ast function 설정 잘못함...");
          exit(1);
#endif
          break;
      }
      break;
  }
}

Env_variable* visitor_variable
(Envs* envs, Env_variable* env_variable, AST_variable* ast_variable)
{
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
      printf("내가 ast variable 설정 잘못함...");
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
  while (check_function->next)
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
      snode = snode->next;
    else
      snode = envs->local->functions;
    check_function = check_function->next;
  }

  check_function = envs->global->functions;
  snode = (void*) 0;
  while (check_function->next)
  {
    if (!strcmp(check_function->name, ast_function->name))
    {
      if (snode)
      {
        snode->next = check_function->next;
        check_function->next = envs->global->functions;
        envs->global->functions = check_function;
      }
      return check_function;
    }
    if (snode)
      snode = snode->next;
    else
      snode = envs->global->functions;
    check_function = check_function->next;
  }

  return (void*) 0;
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
  Envs* new_envs = visitor_merge_envs(envs);

  // Get value from arguments...
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
              printf("에러, 변수에는 값을 저장해야함\n");
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
          // ToDo
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

  printf("!!! %s %ld\n", env_function->name, new_envs->local->variable_size);
  return (void*) 0;
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
  
  if (check_variable)
  {
    while (check_variable->next && check_variable->name)
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
      check_variable = check_variable->next;
    }
  }

  check_variable = envs->global->variables;
  
  if (check_variable)
  {
    snode = (void*) 0;
    while (check_variable->next && check_variable->name)
    {
      if (!strcmp(check_variable->name, ast_variable->name))
      {
        if (snode)
        {
          snode->next = check_variable->next;
          check_variable->next = envs->global->variables;
          envs->global->variables = check_variable;
        }
        return check_variable;
      }
      if (snode)
        snode = snode->next;
      else
      {
        snode = envs->global->variables;
      }
      check_variable = check_variable->next;
    }
  }

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

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable)
{
  bool is_defined_variable;
  switch (ast_variable->value->type)
  {
    case AST_VALUE:
      if (ast_variable->value->value.value_v->size == 1)
      {
        orora_value_type* variable_type =
          get_single_value_type(ast_variable->value->value.
              value_v->stack->type);
        if (variable_type)
        {
          Env_variable* env_variable =
            visitor_get_variable(envs, ast_variable);
          
          if (env_variable)
          {
            env_variable =
              variable_type->visitor_set_value_Env_variable_from_AST_value_stack(
                    env_variable,
                    ast_variable->value->value.value_v->stack
                  );
          }
          else
          {
            env_variable = init_env_variable(ast_variable->name,
                                              ast_variable->name_length);
            env_variable =
              variable_type->visitor_set_value_Env_variable_from_AST_value_stack(
                    env_variable,
                    ast_variable->value->value.value_v->stack
                  );

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
          visitor_get_value(envs, ast_variable->value->value.value_v);

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
      break;
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

    case AST_FUNCTION:
      // ToDo
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
  AST_value_stack* new_value_stack = malloc(sizeof(AST_value));
  new_value_stack->type = ast_value_stack->type;
  new_value_stack->value = ast_value_stack->value;
  new_value_stack->next = ast_value_stack->next;

  new_value_stack->col = ast_value_stack->col;
  new_value_stack->col_first = ast_value_stack->col_first;
  new_value_stack->row = ast_value_stack->row;
  new_value_stack->row_char = ast_value_stack->row_char;
  new_value_stack->row_char_first = ast_value_stack->row_char_first;

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
//     text_array[i] = get_deep_copy_ast_value_stack(p);
    text_array[i] = p;
    p = p->next;
  }

  int i = 0;
  while (i < max_cnt)
  {
    text = text_array[i];
    i ++;
    if (parser_precedence(text->type) == -1)
    {
      switch (text->type)
      {
        case AST_VALUE_VARIABLE:
          Env_variable* env_variable =
            visitor_get_variable(envs, text->value.variable_v);
          if (!env_variable)
          {
            visitor_nondefine_variable_error(text->value.variable_v);
          }

          AST_value_stack* new_value_stack =
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

          visitor_get_value_from_function(envs, ast_function, env_function);
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
      }

      parser_push_value(stack, result);
    }
  }

  if (stack->size != 1)
  {
    printf("에러, 연산 불가함\n");
    exit(1);
  }

  return parser_pop_value(stack);
}

Envs* visitor_merge_envs(Envs* envs)
{
  Env* global = envs->global;
  Env* local = envs->local;

  Env* new_global_env = init_env();

  new_global_env->variable_size =
    global->variable_size + local->variable_size;
  new_global_env->variables = malloc(sizeof(struct env_variable_t));
  // Warning! possibility error...
  new_global_env->variables = local->variables;
  
  Env_variable* p_v = new_global_env->variables;
  while (p_v->next)
    p_v = p_v->next;
  p_v->next = global->variables;

  new_global_env->function_size =
    global->function_size + local->function_size;
  new_global_env->functions = malloc(sizeof(struct env_function_t));
  new_global_env->functions = local->functions;

  Env_function* p_f = new_global_env->functions;
  while (p_f->next)
    p_f = p_f->next;
  p_f->next = global->functions;
  
  Envs* new_envs = init_envs(new_global_env, init_env());

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
  if (operand1->type == AST_VALUE_STRING
      || operand2->type == AST_VALUE_STRING)
  {
    printf("에러, string은 + 연산이 불가함\n");
    exit(1);
  }
  else if (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_INT)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value =
      operand1->value.int_v->value + operand2->value.int_v->value;
  }
  else if ((operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_INT)
      || (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_FLOAT)
      || (operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_FLOAT))
  {
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    if (operand1->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.int_v->value + operand2->value.float_v->value;
    else if (operand2->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.float_v->value + operand2->value.int_v->value;
    else
      result->value.float_v->value =
        operand1->value.float_v->value + operand2->value.float_v->value;
  }
  else
  {
    printf("에러, 정의되지 않은 연산\n");
    exit(1);
  }

  return result;
}

AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_STRING
      || operand2->type == AST_VALUE_STRING)
  {
    printf("에러, string은 - 연산이 불가함\n");
    exit(1);
  }
  else if (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_INT)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value =
      operand1->value.int_v->value - operand2->value.int_v->value;
  }
  else if ((operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_INT)
      || (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_FLOAT)
      || (operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_FLOAT))
  {
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    if (operand1->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.int_v->value - operand2->value.float_v->value;
    else if (operand2->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.float_v->value - operand2->value.int_v->value;
    else
      result->value.float_v->value =
        operand1->value.float_v->value - operand2->value.float_v->value;
  }
  else
  {
    printf("에러, 정의되지 않은 연산\n");
    exit(1);
  }

  return result;
}

AST_value_stack* visitor_operator_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_STRING
      || operand2->type == AST_VALUE_STRING)
  {
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
  }
  else if (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_INT)
  {
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value =
      operand1->value.int_v->value * operand2->value.int_v->value;
  }
  else if ((operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_INT)
      || (operand1->type == AST_VALUE_INT
      && operand2->type == AST_VALUE_FLOAT)
      || (operand1->type == AST_VALUE_FLOAT
      && operand2->type == AST_VALUE_FLOAT))
  {
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    if (operand1->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.int_v->value * operand2->value.float_v->value;
    else if (operand2->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.float_v->value * operand2->value.int_v->value;
    else
      result->value.float_v->value =
        operand1->value.float_v->value * operand2->value.float_v->value;
  }
  else
  {
    printf("에러, 정의되지 않은 연산\n");
    exit(1);
  }

  return result;
}

AST_value_stack* visitor_operator_div(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_STRING
      || operand2->type == AST_VALUE_STRING)
  {
    printf("에러, string은 - 연산이 불가함\n");
    exit(1);
  }
  else if ((operand1->type == AST_VALUE_INT
      || operand1->type == AST_VALUE_FLOAT)
      && (operand2->type == AST_VALUE_INT
      || operand2->type == AST_VALUE_FLOAT))
  {
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    if (operand1->type == AST_VALUE_INT
        && operand2->type == AST_VALUE_INT)
      result->value.float_v->value =
        (float)operand1->value.int_v->value
          / (float)operand2->value.int_v->value;
    else if (operand1->type == AST_VALUE_INT)
      result->value.float_v->value =
        (float)operand1->value.int_v->value / operand2->value.float_v->value;
    else if (operand2->type == AST_VALUE_INT)
      result->value.float_v->value =
        operand1->value.float_v->value / (float)operand2->value.int_v->value;
    else
      result->value.float_v->value =
        operand1->value.float_v->value / operand2->value.float_v->value;
  }
  else
  {
    printf("에러, 정의되지 않은 연산\n");
    exit(1);
  }

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

