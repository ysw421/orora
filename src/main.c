#include "main.h"
#include "file.h"
#include "Lexer/lexer.h"
#include "Parser/parser_value.h"
#include "env.h"
#include "Visitor/visitor.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef DEVELOP_MODE
void print_variable(AST_variable* node);
void print_value(AST* ast);
void print_function(AST_function* checked_function);
#endif

orora_value_type* value_type_list;

void init_orora()
{
  push_value_type_list
    (
     &value_type_list, "string", TOKEN_STRING, parser_get_new_string_ast,
     parser_get_new_string_ast_value_stack, is_string_ast, AST_STRING,
     ENV_VARIABLE_STRING, visitor_set_env_variable_string, AST_VALUE_STRING
    );
  push_value_type_list
    (
     &value_type_list, "int", TOKEN_INT, parser_get_new_int_ast,
     parser_get_new_int_ast_value_stack, is_int_ast, AST_INT,
     ENV_VARIABLE_INT, visitor_set_env_variable_int, AST_VALUE_INT
    );
  push_value_type_list
    (
     &value_type_list, "float", TOKEN_FLOAT, parser_get_new_float_ast,
     parser_get_new_float_ast_value_stack, is_float_ast, AST_FLOAT,
     ENV_VARIABLE_FLOAT, visitor_set_env_variable_float, AST_VALUE_FLOAT
    );
}

int main(int argc, char** argv)
{
  init_orora();

#ifndef GLOBAL_ENV_DEFINE
#define GLOBAL_ENV_DEFINE
  Env* global_env = init_env();
  Envs* root_envs = init_envs(global_env, init_env());
#endif

  if (argc == 2)
  {
    File* file = openfile(argv[1]);

#ifdef DEVELOP_MODE
//     for (int i = 0; i < file->length; i ++)
//     {
//       printf("%c\n", file->contents[i]);
//       if (file->contents[i] == '\0')
//         printf("=================\n");
//     }
#endif

    Lexer* root = init_lexer(file->contents, &file->length);

#ifdef DEVELOP_MODE
//     Token* token = (void*) 0;
//     while ((token = lexer_get_token(root)) != (void*) 0)
//       printf(
//           "value: %s\t\t type: %d\n",
//           token->value, token->type);
//     printf("%s\n", lexer_get_token(root)->value);
#endif

    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);
    
    printf("======================\n");

    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
    {
      visitor_visit(root_envs, ast_tree->value.compound_v->items[i]);
    }
    printf("\n======================\n");

#ifdef DEVELOP_MODE
    // For Develop....
    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
    {
      AST* checked_ast_tree = ast_tree->value.compound_v->items[i];

      switch (checked_ast_tree->type)
      {
        case AST_VALUE:
          print_value(checked_ast_tree);
          break;
        case AST_FUNCTION:
          print_function(checked_ast_tree->value.function_v);
          break;
        case AST_VARIABLE:
          print_variable(checked_ast_tree->value.variable_v);
          break;
      }
    }
#endif
  }
  return 0;
}


#ifdef DEVELOP_MODE
void print_function(AST_function* checked_function)
{
  printf("function: %s\n", checked_function->name);
  printf("  ->arguments:\n");
  for (int i = 0; i < checked_function->args_size; i ++)
  {
    switch (checked_function->args[i]->type)
    {
      case AST_VALUE:
        printf("    ->");
        print_value(checked_function->args[i]);
        printf("\n");
        break;
      case AST_VARIABLE:
        printf("    ->");
        print_variable(checked_function->args[i]->value.variable_v);
        printf("\n");
        break;
    }
  }
  if (checked_function->codes_size)
  {
    printf("  ->code:\n");
    if (checked_function->codes_size == 1)
      print_value(checked_function->codes[0]);
  }
}

void print_value(AST* ast)
{
  AST_value* checked = ast->value.value_v;
  printf("value: ->size: %ld\n", checked->size);
  struct ast_value_stack_t* stack = checked->stack;
  
  for (int i = 0; i < checked->size; i ++)
  {
    switch (stack->type)
    {
      case AST_VALUE_INT:
        printf("\t->int: %d\n", stack->value.int_v->value);
        break;
      case AST_VALUE_FLOAT:
        printf("\t->float: %f\n", stack->value.float_v->value);
        break;
      case AST_VALUE_STRING:
        printf("\t->string: %s\n", stack->value.string_v->real_value);
        break;
      case AST_VALUE_PLUS:
        printf("\t->+\n");
        break;
      case AST_VALUE_MINUS:
        printf("\t->-\n");
        break;
      case AST_VALUE_PRODUCT:
        printf("\t->*\n");
        break;
      case AST_VALUE_DIV:
        printf("\t->/\n");
        break;
      case AST_VALUE_VARIABLE:
        printf("    ->");
        print_variable(stack->value.variable_v);
        printf("\n");
        break;
      case AST_VALUE_FUNCTION:
        printf("    ->");
        print_function(stack->value.function_v);
        printf("\n");
        break;
      default:
        printf("\t->unkwon type: %d\n", stack->type);
        break;
    }

    stack = stack->next;
  }
}
 
void print_variable(AST_variable* node)
{
  printf("variable: %s\n", node->name);
  if (node->value)
  {
    switch (node->value->type)
    {
      case AST_VALUE:
        printf("    ->in variable");
        print_value(node->value);
        break;
      case AST_VARIABLE:
        AST* p = node->value;
        do
        {
          switch (p->type)
          {
            case AST_VALUE:
              printf("    ->in variable");
              print_value(p);
              printf("\n");
              break;
            case AST_VARIABLE:
              printf("\t->variable: %s\n",
                      p->value.variable_v->name);
              break;
          }
          if (p->type != AST_VARIABLE)
            break;
          p = p->value.variable_v->value;
        } while (p);
        break;
      default: break;
    }
  }
  if (node->satisfy_size)
  {
    printf("    ->satisfy\n");
    for (int i = 0; i < node->satisfy_size; i ++)
    {
      printf("    ->condition: ");
      print_value(node->satisfy[i]);
    }
  }
}
#endif
