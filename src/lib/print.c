#include "lib/print.h"
#include "develop/develop_mode.h"

const char* float_to_string(float value)
{
  char* buffer = (char*) malloc(32 * sizeof(char));
  if (buffer == NULL)
    return NULL;
  
  snprintf(buffer, 32, "%f", value);

  return buffer;
}

const char* concatenate_strings(const char* str1, const char* str2)
{
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  
  char* result = (char*) malloc(len1 + len2 + 1);
  if (!result)
      return NULL;
  
  strcpy(result, str1);
  strcat(result, str2);
  
  return result;
}

const char* visitor_print_function_value(AST_value_stack* new_value)
{
  switch (new_value->type)
  {
    case AST_VALUE_STRING:
      return (const char*) new_value->value.string_v->real_value;
//       printf("%s",
//           new_value->value.string_v->real_value);
      break;

    case AST_VALUE_INT:
      char buffer[32];
      snprintf(buffer, sizeof(buffer), "%d", new_value->value.int_v->value);
      return strdup(buffer);
//       return (const char*) new_value->value.int_v->value;
//       printf("%d",
//           new_value->value.int_v->value);
      break;

    case AST_VALUE_FLOAT:
      return float_to_string(new_value->value.float_v->value);
//       printf("%f",
//           new_value->value.float_v->value);
      break;

    case AST_VALUE_NULL:
      return "(NULL)";
//       printf("(NULL)");
      break;

    case AST_VALUE_BOOL:
      return new_value->value.bool_v->value ? "true" : "false";
//       printf(new_value->value.
//               bool_v->value ? "true" : "false");
      break;

    case AST_VALUE_MATRIX:
      AST_matrix* ast_matrix = new_value->value.matrix_v;
      const char* result = "[";
      for (
          int j = 0;
          j < ast_matrix->col_size;
          j ++ 
          )
      {
        result = concatenate_strings(result, "[");
        for (
            int i = 0; 
            i < ast_matrix->row_size;
            i ++ 
            )
        {
          AST_value_stack* new_value2 = 
            ast_matrix->value[j * ast_matrix->row_size + i]
              ->value.value_v->stack;
          result = concatenate_strings(result, visitor_print_function_value(new_value2));
          if (i != ast_matrix->row_size - 1)
            result = concatenate_strings(result, ", ");
        }
        result = concatenate_strings(result, "]");
      }
      result = concatenate_strings(result, "]");
      return result;
//       printf("[");
//       for (
//            int j = 0;
//            j < ast_matrix->col_size;
//            j ++ 
//           )
//       {
//         printf("[");
//         for (
//              int i = 0; 
//              i < ast_matrix->row_size;
//              i ++ 
//             )
//         {
//           AST_value_stack* new_value2 = 
//             ast_matrix->value[j * ast_matrix->row_size + i]
//               ->value.value_v->stack;
//           visitor_print_function_value(new_value2);
//           if (i != ast_matrix->row_size - 1)
//             printf(", ");
//         }
//         printf("]");
//       }
//       printf("]");
      break;

    default:
      printf("에러, 정의되지 않은 출력\n");
      exit(1);
      break;
  }
}

void visitor_print_function_variable(Env_variable* new_value)
{
  switch (new_value->type)
  {
    case ENV_VARIABLE_STRING:
      printf("%s",
          new_value->value.string_v->real_value);
      break;

    case ENV_VARIABLE_INT:
      printf("%d",
          new_value->value.int_v->value);
      break;

    case ENV_VARIABLE_FLOAT:
      printf("%f",
          new_value->value.float_v->value);
      break;

    case ENV_VARIABLE_NULL:
      printf("(NULL)");
      break;

    case ENV_VARIABLE_BOOL:
      printf(new_value->value.
              bool_v->value ? "true" : "false");
      break;

    case ENV_VARIABLE_MATRIX:
      AST_matrix* ast_matrix = new_value->value.matrix_v;
      printf("[");
      for (
           int j = 0;
           j < ast_matrix->col_size;
           j ++ 
          )
      {
        printf("[");
        for (
             int i = 0; 
             i < ast_matrix->row_size;
             i ++ 
            )
        {
          AST_value_stack* new_value2 = 
            ast_matrix->value[j * ast_matrix->row_size + i]
              ->value.value_v->stack;
          visitor_print_function_value(new_value2);
          if (i != ast_matrix->row_size - 1)
            printf(", ");
        }
        printf("]");
      }
      printf("]");
      break;
  }
}

const char* visitor_print_function(Envs* envs, AST* ast)
{
  AST_function* ast_function = ast->value.function_v;

  if (!strcmp(ast_function->name, "print"))
  {
    for (int i = 0; i < ast_function->args_size; i ++)
    {
      AST* ast_function_arg = ast_function->args[i];
      AST_value_stack* new_value;
      switch (ast_function_arg->type)
      {
        case AST_VALUE:
          AST_value* ast_function_arg_value =
            ast_function_arg->value.value_v;
          new_value =
            visitor_get_value(envs, ast_function_arg_value);
          visitor_print_function_value(new_value);
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

          visitor_print_function_variable(variable);
          break;

        case AST_FUNCTION:
          AST_function* ast_function_arg_function =
            ast_function_arg->value.function_v;
//           Env_function* function =
//             visitor_get_function(envs, ast_function_arg_function);
//           if (!function)
//           {
//             visitor_nondefine_function_error(ast_function_arg_function);
//           }

          new_value =
            visitor_get_value_from_function(envs, 
                ast_function_arg_function);

          visitor_print_function_value(new_value);
          break;

        case AST_CODE:
        case AST_CASES:
          new_value = 
            visitor_get_value_from_ast(
                envs, 
                ast_function_arg
              );

          visitor_print_function_value(new_value);
          break;

        default:
          printf("에러, 정의되지 않은 출력\n");
          exit(1);
          break;
      }
    }
  }
  return (void*) 0;
}

