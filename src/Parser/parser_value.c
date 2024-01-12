#include "parser_id.h"
#include "../main.h"
#include <stdlib.h>

AST_value_stack* get_single_value(Parser* parser, AST* ast);

GET_VALUE_ENV* init_get_value_env()
{
  GET_VALUE_ENV* new_env = malloc(sizeof(GET_VALUE_ENV));
  new_env->is_in_parentheses = false;

  return new_env;
}

int parser_precedence(Token* token, AST_value_stack* stack)
{
  if (token)
  {}
  else
  {}
  switch (token->type)
  {
//     case :
  }
  
  return 0;
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

AST* parser_get_value(Parser* parser, AST* ast, GET_VALUE_ENV* value_env)
{
  AST_value* stack = init_ast_value();
  AST_value* postfix_expression = init_ast_value();
  bool is_last_value = false;       // 3x -> 3 * x
  Token* token = parser->token;

  AST_value_stack* save_value;

  while (token != (void*) 0)
  {
    bool is_single_value = false;
    orora_value_type* p = value_type_list;
    do
    {
      printf(":::%s\n", p->name);
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
            && parser_precedence((void*) 0, stack->stack))
        {
          save_value = pop_value(stack);
          push_value(postfix_expression, save_value);
        }
        push_value(postfix_expression,
            init_ast_value_stack(AST_VALUE_PRODUCT, token));
      }
      save_value = get_single_value(parser, ast);
    }

    // For develop
    parser_advance(parser, token->type);
    token = parser->token;
    // end for
  }

  free(value_env);
  return (void*) 0;
}

AST_value_stack* get_single_value(Parser* parser, AST* ast)
{
  Token* token = parser->token;

  orora_value_type* p = value_type_list;
  do
  {
    if (token->type == p->token_id)
    {
      return p->parser_get_new_ast(ast, token);
//       ast_compound_add(ast->compound_v, p->parser_get_new_ast(ast, token));
//       parser = parser_advance(parser, p->token_id);
//       token = parser->token;

      break;
    }
    p = p->next;
  } while (p);

  return (void*) 0;
}

AST* parser_get_new_int_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_INT, ast, token);
  new_ast_node->int_v = init_ast_int(atoi(token->value));

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token);
  new_ast_node->float_v = init_ast_float(atof(token->value));

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_STRING, ast, token);
  new_ast_node->string_v = init_ast_string(token->value);

  return new_ast_node;
}
