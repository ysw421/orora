#include "parser_id.h"
#include "../main.h"
#include <stdlib.h>

AST_value_stack* get_single_value(Parser* parser, AST* ast, bool is_minus);
GET_VALUE_ENV* init_get_value_env();
int parser_precedence(int type_id);
AST_value_stack* pop_value(AST_value* value);
AST_value* push_value(AST_value* value, AST_value_stack* node);
bool is_operator(int token_id);
int get_ast_value_type(int token_id);


AST* parser_get_value(Parser** parser_, AST* ast,
    Token* last_token, GET_VALUE_ENV* value_env)
{
  bool is_in_parentheses_save = value_env->is_in_parentheses;
  Parser* parser = *parser_;
  AST_value* stack = init_ast_value();
  AST_value* postfix_expression = init_ast_value();   // result
  bool is_last_value = false;       // 3x -> 3 * x
  bool is_last_minus_value = true;    // check to see if you can use the
                                      // minus symbol to represent a value
                                      // for next.
  bool is_last_minus_value2 = false;  // get minus symbol...
  Token* token = parser->token;

  AST_value_stack* save_value;

  int count_of_dearkelly = 0;

  bool is_first_turn = true;
  while (token)
  {
    if (!is_first_turn && !value_env->is_in_parentheses &&
        parser->prev_token && parser->prev_token->col != token->col_first)
      break;

    if (is_first_turn)
      is_first_turn = false;

    bool is_single_value = false;
    orora_value_type* p = value_type_list;
    do
    {
      if (p->is_check_type(token))
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
        push_value(stack, init_ast_value_stack(AST_VALUE_PRODUCT, token));
      }
      save_value = get_single_value(parser, ast, is_last_minus_value2);
      push_value(postfix_expression, save_value);
      
      is_last_value = true;
      is_last_minus_value = false;
      is_last_minus_value2 = false;
    }
    else
    {
      is_last_minus_value2 = false;
      if (token->type == TOKEN_ID)
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
          push_value(stack, init_ast_value_stack(AST_VALUE_PRODUCT, token));
        }
        
        AST* function_ast = parser_get_function(parser, ast);
        token = parser->token;

        AST_value_stack* new;
        if (function_ast)
        {
          new = init_ast_value_stack(AST_VALUE_FUNCTION, token);
          new->value.function_v = function_ast->value.function_v;
          push_value(postfix_expression, new);
  
          is_last_value = true;
          is_last_minus_value = false;
          continue;
        }
        else
        {
          new = init_ast_value_stack(AST_VALUE_VARIABLE, token);
          new->value.variable_v =
            init_ast_variable(token->value, token->length);
          new->value.variable_v->ast_type = AST_VARIABLE_VALUE;
          push_value(postfix_expression, new);

          is_last_value = true;
          is_last_minus_value = false;
        }
      }
      else if (token->type == TOKEN_LPAR)
      {
        count_of_dearkelly ++;
        value_env->is_in_parentheses = true;
        push_value(stack,
            init_ast_value_stack(AST_VALUE_LPAR, token));

        is_last_value = false;
        is_last_minus_value = true;
      }
      else if (token->type == TOKEN_RPAR)
      {
        if (!count_of_dearkelly)
          break;

        count_of_dearkelly --;
        if (!count_of_dearkelly && !is_in_parentheses_save)
          value_env->is_in_parentheses = false;
        while (stack->stack->type != AST_VALUE_LPAR)
        {
          push_value(postfix_expression, pop_value(stack));
        }
        pop_value(stack);

        is_last_value = true;
        is_last_minus_value = false;
      }
      else if (is_operator(token->type))
      {
        if (token->type == TOKEN_MINUS && is_last_minus_value)
        {
          is_last_minus_value2 = true;
        }
        else
        {
          while (stack->size
              && parser_precedence(stack->stack->type)
                  >= parser_precedence(get_ast_value_type(token->type)))
          {
            push_value(postfix_expression, pop_value(stack));
          }
          push_value(stack,
              init_ast_value_stack(get_ast_value_type(token->type), token));
        }
        is_last_minus_value = false;
        is_last_value = false;
      }
      else
        break;
    }

    parser = parser_advance(parser, token->type);
    token = parser->token;
  }
  while (stack->size)
    push_value(postfix_expression, pop_value(stack));
  free(stack);

  if (postfix_expression->size == 0)
  {
    free(postfix_expression);
    return (void*) 0;
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
  Token* token = parser->token;

  orora_value_type* p = value_type_list;
  do
  {
    if (token->type == p->token_id)
      return p->parser_get_new_ast_value_stack(token, is_minus);
    p = p->next;
  } while (p);

  return (void*) 0;
}

AST* parser_get_new_int_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_INT, ast, token);
  new_ast_node->value.int_v = init_ast_int(token);

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token);
  new_ast_node->value.float_v = init_ast_float(token);

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_STRING, ast, token);
  new_ast_node->value.string_v = init_ast_string(token);

  return new_ast_node;
}

AST_value_stack* parser_get_new_int_ast_value_stack
  (Token* token, bool is_minus)
{
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_INT, token);
  AST_int* new_value = init_ast_int(token);
  if (is_minus)
    new_value->value = 0 - new_value->value;
  new->value.int_v = new_value;

  return new;
}

AST_value_stack* parser_get_new_float_ast_value_stack
  (Token* token, bool is_minus)
{
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_FLOAT, token);
  AST_float* new_value = init_ast_float(token);
  if (is_minus)
    new_value->value = 0 - new_value->value;
  new->value.float_v = new_value;

  return new;
}

AST_value_stack* parser_get_new_string_ast_value_stack
  (Token* token, bool is_minus)
{
  if (is_minus)
  {
    printf("에러, string에는 -연산이 불가함");
  }
  AST_value_stack* new =
    init_ast_value_stack(AST_VALUE_STRING, token);
  new->value.string_v = init_ast_string(token);

  return new;
}

bool is_string_ast(Token* token)
{
  if (token->type == TOKEN_STRING)
    return true;
  return false;
}

bool is_float_ast(Token* token)
{
  if (token->type == TOKEN_FLOAT)
    return true;
  return false;
}

bool is_int_ast(Token* token)
{
  if (token->type == TOKEN_INT)
    return true;
  return false;
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

int parser_precedence(int ast_stack_id)
{
  switch (ast_stack_id)
  {
    case AST_VALUE_LPAR:
      return 1;
      break;
    case AST_VALUE_PLUS:
    case AST_VALUE_MINUS:
      return 2;
      break;
    case AST_VALUE_DOT_PRODUCT:
    case AST_VALUE_PRODUCT:
    case AST_VALUE_DIV:
      return 3;
      break;
    case AST_VALUE_RPAR:
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
    case TOKEN_STAR:
    case TOKEN_SLASH:
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
    case TOKEN_MINUS:
      return AST_VALUE_MINUS;
    case TOKEN_STAR:
      return AST_VALUE_PRODUCT;
    case TOKEN_SLASH:
      return AST_VALUE_DIV;
  }

  return -1;
}
