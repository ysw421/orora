#include "parser/parser_id.h"
#include "loader/main.h"
#include "loader/error_log.h"
#include <stdlib.h>
#include <string.h>

AST_value_stack* get_single_value(Parser* parser, AST* ast, bool is_minus);
GET_VALUE_ENV* init_get_value_env();
bool is_operator(int token_id);

AST* parser_get_value
(
 Parser** parser_, AST* ast,
 Token* last_token, GET_VALUE_ENV* value_env, 
 GET_COMPOUND_ENV* compound_env
)
{
  bool is_in_parentheses_save = value_env->is_in_parentheses;
  Parser* parser = *parser_;
  AST_value* stack = init_ast_value();
  AST_value* postfix_expression = init_ast_value();   // result
  bool is_last_value = false;
  bool is_last_single_value = false;  // 3x -> 3 * x
  bool is_last_minus_value = true;    // check to see if you can use the
                                      // minus symbol to represent a value
                                      // for next.
  bool is_last_minus_value2 = false;  // get minus symbol...
  bool is_last_operator = false;
  Token* token = parser->token;

  AST_value_stack* save_value;

  int count_of_dearkelly = 0;
  int count_of_brace = 0;

  bool is_first_turn = true;
  while (token)
  {
    if (!is_first_turn 
        && count_of_dearkelly == 0 
        && count_of_brace == 0 
        && !value_env->is_in_parentheses 
        && parser->prev_token 
        && parser->prev_token->col != token->col_first
       )
      break;

    if (is_first_turn)
      is_first_turn = false;

    bool is_single_value = false;
    orora_value_type* p = value_type_list;
    do
    {
      if (p->is_check_type(parser))
      {
        is_single_value = true;
        break;
      }
      p = p->next;
    } while (p);

    if (is_single_value)
    {
//       fprintf(stderr, "!@#!#@!#\n");
      if (is_last_single_value)
        break;
      else if (is_last_value)
      {
        while (stack->size
            && parser_precedence(stack->stack->type)
                >= parser_precedence(AST_VALUE_DOT_PRODUCT))
        {
          save_value = parser_pop_value(stack);
          parser_push_value(postfix_expression, save_value);
        }
        parser_push_value(stack,
            init_ast_value_stack(AST_VALUE_DOT_PRODUCT, token));
      }
      save_value = get_single_value(parser, ast, is_last_minus_value2);
      parser_push_value(postfix_expression, save_value);
      
      is_last_value = true;
      is_last_minus_value = false;
      is_last_minus_value2 = false;
      is_last_single_value = true;
//       is_last_single_value = false;
      is_last_operator = false;

      token = parser->token;
      continue;
    }
    else
    {
      enum
      {
        TOKEN_TYPE_NULL = 0,
        TOKEN_TYPE_ID,
        TOKEN_TYPE_LPAR,
        TOKEN_TYPE_RPAR,
        TOKEN_TYPE_OPERATOR,
        TOKEN_TYPE_LBRACE,
        TOKEN_TYPE_RBRACE,
        TOKEN_TYPE_BEGIN
      };
      int token_type = TOKEN_TYPE_NULL;
      switch (token->type)
      {
        case TOKEN_ID:
          token_type = TOKEN_TYPE_ID;
          break;

        case TOKEN_LPAR:
          token_type = TOKEN_TYPE_LPAR;
          break;

        case TOKEN_RPAR:
          token_type = TOKEN_TYPE_RPAR;
          break;

        case TOKEN_LBRACE:
          token_type = TOKEN_TYPE_LBRACE;
          break;

        case TOKEN_RBRACE:
          token_type = TOKEN_TYPE_RBRACE;
          break;

        case TOKEN_BEGIN:
          token_type = TOKEN_TYPE_BEGIN;
          break;

        default:
          if (is_operator(token->type))
            token_type = TOKEN_TYPE_OPERATOR;
          break;
      }

      if (token_type == TOKEN_TYPE_NULL)
      {
        break;
      }
      else if (is_last_minus_value2)
      {
        orora_error("에러, -연산자 다음에는 값이 와야함", (void*) 0);
//         printf("에러, operator 뒤에는 값이 와야함\n");
//         exit(1);
      }

      bool is_continue = false;
      bool is_break = false;
      switch (token_type)
      {
        case TOKEN_TYPE_ID:
          if (is_last_single_value)
          {
            while (stack->size
                && parser_precedence(stack->stack->type)
                    >= parser_precedence(AST_VALUE_DOT_PRODUCT))
            {
              save_value = parser_pop_value(stack);
              parser_push_value(postfix_expression, save_value);
            }
            parser_push_value(stack,
                init_ast_value_stack(AST_VALUE_DOT_PRODUCT, token));
          }
          else if (is_last_value)
          {
            orora_error("에러, 값 전에 operator가 와야함", (void*) 0);
//             printf("에러, 값 전에 operator가 와야함\n");
//             exit(1);
          }
          
          AST* function_ast = parser_get_function(
                                  parser, 
                                  ast, 
                                  compound_env
                                );
          token = parser->token;

          is_last_value = true;
          is_last_minus_value = false;
          is_last_operator = false;

          is_last_single_value = true;

          AST_value_stack* new;
          if (function_ast)
          {
            new = init_ast_value_stack(AST_VALUE_FUNCTION, token);
            function_ast->value.function_v->ast_type = AST_FUNCTION_VALUE;
            new->value.function_v = function_ast->value.function_v;
            parser_push_value(postfix_expression, new);
    
            is_continue = true;
            break;
          }
          else
          {
            new = init_ast_value_stack(AST_VALUE_VARIABLE, token);
            new->value.variable_v =
              init_ast_variable(token->value, token->length);
            new->value.variable_v->ast_type = AST_VARIABLE_VALUE;
            parser_push_value(postfix_expression, new);
          }
          break;

        case TOKEN_TYPE_LPAR:
          count_of_dearkelly ++;
          parser_push_value(stack,
              init_ast_value_stack(AST_VALUE_LPAR, token));

          is_last_value = false;
          is_last_minus_value = true;
          is_last_single_value = false;
          is_last_operator = false;
          break;

        case TOKEN_TYPE_RPAR:
          if (!count_of_dearkelly)
          {
            is_break = true;
            break;
          }

          count_of_dearkelly --;
          if (!count_of_dearkelly && !is_in_parentheses_save)
          {
            value_env->is_in_parentheses = false;
          }
          while (stack->stack && stack->stack->type != AST_VALUE_LPAR)
          {
            parser_push_value(postfix_expression, parser_pop_value(stack));
          }
          free(parser_pop_value(stack));

          is_last_value = true;
          is_last_minus_value = false;
//           is_last_single_value = false;
          is_last_single_value = true;
          is_last_operator = false;
          break;

        case TOKEN_TYPE_LBRACE:
          count_of_brace ++;
          parser_push_value(stack,
              init_ast_value_stack(AST_VALUE_LBRACE, token));

          is_last_value = false;
          is_last_minus_value = true;
          is_last_single_value = false;
          is_last_operator = false;
          break;

        case TOKEN_TYPE_RBRACE:
          if (!count_of_brace)
          {
            is_break = true;
            break;
          }

          count_of_brace --;
          if (!count_of_brace && !is_in_parentheses_save)
          {
            value_env->is_in_parentheses = false;
          }
          while (stack->stack && stack->stack->type != AST_VALUE_LBRACE)
          {
            parser_push_value(postfix_expression, parser_pop_value(stack));
          }
          free(parser_pop_value(stack));

          is_last_value = true;
          is_last_minus_value = false;
          is_last_single_value = false;
//           is_last_single_value = false;
          is_last_operator = false;
          break;

        case TOKEN_TYPE_BEGIN:
          Token* s_token = parser->token;
          char* code = parser_is_begin(parser, 4, 
                          "cases", "code", "function", "fun"
                       );
          if (code)
          {
            AST* begin_ast;
            if (!strcmp(code, "cases"))
            {
              begin_ast = parser_get_cases(
                              parser, 
                              ast, 
                              token, 
                              s_token,
                              compound_env
                            );
              token = parser->token;
            }
            else if (
                     !strcmp(code, "code") 
                     || !strcmp(code, "function") 
                     || !strcmp(code, "fun")
                    )
            {
              begin_ast = parser_get_code(
                              parser, 
                              ast, 
                              token, 
                              s_token, 
                              compound_env, 
                              code
                            );
              token = parser->token;
            }

            if (is_last_single_value)
              break;
            else if (is_last_value)
            {
              while (stack->size
                  && parser_precedence(stack->stack->type)
                      >= parser_precedence(AST_VALUE_DOT_PRODUCT))
              {
                save_value = parser_pop_value(stack);
                parser_push_value(postfix_expression, save_value);
              }
              parser_push_value(stack,
                  init_ast_value_stack(AST_VALUE_DOT_PRODUCT, token));
            }
            Token* token = parser->token;

            AST_value_stack* new;
            if (is_last_minus_value2)
            {
              orora_error("에러, begin은 -연산이 불가함", (void*) 0);
//               printf("에러, begin은 -연산이 불가함\n");
//               exit(1);
            }

            if (!strcmp(code, "cases"))
            {
              new = init_ast_value_stack(AST_VALUE_CASES, token);
              new->value.cases_v = begin_ast->value.cases_v;
            }
            else if (
                     !strcmp(code, "code") 
                     || !strcmp(code, "function") 
                     || !strcmp(code, "fun")
                    )
            {
              new = init_ast_value_stack(AST_VALUE_CODE, token);
              new->value.code_v = begin_ast->value.code_v;
            }

            save_value = new;
            parser_push_value(postfix_expression, save_value);
            
            is_last_value = true;
            is_last_minus_value = false;
            is_last_minus_value2 = false;
            is_last_single_value = true;
            is_last_operator = false;

            token = parser->token;
            continue;
          }
          break;

        case TOKEN_TYPE_OPERATOR:
          if (token->type == TOKEN_MINUS
            && (is_last_minus_value || is_last_operator))
          {
            is_last_minus_value2 = true;
          }
          else
          {
            if (!is_last_value && !is_operator_use_one_value(token->type))
            {
              orora_error("에러, operator는 값 다음에 와야 함", (void*) 0);
//               printf("에러, operator는 값 다음에 와야 함\n");
//               exit(1);
            }
            while (stack->size
                && parser_precedence(stack->stack->type)
                    >= parser_precedence(get_ast_value_type(token->type)))
            {
              parser_push_value(postfix_expression, parser_pop_value(stack));
            }
            parser_push_value(stack,
                init_ast_value_stack(get_ast_value_type(token->type), token));
          }
          is_last_minus_value = false;
          is_last_value = false;
          is_last_single_value = false;
          is_last_operator = true;
          break;
      }

      if (is_continue)
        continue;
      
      if (is_break)
        break;
    }

    parser = parser_advance(parser, token->type);
    token = parser->token;
  }

  while (stack->size)
    parser_push_value(postfix_expression, parser_pop_value(stack));
  free(stack);

  if (postfix_expression->size == 0)
  {
    free(postfix_expression);
    return (void*) 0;
  }

  // Warning! possibility error
  if (!is_first_turn && !is_last_value)
  {
    orora_error("에러, operator 다음에는 값이 와야함", (void*) 0);
//     printf("에러, operator 다음에는 값이 와야함\n");
//     exit(1);
  }
  else if(count_of_dearkelly)
  {
    orora_error("에러, (가 끝나지 아니함", (void*) 0);
//     printf("에러, (가 끝나지 아니함\n");
//     exit(1);
  }
  else if(count_of_brace)
  {
    orora_error("에러, {가 끝나지 아니함", (void*) 0);
//     printf("에러, {가 끝나지 아니함\n");
//     exit(1);
  }

//   free(value_env);
  AST* new_ast_node =
    init_ast(AST_VALUE, ast, last_token);
  new_ast_node->value.value_v = postfix_expression;

  *parser_ = parser;
  return new_ast_node;
}

