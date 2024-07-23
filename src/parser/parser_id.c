#include <stdlib.h>
#include <string.h>
#include "parser/parser_id.h"
#include "loader/main.h"
#include "loader/error_log.h"
#include "utilities/utils.h"

AST* parser_set_variable_value(Parser* parser, AST* ast, Token* last_token);
AST* parser_set_function(Parser* parser, AST* ast, Token* last_token);

AST* parser_parse_variable
(Parser* parser, AST* ast, 
 Token* last_token, GET_COMPOUND_ENV* compound_env)
{
  Token* token = parser->token;

  if (token == (void*) 0)
    return parser_set_variable_value(parser, ast, last_token);

  // Variable
  switch (token->type)
  {
    case TOKEN_DEFINE:   // Define variable
      if (parser->prev_token->col == token->col_first)
      {
        parser = parser_advance(parser, TOKEN_DEFINE);
        AST* new_ast = parser_value_define(
                          parser, 
                          ast, 
                          last_token, 
                          compound_env
                        );
      
        return new_ast;
      }
      break;

    default:
      return parser_set_variable_value(parser, ast, last_token);
      break;
  }
  // End variable

  return parser_set_variable_value(parser, ast, last_token);
}

AST* parser_parse_function(Parser* parser, AST* ast, Token* last_token,
    AST_function* fa, GET_COMPOUND_ENV* compound_env)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_FUNCTION, ast, last_token);
  new_ast_node->value.function_v = fa;

  if (token == (void*) 0)
  {
    new_ast_node->value.function_v->ast_type = AST_FUNCTION_VALUE;
    return new_ast_node;
  }

  if (token->type == TOKEN_DEFINE)
  {
    if (parser->prev_token->col == token->col_first)
    {
      parser = parser_advance(parser, TOKEN_DEFINE);

      // Check arguments
      char** s_arg_variaable_name = malloc(sizeof(char*));
      int check_arg_num = 0;
      for (int i = 0; i < fa->args_size; i ++)
      {
        if (fa->args[i]->type == AST_VARIABLE)
        {
          char* variable_name =
            fa->args[i]->value.variable_v->name;
          for (int j = 0; j < check_arg_num; j ++)
          {
            if (!strcmp(s_arg_variaable_name[j], variable_name))
            {
              const char* error_message = "에러, 함수 ";
              error_message = const_strcat(error_message, fa->name);
              error_message = const_strcat(error_message, "의 argument 이름이 중복됨");
              orora_error(error_message, parser);
//               printf("에러, 함수의 argument 이름이 중복됨\n");
//               exit(1);
            }
          }

          check_arg_num ++;
          s_arg_variaable_name =
            realloc(s_arg_variaable_name,
                check_arg_num * sizeof(char*));
          s_arg_variaable_name[check_arg_num - 1] = variable_name;
        }
        else
        {
          const char* error_message = "에러, 함수 ";
          error_message = const_strcat(error_message, fa->name);
          error_message = const_strcat(error_message, "의 정의를 위해 argument는 변수여야함");
          orora_error(error_message, parser);
//           int required =
//             snprintf(NULL, 0, "에러, 함수 %s의 정의를 위해 argument는 변수여야함",
//                 fa->name);
//           char* error_message = malloc((required + 1) * sizeof(char));
//           snprintf(error_message, required + 1,
//               "에러, 함수 %s의 정의를 위해 argument는 변수여야함",
//               fa->name);
//           error(error_message, parser);
        }
      }
      token = parser->token;
      // -------------------

      if (!token)
      {
        orora_error("에러, ':=' 사용에 목적이 없음.", parser);
//         printf("에러, ':=' 사용에 목적이 없음.");
//         exit(1);
      }
      if (parser->prev_token->col != token->col_first)
      {
        orora_error("에러, ':=' 뒤에는 값이 와야함.", parser);
//         printf("에러, ':=' 뒤에는 값이 와야함.");
//         exit(1);
      }

      AST* value_node =
          parser_get_value(
              &parser, 
              ast, 
              token, 
              init_get_value_env(), 
              compound_env
            );
      token = parser->prev_token;

      if (value_node)
      {
        // e.g.
        // f(x) := 5x + 3
        
        // e.g.
        // f(x) := \begin{function}
        //           print("hello\n")
        //         \end{function}

        AST_function* new_ast_function = new_ast_node->value.function_v;

//           new_ast_function->code = malloc(sizeof(AST));

        new_ast_function->code = value_node;
      }
      else
      {
        printf("에러, ':=' 뒤에는 값이 와야함.");
        exit(1);
      }
      new_ast_node->value.function_v->ast_type = AST_FUNCTION_DEFINE;
      return new_ast_node;
    }
  }

  new_ast_node->value.function_v->ast_type = AST_FUNCTION_VALUE;
  return new_ast_node;
}

