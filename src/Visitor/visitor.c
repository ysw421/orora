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
Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable);
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
AST_value_stack* get_variable_from_Env_variable(Envs* envs, AST_value_stack* ast);
Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable);

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
          visitor_variable_define(envs, ast_variable);
          break;

        case AST_VARIABLE_SATISFY:
          visitor_variable_satisfy(envs, ast_variable);
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

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* check_variable = envs->local->variables;
  Env_variable* snode = (void*) 0;
  while (check_variable->next)
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

  // ToDo:
  //    get variable from global

  printf("에러, 정의되지 않은 변수: %s\n", ast_variable->name);
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

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable)
{
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
            init_env_variable(ast_variable->name,
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
          init_env_variable(ast_variable->name,
              ast_variable->name_length);

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

        Env* local_env = envs->local;
        env_variable->next = local_env->variables;
        local_env->variable_size ++;
        local_env->variables = env_variable;

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
          env_variable =
            init_env_variable(ast_variable->name,
                  ast_variable->name_length);
          value_variable =
            visitor_get_variable(envs,
                ast_variable->value->value.variable_v);
          env_variable->type = value_variable->type;
          env_variable->value = value_variable->value;

          env_variable->next = local_env->variables;
          local_env->variable_size ++;
          local_env->variables = env_variable;

          return env_variable;
          break;

        case AST_VARIABLE_DEFINE:
          env_variable =
            init_env_variable(ast_variable->name,
                  ast_variable->name_length);
          value_variable =
            visitor_variable_define(envs,
                ast_variable->value->value.variable_v);
          env_variable->type = value_variable->type;
          env_variable->value = value_variable->value;

          env_variable->next = local_env->variables;
          local_env->variable_size ++;
          local_env->variables = env_variable;

          return env_variable;
          break;
      }
      break;
  }
  return (void*) 0;
}

Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable)
{
  Env_variable* get_variable =
    visitor_get_variable(envs, ast_variable);
  size_t s_satisfy_size = get_variable->satisfy_size;
  get_variable->satisfy_size += ast_variable->satisfy_size;
  get_variable->satisfy =
    realloc(get_variable->satisfy,
        get_variable->satisfy_size * sizeof(struct ast_variable_t*));
  for (size_t i = 0; i < ast_variable->satisfy_size; i ++)
  {
    get_variable->satisfy[s_satisfy_size + i] =
      ast_variable->satisfy[i]->value.value_v;
  }
  return (void*) 0;
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
      if (text->type == AST_VALUE_VARIABLE)
      {
        Env_variable* env_variable =
          visitor_get_variable(envs, text->value.variable_v);

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
        parser_push_value(stack, new_value_stack);
      }
      else
        parser_push_value(stack, text);
    }
    else
    {
      AST_value_stack* operand1;
      AST_value_stack* operand2;
      operand2 = parser_pop_value(stack);

      if (true)   // ToDo... if operator is !
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

AST_value_stack* get_variable_from_Env_variable(Envs* envs, AST_value_stack* ast)
{
  Env_variable* env_variable =
    visitor_get_variable(envs, ast->value.variable_v);

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
        (float)operand1->value.int_v->value / (float)operand2->value.int_v->value;
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