AST_value_stack* get_single_value(Parser* parser, AST* ast, bool is_minus)
{
  orora_value_type* p = value_type_list;
  do
  {
    if (p->is_check_type(parser))
      return p->parser_get_new_ast_value_stack(parser, is_minus);
    p = p->next;
  } while (p);

  return (void*) 0;
}

AST* parser_get_new_int_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_INT, ast, token);
  new_ast_node->value.int_v = init_ast_int(parser);

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token);
  new_ast_node->value.float_v = init_ast_float(parser);

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_STRING, ast, token);
  new_ast_node->value.string_v = init_ast_string(parser);

  return new_ast_node;
}

AST* parser_get_new_null_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node = 
    init_ast(AST_NULL, ast, token);

  parser = parser_advance(parser, TOKEN_NULL);

  return new_ast_node;
}

AST* parser_get_new_bool_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_BOOL, ast, token);
  new_ast_node->value.bool_v = init_ast_bool(parser);

  return new_ast_node;
}

AST* parser_get_new_matrix_ast(AST* ast, Parser* parser)
{
  Token* token = parser->token;

  AST* new_ast_node = 
    init_ast(AST_MATRIX, ast, token);
  new_ast_node->value.matrix_v = init_ast_matrix(parser, false);

  return new_ast_node;
}

