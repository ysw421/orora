#include "parser_id.h"
#include "../main.h"
#include <stdlib.h>

AST_value_stack* get_single_value(Parser* parser, AST* ast);
GET_VALUE_ENV* init_get_value_env();
int parser_precedence(int type_id);
AST_value_stack* pop_value(AST_value* value);
AST_value* push_value(AST_value* value, AST_value_stack* node);
bool is_operator(int token_id);
int get_ast_value_type(int token_id);


AST* parser_get_value(Parser* parser, AST* ast,
    Token* last_token, GET_VALUE_ENV* value_env)
{
  AST_value* stack = init_ast_value();
  AST_value* postfix_expression = init_ast_value();   // result
  bool is_last_value = false;       // 3x -> 3 * x
  Token* token = parser->token;

  AST_value_stack* save_value;

  int count_of_dearkelly = 0;

  printf("### %s\n", token->value);
  bool is_first_turn = true;
  while (token != (void*) 0)
  {
    if (!is_first_turn && !value_env->is_in_parentheses &&
        parser->prev_token && parser->prev_token->col != token->col_first)
      break;
    printf("@@@ %s\n", token->value);

    if (is_first_turn)
      is_first_turn = false;

    if (token->type == TOKEN_LPAR)
    {
      count_of_dearkelly ++;
      value_env->is_in_parentheses = true;
      push_value(stack,
          init_ast_value_stack(AST_VALUE_LPAR, token));

      is_last_value = false;
    }
    else if (token->type == TOKEN_RPAR)
    {
      if (!count_of_dearkelly)
        break;

      count_of_dearkelly --;
      if (!count_of_dearkelly)
        value_env->is_in_parentheses = false;
      while (stack->stack->type != AST_VALUE_LPAR)
      {
        push_value(postfix_expression, pop_value(stack));
      }
      pop_value(stack);

      is_last_value = true;
    }
    else if (is_operator(token->type))
    {
      while (stack->size
          && parser_precedence(stack->stack->type)
              >= parser_precedence(get_ast_value_type(token->type)))
      {
        push_value(postfix_expression, pop_value(stack));
      }
      push_value(stack,
          init_ast_value_stack(get_ast_value_type(token->type), token));

      is_last_value = false;
    }

    else
    {
      bool is_single_value = false;
      orora_value_type* p = value_type_list;
      do
      {
        if (token->type == p->token_id)
        {
          is_single_value = true;
          break;
        }
        p = p->next;
      } while (p);

      if (is_single_value)
      {
        if (is_last_value)
        {
          while (stack->size
              && parser_precedence(stack->stack->type)
                  >= parser_precedence(AST_VALUE_PRODUCT))
          {
            save_value = pop_value(stack);
            push_value(postfix_expression, save_value);
          }
          push_value(postfix_expression,
              init_ast_value_stack(AST_VALUE_PRODUCT, token));
        }
        save_value = get_single_value(parser, ast);
        push_value(postfix_expression, save_value);
        is_last_value = true;
      }
      else
        break;
    }

    // For develop
    parser_advance(parser, token->type);
    token = parser->token;
//     break;
    // end for
  }
  printf("end: %s\n", token->value);
  while (stack->size)
    push_value(postfix_expression, pop_value(stack));
  free(stack);

  if (postfix_expression->size == 0)
  {
    free(postfix_expression);
    return (void*) 0;
  }

  free(value_env);
  AST* new_ast_node =
    init_ast(AST_VALUE, ast, last_token);
  new_ast_node->value_v = postfix_expression;
  return new_ast_node;
}

AST_value_stack* get_single_value(Parser* parser, AST* ast)
{
  Token* token = parser->token;

  orora_value_type* p = value_type_list;
  do
  {
    if (token->type == p->token_id)
      return p->parser_get_new_ast_value_stack(token);
    p = p->next;
  } while (p);

  return (void*) 0;
}

AST* parser_get_new_int_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_INT, ast, token);
  new_ast_node->int_v = init_ast_int(token);

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token);
  new_ast_node->float_v = init_ast_float(token);

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_STRING, ast, token);
  new_ast_node->string_v = init_ast_string(token);

  return new_ast_node;
}

AST_value_stack* parser_get_new_int_ast_value_stack(Token* token)
{
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_INT, token);
  new->value.int_v = init_ast_int(token);

  return new;
}

AST_value_stack* parser_get_new_float_ast_value_stack(Token* token)
{
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_FLOAT, token);
  new->value.float_v = init_ast_float(token);

  return new;
}

AST_value_stack* parser_get_new_string_ast_value_stack(Token* token)
{
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_STRING, token);
  new->value.string_v = init_ast_string(token);

  return new;
}

AST_value_stack* pop_value(AST_value* value)
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
    printf("에러, 삭제 불가함");
    exit(1);
  }
  value->size --;
  AST_value_stack* snode = value->stack;
  value->stack = value->stack->next;

  return snode;
}

AST_value* push_value(AST_value* value, AST_value_stack* node)
{
  value->size ++;
  AST_value_stack* snode = value->stack;
  value->stack = node;
  value->stack->next = snode;

  return value;
}

GET_VALUE_ENV* init_get_value_env()
{
  GET_VALUE_ENV* new_env = malloc(sizeof(GET_VALUE_ENV));
  new_env->is_in_parentheses = false;

  return new_env;
}

int parser_precedence(int type_id)
{
  switch (type_id)
  {
    case TOKEN_LPAR:
      return 1;
      break;
    case TOKEN_PLUS:
      return 2;
      break;
    case TOKEN_RPAR:
      return 3;
      break;
  }
  
  return -1;
}

bool is_operator(int token_id)
{
  switch (token_id)
  {
    case TOKEN_PLUS:
      return true;
  }

  return false;
}

int get_ast_value_type(int token_id)
{
  switch (token_id)
  {
    case TOKEN_PLUS:
      return AST_VALUE_PLUS;
      break;
  }

  return -1;
}