AST* parser_value_define
(Parser* parser, AST* ast, 
 Token* last_token, GET_COMPOUND_ENV* compound_env)
{
  Token* token = parser->token;

  if (!token)
  {
    orora_error("에러, ':=' 사용에 목적이 없음.", parser);
//     printf("에러, ':=' 사용에 목적이 없음.");
//     exit(1);
  }

  // Check value
  if (parser->prev_token->col != token->col_first)
  {
    orora_error("에러, ':=' 뒤에는 값이 와야함.", parser);
//     printf("에러, ':=' 뒤에는 값이 와야함.");
//     exit(1);
  }
  AST* value_node =
      parser_get_value(
          &parser, 
          ast, 
          token, 
          init_get_value_env(), 
          compound_env
        );
  token = parser->prev_token;
  if (value_node)
  {
    if (value_node->value.value_v->size == 1
        && value_node->value.value_v->stack->type == AST_VALUE_VARIABLE)
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->value.variable_v =
        init_ast_variable(last_token->value, last_token->length);

      if (parser->token && parser->token->type == TOKEN_DEFINE
          && parser->prev_token->col == parser->token->col)
      {
        Token* stoken = parser->prev_token;
        parser = parser_advance(parser, TOKEN_DEFINE);

        new_ast_node->value.variable_v->value =
          parser_value_define(
              parser, 
              new_ast_node, 
              stoken, 
              compound_env
            );
        
        token = parser->token;
      }
      else
      {
        AST* new_ast_node2 =
          init_ast(AST_VARIABLE, ast, token);
        new_ast_node2->value.variable_v =
          init_ast_variable(token->value, token->length);
        new_ast_node->value.variable_v->value = new_ast_node2;
        new_ast_node2->value.variable_v->ast_type = AST_VARIABLE_VALUE;

        token = parser->token;
      }
      new_ast_node->value.variable_v->ast_type = AST_VARIABLE_DEFINE;

      return new_ast_node;
    }
    else if (value_node->value.value_v->size == 1
        && value_node->value.value_v->stack->type == AST_VALUE_FUNCTION)
    {
      AST* new_ast_node = 
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->value.variable_v =
        init_ast_variable(last_token->value, last_token->length);

      AST* new_ast_node2 =
        init_ast(AST_FUNCTION, ast, token);
      new_ast_node2->value.function_v =
        value_node->value.value_v->stack->value.function_v;
      new_ast_node->value.variable_v->value = new_ast_node2;
      new_ast_node2->value.function_v->ast_type = AST_FUNCTION_VALUE;

      token = parser->token;

      new_ast_node->value.variable_v->ast_type = AST_VARIABLE_DEFINE;

      return new_ast_node;
    }
    else
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->value.variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->value.variable_v->value = value_node;
      new_ast_node->value.variable_v->ast_type = AST_VARIABLE_DEFINE;

      return new_ast_node;
    }
  }
  else
  {
    orora_error("에러, ':=' 뒤에는 값이 와야함.", parser);
//     printf("에러, ':=' 뒤에는 값이 와야함.");
//     exit(1);
  }

  return (void*) 0;
}