AST_value_stack* parser_get_new_int_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_INT, token);
  AST_int* new_value = init_ast_int(parser);
  if (is_minus)
    new_value->value = 0 - new_value->value;
  new->value.int_v = new_value;

  return new;
}

AST_value_stack* parser_get_new_float_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_FLOAT, token);
  AST_float* new_value = init_ast_float(parser);
  if (is_minus)
    new_value->value = 0 - new_value->value;
  new->value.float_v = new_value;

  return new;
}

AST_value_stack* parser_get_new_string_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  if (is_minus)
  {
    orora_error("에러, string에는 -연산이 불가함", (void*) 0);
//     printf("에러, string에는 -연산이 불가함\n");
//     exit(1);
  }
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_STRING, token);
  new->value.string_v = init_ast_string(parser);

  return new;
}

AST_value_stack* parser_get_new_null_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  if (is_minus)
  {
    orora_error("에러, null에는 -연산이 불가함", (void*) 0);
//     printf("에러, null에는 -연산이 불가함\n");
//     exit(1);
  }
  AST_value_stack* new = 
    init_ast_value_stack(AST_VALUE_NULL, token);

  parser = parser_advance(parser, TOKEN_NULL);

  return new;
}

AST_value_stack* parser_get_new_bool_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_BOOL, token);
  AST_bool* new_value = init_ast_bool(parser);
  if (is_minus)
    new_value->value = !new_value->value;
  new->value.bool_v = new_value;

  return new;
}

