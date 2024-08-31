#include <string.h>
#include <math.h>
#include "visitor/visitor.h"
#include "loader/main.h"
#include "parser/parser_value.h"
#include "loader/error_log.h"
#include "utilities/utils.h"

#include <stdlib.h>
#include <stdio.h>
#include "syslib/sysfunc.h"

GET_VISITOR_ENV* init_get_visitor_env();
orora_value_type* get_single_value_type(int ast_type);

Env_variable* visitor_get_variable(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_get_function(Envs* envs, AST_function* ast_function);
Env_macro* visitor_get_macro(Envs* envs, AST_macro* ast_macro);
Env_macro* visitor_get_macro_newenv(Envs* envs, const char* name, AST_newenv* ast_macro);

Env_variable* visitor_variable
(Envs* envs, AST_variable* ast_variable);

// AST_value_stack* visitor_function_value
// (Envs* envs, AST_function* ast_function);

Env_variable* visitor_variable_define(Envs* envs, AST_variable* ast_variable);
Env_function* visitor_function_define(Envs* envs, AST_function* ast_function);
Env_macro* visitor_macro_define(Envs* envs, AST_newenv* ast_newenv);

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
AST_value_stack* visitor_operator_mod(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_colon(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_comma(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);
AST_value_stack* visitor_operator_under(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs);
AST_value_stack* visitor_operator_in(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2);

AST_value_stack* get_variable_from_Env_variable
(Envs* envs, AST_value_stack* ast);
Env_variable* visitor_variable_satisfy(Envs* envs, AST_variable* ast_variable);

void visitor_env_variable_autotype(Envs* new_envs, Env_variable* env_variable, AST* ast);


struct AST_VALUE_STACK_INDEX_T
{
  AST_value_stack* value;
  AST** index;
  size_t index_size;
};

struct AST_VALUE_STACK_INDEX_T* visitor_operator_under_for_index(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs);

struct AST_VALUE_STACK_INDEX_T* visitor_get_matrix_index(Envs* envs, AST_value* ast_value);


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
AST_value_stack* visitor_get_value_from_macro
(Envs* envs, AST_macro* ast_macro);
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
    case AST_NEWENV:
      visitor_macro_define(envs, ast->value.newenv_v);
      break;

    case AST_MATRIX_INDEX:
      AST_matrix_index* ast_matrix_index = ast->value.matrix_index_v;

      struct AST_VALUE_STACK_INDEX_T* index = visitor_get_matrix_index(envs, ast_matrix_index->index->value.value_v);

      size_t index_size = index->index_size;
      if (index_size == 1)
      {
        index->index[0]->value.value_v->stack = visitor_get_value_from_ast(envs, ast_matrix_index->value);
      }
      else
      {
        AST_value_stack* value_ast = visitor_get_value_from_ast(envs, ast_matrix_index->value);
        if (value_ast->type != AST_VALUE_MATRIX)
        {
          orora_error("에러, 대입값은 (1, n) 행렬이어야 함..", (void*) 0);
        }
        AST_matrix* value_matrix = value_ast->value.matrix_v;
        if (!(value_matrix->col_size == 1 && value_matrix->row_size == index_size))
        {
          orora_error("에러, 대입값은 (1, n) 행렬이어야 함.", (void*) 0);
        }
        for (int i = 0; i < index_size; i++)
        {
          index->index[i]->value.value_v->stack = visitor_get_value_from_ast(envs, value_matrix->value[i]);
        }
      }

      break;

    case AST_VARIABLE:
      AST_variable* ast_variable = ast->value.variable_v;
      Env_variable* env_variable = visitor_variable(envs, ast_variable);

      if (INTERACTIVE_MODE && ast_variable->ast_type == AST_VARIABLE_VALUE)
        get_visitor_env->output = 
          visitor_get_value_from_variable(envs, env_variable);
      else
        visitor_get_value_from_variable(envs, env_variable);
      break;

    case AST_MACRO:
      if (INTERACTIVE_MODE)
        get_visitor_env->output =
          visitor_get_value_from_macro(envs, ast->value.macro_v);
      else
        visitor_get_value_from_macro(envs, ast->value.macro_v);
      break;

    case AST_FUNCTION:
      AST_value_stack* output = check_sysfunc(envs, ast->value.function_v);

      if (output)
      {
        get_visitor_env->output = output;
        break;
      }
      else
      {
        free(output);
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
      orora_error("에러, satisfy 조건에 만족하지 아니함\n", (void*) 0);
//       printf("에러, satisfy 조건에 만족하지 아니함\n");
//       exit(1);
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

void visitor_nondefine_macro_error(AST_macro* ast_macro)
{
  const char* error_log = "에러 정의되지 않은 macro: ";
  error_log = const_strcat(error_log, ast_macro->name);
  orora_error(error_log, (void*) 0);
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

Env_macro* visitor_get_macro(Envs* envs, AST_macro* ast_macro)
{
  Env_macro* check_macro = envs->local->macros;

  Env_macro* snode = (void*) 0;
  while (check_macro)
  {
    if (!strcmp(check_macro->name, ast_macro->name))
    {
      if (snode)
      {
        snode->next = check_macro->next;
        check_macro->next = envs->local->macros;
        envs->local->macros = check_macro;
      }
      return check_macro;
    }
    if (snode)
    {
      snode = snode->next;
    }
    else
    {
      snode = envs->local->macros;
    }
    if (!check_macro->next)
      break;

    check_macro = check_macro->next;
  }

  if (!envs->global)
    return (void*) 0;

  return visitor_get_macro(envs->global, ast_macro);
}

Env_macro* visitor_get_macro_newenv(Envs* envs, const char* name, AST_newenv* ast_macro)
{
  Env_macro* check_macro = envs->local->macros;

  Env_macro* snode = (void*) 0;
  while (check_macro)
  {
    if (!strcmp(check_macro->name, name))
    {
      if (snode)
      {
        snode->next = check_macro->next;
        check_macro->next = envs->local->macros;
        envs->local->macros = check_macro;
      }
      return check_macro;
    }
    if (snode)
    {
      snode = snode->next;
    }
    else
    {
      snode = envs->local->macros;
    }
    if (!check_macro->next)
      break;

    check_macro = check_macro->next;
  }

  if (!envs->global)
    return (void*) 0;

  return visitor_get_macro_newenv(envs->global, name, ast_macro);
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

    case AST_MACRO:
      condition_value = 
        visitor_get_value_from_macro(
            envs,
            condition->value.macro_v
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

void visitor_env_variable_autotype(Envs* new_envs, Env_variable* env_variable, AST* ast)
{
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

        const char* satisfy_condition = "!\\begin{code}";
        satisfy_condition = const_strcat(satisfy_condition, ast->value.variable_v->name);
        satisfy_condition = const_strcat(satisfy_condition, ":=");
        satisfy_condition = const_strcat(satisfy_condition, env_variable->name);
        satisfy_condition = const_strcat(satisfy_condition, "\\end{code}");

        off_t len = strlen(satisfy_condition);
        off_t* len_p = &len;
        Lexer* satisfy_lexer = init_lexer((char*) satisfy_condition, len_p);
        Parser* satisfy_parser = init_parser(satisfy_lexer);
        AST* satisfy_ast = parser_parse(satisfy_parser);

        AST_value* satisfy = init_ast_value();
        satisfy->size = 2;
        satisfy->stack = satisfy_ast->value.compound_v->items[0]->value.value_v->stack;

        env_variable->satisfy_size = 1;
        env_variable->satisfy = init_env_value_list(satisfy);
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
  //   switch (new_value->type)
  //   {
  //     case AST_VALUE_INT:
  //       env_variable->type = ENV_VARIABLE_INT;
  //       env_variable->value.int_v = new_value->value.int_v;
  //       break;
  // 
  //     case AST_VALUE_FLOAT:
  //       env_variable->type = ENV_VARIABLE_FLOAT;
  //       env_variable->value.float_v = new_value->value.float_v;
  //       break;
  // 
  //     case AST_VALUE_STRING:
  //       env_variable->type = ENV_VARIABLE_STRING;
  //       env_variable->value.string_v = new_value->value.string_v;
  //       break;
  // 
  //     case AST_VALUE_BOOL:
  //       env_variable->type = ENV_VARIABLE_BOOL;
  //       env_variable->value.bool_v = new_value->value.bool_v;
  //       break;
  // 
  //     case AST_VALUE_MATRIX:
  //       env_variable->type = ENV_VARIABLE_MATRIX;
  //       env_variable->value.matrix_v = new_value->value.matrix_v;
  //       break;
  // 
  //     case AST_VALUE_NULL:
  //       env_variable->type = ENV_VARIABLE_NULL;
  //       //         env_variable->value.null_v = (void*) 0;
  //       break;
  // 
  //     default:
  //       orora_error("에러, 변수의 타입이 없음", (void*) 0);
  //   }
}

AST_value_stack* visitor_get_value_from_macro
(Envs* envs, AST_macro* ast_macro)
{
  Env_macro* env_macro =
    visitor_get_macro(envs, ast_macro);
  if (!env_macro)
  {
//     AST_value_stack* result = check_sysfunc(envs, ast_function);

//     if (result)
    if (0)
      return (void*) 0;
//       return result;
    else
    {
//       free(result);
      visitor_nondefine_macro_error(ast_macro);
    }
  }

  Envs* new_envs = visitor_merge_envs(envs);

  if (env_macro->args_size != ast_macro->args_size)
    orora_error("에러, macro의 argument 개수가 다름", (void*) 0);

  if (ast_macro->super)
  {
    const char* arg_name = "#super";
    Env_variable* env_variable = init_env_variable(
        (char*) arg_name,
        strlen(arg_name)
      );
    env_variable->next = new_envs->local->variables;
    new_envs->local->variable_size ++;
    new_envs->local->variables = env_variable;

    visitor_env_variable_autotype(envs, env_variable, ast_macro->super);
  }

  if (ast_macro->sub)
  {
    const char* arg_name = "#sub";
    Env_variable* env_variable = init_env_variable(
        (char*) arg_name,
        strlen(arg_name)
      );
    env_variable->next = new_envs->local->variables;
    new_envs->local->variable_size ++;
    new_envs->local->variables = env_variable;

    visitor_env_variable_autotype(envs, env_variable, ast_macro->sub);
  }

  for (int i = 0; i < env_macro->args_size; i++)
  {
    const char* arg_name = "#";
    arg_name = const_strcat(arg_name, int_to_string(i+1));
//     Env_variable* env_variable = init_env_variable(
//         (char*) arg_name,
//         strlen(arg_name)
//       );
    AST_macro* ast_macro_ast = init_ast_macro((char*) arg_name, (size_t) strlen(arg_name));
    AST* name_string_ast = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    name_string_ast->value.value_v = init_ast_value();
    name_string_ast->value.value_v->size = 1;
    name_string_ast->value.value_v->stack = init_ast_value_stack(AST_VALUE_MACRO, (void*) 0);
    name_string_ast->value.value_v->stack->value.macro_v = ast_macro_ast;

    AST* name_ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
    name_ast->value.compound_v = init_ast_compound();
    name_ast->value.compound_v->size = 1;
    name_ast->value.compound_v->items[0] = name_string_ast;

    AST_int* ast_int = (AST_int*) malloc(sizeof(struct ast_int_t));
    ast_int->value = 0;
    AST* args_size = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    args_size->value.value_v = init_ast_value();
    args_size->value.value_v->size = 1;
    args_size->value.value_v->stack = init_ast_value_stack(AST_VALUE_INT, (void*) 0);
    args_size->value.value_v->stack->value.int_v = ast_int;
    AST* args_size_ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
    args_size_ast->value.compound_v = init_ast_compound();
    args_size_ast->value.compound_v->size = 1;
    args_size_ast->value.compound_v->items[0] = args_size;

    AST* code_ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
    code_ast->value.compound_v = init_ast_compound();
    code_ast->value.compound_v->size = 1;
    code_ast->value.compound_v->items[0] = ast_macro->args[i];

    if (ast_macro->args[i]->type == AST_VARIABLE && ast_macro->args[i]->value.variable_v->ast_type == AST_VARIABLE_DEFINE)
    {
      Env_variable* env_variable = visitor_variable_define(new_envs, ast_macro->args[i]->value.variable_v);

    }

    AST_newenv* newenv = init_ast_newenv();
    newenv->name = name_ast;
    newenv->args_size = args_size_ast;
    newenv->code = code_ast;

    visitor_macro_define(new_envs, newenv);

//     env_variable->next = new_envs->local->variables;
//     new_envs->local->variable_size ++;
//     new_envs->local->variables = env_variable;
// 
//     visitor_env_variable_autotype(envs, env_variable, ast_macro->args[i]);
  }

  AST_value_stack* result = 
    visitor_get_value_from_ast(new_envs, env_macro->code);

  free(new_envs);
  
  return result;
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
    AST_value_stack* result = check_sysfunc(envs, ast_function);

    if (result)
      return result;
    else
    {
      free(result);
      visitor_nondefine_function_error(ast_function);
    }
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

Env_macro* visitor_macro_define(Envs* envs, AST_newenv* ast_newenv)
{
  AST* macro_name_ast = ast_newenv->name->value.compound_v->items[0];
  AST* macro_args_size_ast = ast_newenv->args_size->value.compound_v->items[0];
  AST* macro_code_ast = ast_newenv->code->value.compound_v->items[0];
  if (!macro_name_ast
      || ast_newenv->name->value.compound_v->size != 1
      || ast_newenv->args_size->value.compound_v->size != 1
      || ast_newenv->code->value.compound_v->size != 1
      || macro_name_ast->type != AST_VALUE
      || macro_name_ast->value.value_v->size != 1
      || macro_name_ast->value.value_v->stack->type != AST_VALUE_MACRO
      || macro_args_size_ast->type != AST_VALUE
      || macro_args_size_ast->value.value_v->size != 1
      || macro_args_size_ast->value.value_v->stack->type != AST_VALUE_INT
      )
  {
    orora_error("에러, newenvironment 설정 잘못되었음", (void*) 0);
  }
  AST_value_stack* macro_name = macro_name_ast->value.value_v->stack;
  AST_value_stack* macro_args_size = macro_args_size_ast->value.value_v->stack;
  if (!macro_name
      || macro_name->type != AST_VALUE_MACRO
      || (macro_name->value.string_v->value[0] != '\\'
          && macro_name->value.string_v->value[0] != '#')
      || !macro_args_size
      || macro_args_size->type != AST_VALUE_INT
      || macro_args_size->value.int_v->value > 9
      || macro_args_size->value.int_v->value < 0
      )
  {
    orora_error("에러, newenvironment 설정 잘못되었음", (void*) 0);
  }
  Env_macro* env_macro =
    visitor_get_macro_newenv(envs, macro_name->value.string_v->value, ast_newenv);

  if (env_macro)
  {
    get_env_macro_from_ast_macro(env_macro, macro_args_size->value.int_v->value, macro_code_ast);
  }
  else
  {
    env_macro = init_env_macro(macro_name->value.string_v->value, macro_args_size->value.int_v->value, macro_code_ast);

    Env* local_env = envs->local;
    env_macro->next = local_env->macros;
    local_env->macro_size ++;
    local_env->macros = env_macro;
  }

  return env_macro;
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

struct AST_VALUE_STACK_INDEX_T* visitor_get_matrix_index(Envs* envs, AST_value* ast_value)
{
  struct AST_VALUE_STACK_INDEX_T* index_result = malloc(sizeof(struct AST_VALUE_STACK_INDEX_T));

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

        case AST_VALUE_MACRO:
          AST_macro* ast_macro = text->value.macro_v;
          new_value_stack = 
            visitor_get_value_from_macro(envs, ast_macro);

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

        case AST_VALUE_MOD:
          result = visitor_operator_mod(result, operand1, operand2);
          break;

        case AST_VALUE_COLON:
          result = visitor_operator_colon(result, operand1, operand2);
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

        case AST_VALUE_COMMA:
          result = visitor_operator_comma(result, operand1, operand2);
          break;

        case AST_VALUE_UNDER:
          index_result = visitor_operator_under_for_index(result, operand1, operand2, envs);
          result = index_result->value;
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

//   if (stack->size != 1)
//   {
//     orora_error("에러, 연산 불가함", (void*) 0);
// //     printf("에러, 연산 불가함\n");
// //     exit(1);
//   }

  free(text_array);

//   AST_value_stack* return_value = parser_pop_value(stack);
  free(stack);
  return index_result;
}

AST_value_stack* visitor_get_value(Envs* envs, AST_value* ast_value)
{
  // if (!ast_value)
  //   return (void*) 0;
  // if (ast_value->size == 1 && ast_value->stack->type == AST_VALUE_MATRIX_INDEX)
  // {
  //   AST_matrix_index* ast_matrix_index = ast_value->stack->value.matrix_index_v;

  //   struct AST_VALUE_STACK_INDEX_T* index = visitor_get_matrix_index(envs, ast_matrix_index->index->value.value_v);

  //   size_t index_size = index->index_size;
  //   if (index_size == 1)
  //   {
  //     index->index[0]->value.value_v->stack = visitor_get_value_from_ast(envs, ast_matrix_index->value);
  //   }
  //   else
  //   {
  //     AST_value_stack* value_ast = visitor_get_value_from_ast(envs, ast_matrix_index->value);
  //     if (value_ast->type != AST_VALUE_MATRIX)
  //     {
  //       orora_error("에러, 대입값은 (1, n) 행렬이어야 함..", (void*) 0);
  //     }
  //     AST_matrix* value_matrix = value_ast->value.matrix_v;
  //     if (!(value_matrix->col_size == 1 && value_matrix->row_size == index_size))
  //     {
  //       orora_error("에러, 대입값은 (1, n) 행렬이어야 함.", (void*) 0);
  //     }
  //     for (int i = 0; i < index_size; i++)
  //     {
  //       index->index[i]->value.value_v->stack = visitor_get_value_from_ast(envs, value_matrix->value[i]);
  //     }
  //   }
  //   AST_value_stack* result = malloc(sizeof(AST_value_stack));
  //   result->type = AST_VALUE_NULL;
  //   return result;
  // }
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

        case AST_VALUE_MACRO:
          AST_macro* ast_macro = text->value.macro_v;
          new_value_stack = 
            visitor_get_value_from_macro(envs, ast_macro);

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

        case AST_VALUE_MOD:
          result = visitor_operator_mod(result, operand1, operand2);
          break;

        case AST_VALUE_COLON:
          result = visitor_operator_colon(result, operand1, operand2);
          break;

        case AST_VALUE_CIRCUMFLEX:
          result = visitor_operator_power(result, operand1, operand2);
          break;

        case AST_VALUE_IN:
          result = visitor_operator_in(result, operand1, operand2);
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

        case AST_VALUE_COMMA:
          result = visitor_operator_comma(result, operand1, operand2);
          break;
        case AST_VALUE_UNDER:
          result = visitor_operator_under(result, operand1, operand2, envs);
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

  Env_variable** args = malloc(sizeof(Env_variable*) * env_function->args_size);
  Env_macro** macros = malloc(sizeof(Env_macro*) * env_function->args_size);
  bool is_variable[env_function->args_size];

  int args_cnt = 0;
  int macros_cnt = 0;

  for (int i = 0; i < env_function->args_size; i ++)
  {
    if (env_function->args[i]->type == AST_VARIABLE)
    {
      is_variable[i] = true;
      AST_variable* ast_variable =
        env_function->args[i]->value.variable_v;
      if (i < ast_function->args_size)
      {
        AST* ast = ast_function->args[i];
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

  //       Envs* global_envs = new_envs->global;
  //       Env_variable* global_variable =
  //         visitor_get_variable(global_envs, ast_variable);
  //       if (global_variable)
  //       {
  //         global_variable->type = env_variable->type;
  //         global_variable->value = env_variable->value;
  //       }
  //       else
  //       {
  //         global_variable = get_deep_copy_env_variable(env_variable);
  //         global_variable->next = global_envs->local->variables;
  //         global_envs->local->variable_size ++;
  //         global_envs->local->variables = global_variable;
  //       }

  //       Env* local_env = new_envs->local;
  //       env_variable->next = local_env->variables;
  //       local_env->variable_size ++;
  //       local_env->variables = env_variable;
        args[args_cnt] = env_variable;
        args_cnt ++;
      }
      else
      {
        if (env_function->args[i]
            ->value.variable_v->ast_type == AST_VARIABLE_DEFINE)
        {
          Env_variable* env_variable =
            init_env_variable(ast_variable->name,
                            ast_variable->name_length);
          
          env_variable = visitor_variable_define(new_envs, env_function->args[i]->value.variable_v);
          args[args_cnt] = env_variable;
          args_cnt ++;
        }
        else
        {
          orora_error("에러, 함수의 매개변수 개수가 다름", (void*) 0);
  //         printf("에러, 함수의 매개변수 개수가 다름\n");
  //         exit(1);
        }
      }
    }
    else if (env_function->args[i]->type == AST_VALUE
             && env_function->args[i]->value.value_v->stack->type == AST_VALUE_MACRO)
    {
      is_variable[i] = false;
      AST_macro* ast_macro = env_function->args[i]->value.value_v->stack->value.macro_v;
      if (i < ast_function->args_size)
      {
        AST* ast = ast_function->args[i];

        Env_macro* env_macro =
          init_env_macro(ast_macro->name, 0, ast);
        
        macros[macros_cnt] = env_macro;
        macros_cnt ++;
      }
      else
      {
        orora_error("에러, macro는 기본 값 설정이 불가함", (void*) 0);
      }
    }
    else
    {
      orora_error("에러, 함수의 매개변수는 변수 또는 매크로만 가능함..", (void*) 0);
    }
  }
  Env* local_env = new_envs->local;
  int variable_cnt = 0;
  int macro_cnt = 0;
  for (int i = 0; i < env_function->args_size; i ++)
  {
    if (is_variable[i])
    {
      args[variable_cnt]->next = local_env->variables;
      local_env->variable_size ++;
      local_env->variables = args[variable_cnt];
      variable_cnt ++;
    }
    else
    {
      macros[macro_cnt]->next = local_env->macros;
      local_env->macro_size ++;
      local_env->macros = macros[macro_cnt];
      macro_cnt ++;
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

  Envs* new_envs = visitor_merge_envs(envs);

  AST* variable_ast = ast_for->variable;
  AST* range_ast = ast_for->range;

  if (variable_ast->type != AST_VARIABLE)
    orora_error("에러, 첫 argument는 변수여야 함", (void*) 0);

  AST_matrix* range = malloc(sizeof(AST_matrix));
  range = visitor_get_value_from_ast(new_envs, range_ast)->value.matrix_v;

  if (range->col_size != 1)
    orora_error("에러, 범위는 (1, n) 행렬이어야 함", (void*) 0);

  Env_variable* env_variable = init_env_variable(
      variable_ast->value.variable_v->name,
      variable_ast->value.variable_v->name_length
    );
  env_variable->next = new_envs->local->variables;
  new_envs->local->variable_size ++;
  new_envs->local->variables = env_variable;

  for (int i=0; i<range->row_size; i++)
  {
    AST* ast_tree = ast_for->code;

    AST_value_stack* new_value = malloc(sizeof(AST_value_stack));
    new_value = visitor_get_value_from_ast(new_envs, range->value[i]);

    switch (new_value->type)
    {
      case AST_VALUE_INT:
        env_variable->type = ENV_VARIABLE_INT;
        env_variable->value.int_v = new_value->value.int_v;
        break;

      case AST_VALUE_FLOAT:
        env_variable->type = ENV_VARIABLE_FLOAT;
        env_variable->value.float_v = new_value->value.float_v;
        break;

      case AST_VALUE_STRING:
        env_variable->type = ENV_VARIABLE_STRING;
        env_variable->value.string_v = new_value->value.string_v;
        break;

      case AST_VALUE_BOOL:
        env_variable->type = ENV_VARIABLE_BOOL;
        env_variable->value.bool_v = new_value->value.bool_v;
        break;

      case AST_VALUE_MATRIX:
        env_variable->type = ENV_VARIABLE_MATRIX;
        env_variable->value.matrix_v = new_value->value.matrix_v;
        break;

      case AST_VALUE_NULL:
        env_variable->type = ENV_VARIABLE_NULL;
        //         env_variable->value.null_v = (void*) 0;
        break;

      default:
        orora_error("에러, 변수의 타입이 없음", (void*) 0);
    }

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

AST_value_stack* visitor_operator_under_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs)
{
  int index = operand2->value.int_v->value;

  if (index <= 0 || index > operand1->value.matrix_v->col_size)
  {
    orora_error("에러, 범위를 넘어감", (void*) 0);
  }

  if (operand1->value.matrix_v->row_size == 1)
  {
    return visitor_get_value_from_ast(envs, operand1->value.matrix_v->value[index - 1]);
  }
  else
  {
    result->type = AST_VALUE_MATRIX;
    result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
    result->value.matrix_v->row_size = operand1->value.matrix_v->row_size;
    result->value.matrix_v->col_size = 1;
    
    result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));

    for (int i = 0; i < operand1->value.matrix_v->row_size; i++)
    {
      result->value.matrix_v->value[i] = operand1->value.matrix_v->value[(index - 1)*operand1->value.matrix_v->row_size + i];
    }

    return result;
  }
}

AST_value_stack* visitor_operator_under(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs)
{
  if (operand1->type != AST_VALUE_MATRIX)
  {
    orora_error("에러, 행렬에 대하여만 _연산이 가능함.", (void*) 0);
  }
  else if (operand2->type != AST_VALUE_MATRIX && operand2->type != AST_VALUE_INT)
  {
    orora_error("에러, 정의되지 않은 연산", (void*) 0);
  }

  if (operand2->type == AST_VALUE_INT)
  {
    return visitor_operator_under_int(result, operand1, operand2, envs);
  }
  else
  {
    int col = operand2->value.matrix_v->col_size;
    int row = operand2->value.matrix_v->row_size;
    if (row == 1)
    {
      if (visitor_get_value_from_ast(envs, operand2->value.matrix_v->value[0])->type == AST_VALUE_INT)
      {
        return visitor_operator_under_int(result, operand1, operand2, envs);
      }
      orora_error("에러, index가 (n, 2)행렬이 아님", (void*) 0);
    }
    if (row != 2)
    {
      orora_error("에러, index가 (n, 2)행렬이 아님", (void*) 0);
    }

    if (col == 1)
    {
      AST* ast_w = operand2->value.matrix_v->value[0];
      AST* ast_h = operand2->value.matrix_v->value[1];
      AST_value_stack* stack_w = visitor_get_value_from_ast(envs, ast_w);
      AST_value_stack* stack_h = visitor_get_value_from_ast(envs, ast_h);
      if (stack_w->type != AST_VALUE_INT || stack_h->type != AST_VALUE_INT)
      {
        orora_error("에러, 정의되지 않은 연산", (void*) 0);
      }
      int w = stack_w->value.int_v->value;
      int h = stack_h->value.int_v->value;
      if (w <= 0 || w > operand1->value.matrix_v->col_size
          || h <= 0 || h > operand1->value.matrix_v->row_size)
      {
        orora_error("에러, 범위를 넘어감", (void*) 0);
      }

      return visitor_get_value_from_ast(envs, operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1]);
    }
    else
    {
      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = row;
      result->value.matrix_v->col_size = 1;
      
      result->value.matrix_v->value = malloc(row * sizeof(struct ast_t));
      for (int i = 0; i < row; i++)
      {
        AST* ast_w = operand2->value.matrix_v->value[2*i];
        AST* ast_h = operand2->value.matrix_v->value[2*i + 1];
        AST_value_stack* stack_w = visitor_get_value_from_ast(envs, ast_w);
        AST_value_stack* stack_h = visitor_get_value_from_ast(envs, ast_h);
        if (stack_w->type != AST_VALUE_INT || stack_h->type != AST_VALUE_INT)
        {
          orora_error("에러, 정의되지 않은 연산", (void*) 0);
        }
        int w = stack_w->value.int_v->value;
        int h = stack_h->value.int_v->value;
        if (w <= 0 || w > operand1->value.matrix_v->col_size
            || h <= 0 || h > operand1->value.matrix_v->row_size)
        {
          orora_error("에러, 범위를 넘어감", (void*) 0);
        }
        result->value.matrix_v->value[i] = operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1];
      }
    }

    return result;
  }

  return (void*) 0;
}


struct AST_VALUE_STACK_INDEX_T* visitor_operator_under_for_index_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs)
{
  int index = operand2->value.int_v->value;

  if (index <= 0 || index > operand1->value.matrix_v->col_size)
  {
    orora_error("에러, 범위를 넘어감", (void*) 0);
  }

  if (operand1->value.matrix_v->row_size == 1)
  {
    struct AST_VALUE_STACK_INDEX_T* return_result = malloc(sizeof(struct AST_VALUE_STACK_INDEX_T));
    return_result->value = visitor_get_value_from_ast(envs, operand1->value.matrix_v->value[index - 1]);
    return_result->index_size = 1;
    return_result->index = malloc(sizeof(struct ast_t));
    return_result->index[0] = operand1->value.matrix_v->value[index - 1];

    return return_result;
  }
  else
  {
    result->type = AST_VALUE_MATRIX;
    result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
    result->value.matrix_v->row_size = operand1->value.matrix_v->row_size;
    result->value.matrix_v->col_size = 1;
    
    result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));

    for (int i = 0; i < operand1->value.matrix_v->row_size; i++)
    {
      result->value.matrix_v->value[i] = operand1->value.matrix_v->value[(index - 1)*operand1->value.matrix_v->row_size + i];
    }

    struct AST_VALUE_STACK_INDEX_T* return_result = malloc(sizeof(struct AST_VALUE_STACK_INDEX_T));
    return_result->value = result;
    return_result->index_size = operand1->value.matrix_v->row_size;
    return_result->index = malloc(operand1->value.matrix_v->row_size * sizeof(struct ast_t));
    for (int i = 0; i < operand1->value.matrix_v->row_size; i++)
    {
      return_result->index[i] = operand1->value.matrix_v->value[(index - 1)*operand1->value.matrix_v->row_size + i];
    }

    return return_result;
  }
}

struct AST_VALUE_STACK_INDEX_T* visitor_operator_under_for_index(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    Envs* envs)
{
  if (operand1->type != AST_VALUE_MATRIX)
  {
    orora_error("에러, 행렬에 대하여만 _연산이 가능함.", (void*) 0);
  }
  else if (operand2->type != AST_VALUE_MATRIX && operand2->type != AST_VALUE_INT)
  {
    orora_error("에러, 정의되지 않은 연산", (void*) 0);
  }

  if (operand2->type == AST_VALUE_INT)
  {
    return visitor_operator_under_for_index_int(result, operand1, operand2, envs);
  }
  else
  {
    int col = operand2->value.matrix_v->col_size;
    int row = operand2->value.matrix_v->row_size;
    if (row == 1)
    {
      if (visitor_get_value_from_ast(envs, operand2->value.matrix_v->value[0])->type == AST_VALUE_INT)
      {
        return visitor_operator_under_for_index_int(result, operand1, operand2, envs);
      }
      orora_error("에러, index가 (n, 2)행렬이 아님", (void*) 0);
    }
    if (row != 2)
    {
      orora_error("에러, index가 (n, 2)행렬이 아님", (void*) 0);
    }

    if (col == 1)
    {
      AST* ast_w = operand2->value.matrix_v->value[0];
      AST* ast_h = operand2->value.matrix_v->value[1];
      AST_value_stack* stack_w = visitor_get_value_from_ast(envs, ast_w);
      AST_value_stack* stack_h = visitor_get_value_from_ast(envs, ast_h);
      if (stack_w->type != AST_VALUE_INT || stack_h->type != AST_VALUE_INT)
      {
        orora_error("에러, 정의되지 않은 연산", (void*) 0);
      }
      int w = stack_w->value.int_v->value;
      int h = stack_h->value.int_v->value;
      if (w <= 0 || w > operand1->value.matrix_v->col_size
          || h <= 0 || h > operand1->value.matrix_v->row_size)
      {
        orora_error("에러, 범위를 넘어감", (void*) 0);
      }

      struct AST_VALUE_STACK_INDEX_T* return_result = malloc(sizeof(struct AST_VALUE_STACK_INDEX_T));
      return_result->value = visitor_get_value_from_ast(envs, operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1]);
      return_result->index_size = 1;
      return_result->index = malloc(sizeof(struct ast_t));
      return_result->index[0] = operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1];

      return return_result;
    }
    else
    {
      result->type = AST_VALUE_MATRIX;
      result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
      result->value.matrix_v->row_size = row;
      result->value.matrix_v->col_size = 1;
      
      result->value.matrix_v->value = malloc(row * sizeof(struct ast_t));

      struct AST_VALUE_STACK_INDEX_T* return_result = malloc(sizeof(struct AST_VALUE_STACK_INDEX_T));
      return_result->index_size = row;
      return_result->index = malloc(row * sizeof(struct ast_t));
      
      for (int i = 0; i < row; i++)
      {
        AST* ast_w = operand2->value.matrix_v->value[2*i];
        AST* ast_h = operand2->value.matrix_v->value[2*i + 1];
        AST_value_stack* stack_w = visitor_get_value_from_ast(envs, ast_w);
        AST_value_stack* stack_h = visitor_get_value_from_ast(envs, ast_h);
        if (stack_w->type != AST_VALUE_INT || stack_h->type != AST_VALUE_INT)
        {
          orora_error("에러, 정의되지 않은 연산", (void*) 0);
        }
        int w = stack_w->value.int_v->value;
        int h = stack_h->value.int_v->value;
        if (w <= 0 || w > operand1->value.matrix_v->col_size
            || h <= 0 || h > operand1->value.matrix_v->row_size)
        {
          orora_error("에러, 범위를 넘어감", (void*) 0);
        }
        result->value.matrix_v->value[i] = operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1];
        return_result->index[i] = operand1->value.matrix_v->value[(w - 1)*operand1->value.matrix_v->row_size + h - 1];
      }
      return_result->value = result;

      return return_result;
    }

    return (void*) 0;
  }

  return (void*) 0;
}