AST* parser_get_function
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env)
{
//   parser = parser_advance(parser, TOKEN_ID);
  Token* token = parser->token;

  if (!parser->next_token
      || token->type != TOKEN_ID
      || parser->next_token->type != TOKEN_LPAR)
    return (void*) 0;

  if (token->col != parser->next_token->col_first)
    return (void*) 0;
//     return parser_set_value(parser, ast, parser->prev_token);

  AST* new_ast = init_ast(AST_FUNCTION, ast, token);
  new_ast->value.function_v =
    init_ast_function(token->value, token->length);
  parser = parser_advance(parser, TOKEN_ID);
  parser = parser_advance(parser, TOKEN_LPAR);
  token = parser->token;

  if (token->type != TOKEN_RPAR)    // if the argument is not empty
  {
    int num_of_lpar = 0;
    while (token && !(num_of_lpar == 0 && token->type == TOKEN_RPAR))
    {
      GET_COMPOUND_ENV* new_env = init_get_compound_env(compound_env);
      new_env->is_size_one = true;
//       new_env->is_allow_linebreak = true;
//       new_env->is_in_parentheses = true;
//       new_env->is_usefull_comma = true;
      AST* new_arg_ast = parser_get_compound(parser, new_env);
      token = parser->token;

      if (new_arg_ast->value.compound_v->size == 0)
      {
        const char* error_message = "에러, 함수 ";
        error_message = const_strcat(error_message, new_ast->value.function_v->name);
        error_message = const_strcat(error_message, "의 ',' 사이 argument가 비어있음");
        orora_error(error_message, parser);
//         int required =
//           snprintf(NULL, 0, "에러, 함수 %s의 ',' 사이 argument가 비어있음",
//             new_ast->value.function_v->name);
//         char* error_message = malloc((required + 1) * sizeof(char));
//         snprintf(error_message, required + 1,
//             "에러, 함수 %s의 ',' 사이 argument가 비어있음",
//             new_ast->value.function_v->name);
//         error(error_message, parser);
      }

      if (new_arg_ast->value.compound_v->size > 1)
      {
//         int required =
//           snprintf(NULL, 0, "에러, 함수 %s의 각 argument는 ','로 구분되어야 함",
//             new_ast->value.function_v->name);
//         char* error_message = malloc((required + 1) * sizeof(char));
        const char* error_message = "에러, 함수 ";
        error_message = const_strcat(error_message, new_ast->value.function_v->name);
        error_message = const_strcat(error_message, "의 각 argument는 ','로 구분되어야 함");

        orora_error(error_message, parser);
//         snprintf(error_message, required + 1,
//             "에러, 함수 %s의 각 argument는 ','로 구분되어야 함",
//             new_ast->value.function_v->name);

//         error_prev_token(error_message, parser);
      }
      if (token && token->type == TOKEN_COMMA)
        parser = parser_advance(parser, TOKEN_COMMA);

      int args_num = ++ new_ast->value.function_v->args_size;
      new_ast->value.function_v->args = realloc(new_ast->value.function_v->args,
          args_num * sizeof(struct ast_t*));
      new_ast->value.function_v->args[args_num - 1] =
        new_arg_ast->value.compound_v->items[0];
    }
  }
  if (token)
  {
    parser = parser_advance(parser, TOKEN_RPAR);
  }
  else
  {
    orora_error("에러, 괄호가 끝나지 아니함", parser);
//     printf("에러, 괄호가 끝나지 아니함\n");
//     exit(1);
  }

  return new_ast;
}

AST* parser_set_variable_value(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_VARIABLE, ast, last_token);
  new_ast_node->value.variable_v =
    init_ast_variable(last_token->value, last_token->length);
  new_ast_node->value.variable_v->ast_type = AST_VARIABLE_VALUE;

  return new_ast_node;
}

AST* parser_set_function(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_FUNCTION, ast, last_token);
  new_ast_node->value.function_v =
    init_ast_function(last_token->value, last_token->length);

  return new_ast_node;
}