AST_value_stack* parser_get_new_matrix_ast_value_stack
  (Parser* parser, bool is_minus)
{
  Token* token = parser->token;

  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_MATRIX, token);
  AST_matrix* new_value = init_ast_matrix(parser, is_minus);

  new->value.matrix_v = new_value;

  return new;
}

bool is_string_ast(Parser* parser)
{
  Token* token = parser->token;

  if (token->type == TOKEN_STRING)
    return true;
  return false;
}

bool is_float_ast(Parser* parser)
{
  Token* token = parser->token;

  if (token->type == TOKEN_FLOAT)
    return true;
  return false;
}

bool is_int_ast(Parser* parser)
{
  Token* token = parser->token;

  if (token->type == TOKEN_INT)
    return true;
  return false;
}

bool is_null_ast(Parser* parser)
{
  Token* token = parser->token;

  if (token->type == TOKEN_NULL)
    return true;
  return false;
}

bool is_bool_ast(Parser* parser)
{
  Token* token = parser->token;

  if (token->type == TOKEN_BOOL)
    return true;
  return false;
}

bool is_matrix_ast(Parser* parser)
{
  size_t pointer = parser->pointer;
  char* code = parser_is_begin(parser, 3, "matrix", "pmatrix", "bmatrix");

  if (code)
  {
    // Warning!
    parser = parser_set(parser, pointer);
    return true;
  }
  return false;
}

AST_value_stack* parser_pop_value(AST_value* value)
{
  if (value->size == 0)
  {
//     int required =
//       snprintf(NULL, 0, "에러, %s가 무엇이죠??::type: %d",
//     token->value, token->type);
//     char* error_message = malloc((required + 1) * sizeof(char));
//     snprintf(error_message, required + 1,
//               "에러, %s가 무엇이죠??::type: %d",
//               token->value, token->type);
//     error(error_message, parser, token);
    orora_error("에러, 삭제 불가함", (void*) 0);
//     printf("에러, 삭제 불가함");
//     exit(1);
  }
  value->size --;
  AST_value_stack* snode = value->stack;
  value->stack = value->stack->next;

  return snode;
}

AST_value* parser_push_value(AST_value* value, AST_value_stack* node)
{
  value->size ++;
  AST_value_stack* snode = value->stack;
  value->stack = node;
  if (snode)
    value->stack->next = snode;

  return value;
}

GET_VALUE_ENV* init_get_value_env()
{
  GET_VALUE_ENV* new_env = malloc(sizeof(GET_VALUE_ENV));
  new_env->is_in_parentheses = false;

  return new_env;
}

