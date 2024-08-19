#include "develop/develop_mode.h"
#include "syslib/print.h"
#include "loader/error_log.h"
#include "server/status.h"
#include "syslib/console_print.h"

const char* visitor_get_function_value(AST_value_stack* new_value);
const char* visitor_print_function_value(AST_value_stack* new_value);
void visitor_print_function_variable(Env_variable* new_value);
AST_value_stack* visitor_print_function(Envs* envs, AST_function* ast_function);
const char* get_matrix_text(AST_matrix* ast_matrix);

const char* visitor_get_function_value(AST_value_stack* new_value)
{
  const char* output;
  switch (new_value->type)
  {
    case AST_VALUE_STRING:
      output = (const char*) new_value->value.string_v->real_value;
      return output;
      break;

    case AST_VALUE_INT:
      char buffer[32];
      snprintf(buffer, sizeof(buffer), "%d", new_value->value.int_v->value);
      output = (const char*) strdup(buffer);
      return output;
      break;

    case AST_VALUE_FLOAT:
      output = float_to_string(new_value->value.float_v->value);
      return output;
      break;

    case AST_VALUE_NULL:
      output = "(NULL)";
      return output;
      break;

    case AST_VALUE_BOOL:
      output = (const char*) new_value->value.bool_v->value ? "true" : "false";
      return output;
      break;

    case AST_VALUE_MATRIX:
      output = get_matrix_text(new_value->value.matrix_v);
      return output;
      break;

    default:
      orora_error("에러, 정의되지 않은 출력", (void*) 0);
      break;
  }
  return (void*) 0;
}

const char* visitor_print_function_value(AST_value_stack* new_value)
{
  const char* output = visitor_get_function_value(new_value);

  console_print(output);
//   if (INTERACTIVE_MODE)
//     fprintf(stderr, "%s", output);
// //     orora_write(output, ORORA_STATUS_CONSOLE);
//   else
//     fprintf(stderr, "%s", output);
  return output;
}

void visitor_print_function_variable(Env_variable* new_value)
{
  switch (new_value->type)
  {
    case ENV_VARIABLE_STRING:
      console_print((const char*) new_value->value.string_v->real_value);
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "%s", new_value->value.string_v->real_value);
// //         orora_write(new_value->value.string_v->real_value, ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "%s", new_value->value.string_v->real_value);
      break;

    case ENV_VARIABLE_INT:
      console_print(int_to_string(new_value->value.int_v->value));
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "%d", new_value->value.int_v->value);
// //         orora_write(int_to_string(new_value->value.int_v->value), ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "%d", new_value->value.int_v->value);
      break;

    case ENV_VARIABLE_FLOAT:
      console_print(float_to_string(new_value->value.float_v->value));
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "%f", new_value->value.float_v->value);
// //         orora_write(float_to_string(new_value->value.float_v->value), ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "%f", new_value->value.float_v->value);
      break;

    case ENV_VARIABLE_NULL:
      console_print("(NULL)");
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "(NULL)");
// //         orora_write("(NULL)", ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "(NULL)");
      break;

    case ENV_VARIABLE_BOOL:
      console_print(new_value->value.bool_v->value ? "true" : "false");
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "%s", new_value->value.bool_v->value ? "true" : "false");
// //         orora_write(new_value->value.bool_v->value ? "true" : "false", ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "%s", new_value->value.bool_v->value ? "true" : "false");
      break;

    case ENV_VARIABLE_MATRIX:
      const char* output = get_matrix_text(new_value->value.matrix_v);
      console_print(output);
//       if (INTERACTIVE_MODE)
//         fprintf(stderr, "%s", output);
// //         orora_write(output, ORORA_STATUS_CONSOLE);
//       else
//         fprintf(stderr, "%s", output);
      break;
  }
}

AST_value_stack* visitor_print_function(Envs* envs, AST_function* ast_function)
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
        orora_error("에러, 정의되지 않은 출력", (void*) 0);
//           printf("에러, 정의되지 않은 출력\n");
//           exit(1);
        break;
    }
  }
  return init_ast_value_stack(AST_VALUE_NULL, (void*) 0);
}

const char* get_matrix_text(AST_matrix* ast_matrix)
{
  const char* result;
  if (ast_matrix->col_size != 1)
    result = "[";
  else
    result = "";
  for (
      int j = 0;
      j < ast_matrix->col_size;
      j ++ 
      )
  {
    if (j != 0)
      result = const_strcat(result, "\n ");
    result = const_strcat(result, "[");
    for (
        int i = 0; 
        i < ast_matrix->row_size;
        i ++ 
        )
    {
      AST_value_stack* new_value2 = 
        ast_matrix->value[j * ast_matrix->row_size + i]
          ->value.value_v->stack;
      result = const_strcat(result, visitor_get_function_value(new_value2));
      if (i != ast_matrix->row_size - 1)
        result = const_strcat(result, ", ");
    }
    result = const_strcat(result, "]");
  }
  if (ast_matrix->col_size != 1)
    result = const_strcat(result, "]");

  return result;
}