int parser_precedence(int ast_stack_id)
{
  switch (ast_stack_id)
  {
    case AST_VALUE_LPAR:          // (
    case AST_VALUE_LBRACE:        // {
      return 1;
      break;

    case AST_VALUE_OR:            // ||
    case AST_VALUE_AND:           // &&
      return 2;
      break;

    case AST_VALUE_EQUAL:         // =
    case AST_VALUE_NOTEQUAL:      // !=
      return 3;
      break;

    case AST_VALUE_LESS:          // <
    case AST_VALUE_GREATER:       // >
    case AST_VALUE_LESSEQUAL:     // <=
    case AST_VALUE_GREATEREQUAL:  // >=
      return 4;
      break;

    case AST_VALUE_PLUS:          // +
    case AST_VALUE_MINUS:         // -
      return 5;
      break;

    case AST_VALUE_DOT_PRODUCT:   // \cdot
    case AST_VALUE_PRODUCT:       // *, \times
    case AST_VALUE_DIV:
      return 6;
      break;

    case AST_VALUE_CIRCUMFLEX:    // ^
      return 7;
      break;

    case AST_VALUE_NEG:           // !
      return 8;
      break;

    case AST_VALUE_RPAR:
    case AST_VALUE_RBRACE:
      return 99;
      break;
  }
  
  return -1;
}

bool is_operator(int token_id)
{
  switch (token_id)
  {
    case TOKEN_PLUS:
    case TOKEN_MINUS:
    case TOKEN_CDOT:
    case TOKEN_STAR:
    case TOKEN_SLASH:
    case TOKEN_EQUAL:
    case TOKEN_LESS:
    case TOKEN_GREATER:
    case TOKEN_NOTEQUAL:
    case TOKEN_LESSEQUAL:
    case TOKEN_GREATEREQUAL:
    case TOKEN_OR:
    case TOKEN_AND:
    case TOKEN_NEG:
    case TOKEN_CIRCUMFLEX:
      return true;
  }

  return false;
}

bool is_operator_use_one_value(int token_id)
{
  switch (token_id)
  {
    case TOKEN_MINUS:
    case TOKEN_NEG:
      return true;
  }

  return false;
}

int get_ast_value_type(int token_id)
{
  switch (token_id)
  {
    case TOKEN_PLUS:                return AST_VALUE_PLUS;
    case TOKEN_MINUS:               return AST_VALUE_MINUS;
    case TOKEN_STAR:                return AST_VALUE_PRODUCT;
    case TOKEN_CDOT:                return AST_VALUE_DOT_PRODUCT;
    case TOKEN_SLASH:               return AST_VALUE_DIV;
    case TOKEN_EQUAL:               return AST_VALUE_EQUAL;
    case TOKEN_LESS:                return AST_VALUE_LESS;
    case TOKEN_GREATER:             return AST_VALUE_GREATER;
    case TOKEN_LESSEQUAL:           return AST_VALUE_LESSEQUAL;
    case TOKEN_GREATEREQUAL:        return AST_VALUE_GREATEREQUAL;
    case TOKEN_NOTEQUAL:            return AST_VALUE_NOTEQUAL;
    case TOKEN_OR:                  return AST_VALUE_OR;
    case TOKEN_AND:                 return AST_VALUE_AND;
    case TOKEN_NEG:                 return AST_VALUE_NEG;
    case TOKEN_CIRCUMFLEX:          return AST_VALUE_CIRCUMFLEX;
  }

  return -1;
}

int get_token_type(int ast_value_id)
{
  switch (ast_value_id)
  {
    case AST_VALUE_PLUS:            return TOKEN_PLUS;
    case AST_VALUE_MINUS:           return TOKEN_MINUS;
    case AST_VALUE_PRODUCT:         return TOKEN_STAR;
    case AST_VALUE_DOT_PRODUCT:     return TOKEN_CDOT;
    case AST_VALUE_DIV:             return TOKEN_SLASH;
    case AST_VALUE_EQUAL:           return TOKEN_EQUAL;
    case AST_VALUE_LESS:            return TOKEN_LESS;
    case AST_VALUE_GREATER:         return TOKEN_GREATER;
    case AST_VALUE_LESSEQUAL:       return TOKEN_LESSEQUAL;
    case AST_VALUE_GREATEREQUAL:    return TOKEN_GREATEREQUAL;
    case AST_VALUE_NOTEQUAL:        return TOKEN_NOTEQUAL;
    case AST_VALUE_OR:              return TOKEN_OR;
    case AST_VALUE_AND:             return TOKEN_AND;
    case AST_VALUE_NEG:             return TOKEN_NEG;
    case AST_VALUE_CIRCUMFLEX:      return TOKEN_CIRCUMFLEX;
  }

  return -1;
}

