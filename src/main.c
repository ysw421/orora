#include "main.h"
#include "file.h"
#include "Lexer/lexer.h"
#include "Parser/parser_value.h"
#include "env.h"
#include "Visitor/visitor.h"
#include <stdio.h>
#include <stdlib.h>

void print_variable(AST_variable* node);

    void print_value(AST* ast)
    {
      AST_value* checked = ast->value_v;
      printf("value: ->size: %ld\n", checked->size);
      struct ast_value_stack_t* stack = checked->stack;
      for (int i = 0; i < checked->size; i ++)
      {
        switch (stack->type)
        {
          case AST_VALUE_INT:
            printf("\t->int: %d\n", stack->value.int_v->value);
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
      if (node->value == (void*) 0)
        return;
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
                break;
              case AST_VARIABLE:
                printf("\t->variable: %s\n",
                        p->variable_v->name);
                break;
            }
            if (p->type != AST_VARIABLE)
              break;
            p = p->variable_v->value;
          } while (p);
          break;
        default: break;
      }
    }

orora_value_type* value_type_list;

void init_orora()
{
  push_value_type_list
    (
     &value_type_list, "string", TOKEN_STRING, parser_get_new_string_ast,
     parser_get_new_string_ast_value_stack
    );
  push_value_type_list
    (
     &value_type_list, "int", TOKEN_INT, parser_get_new_int_ast,
     parser_get_new_int_ast_value_stack
    );
  push_value_type_list
    (
     &value_type_list, "float", TOKEN_FLOAT, parser_get_new_float_ast,
     parser_get_new_float_ast_value_stack
    );
}

int main(int argc, char** argv)
{
  init_orora();

#ifndef GLOBAL_ENV_DEFINE
#define GLOBAL_ENV_DEFINE
  Env* global_env = init_env();
  Envs* root_envs = init_envs(global_env, (void*) 0);
#endif

  if (argc == 2)
  {
    File* file = openfile(argv[1]);

//     for (int i = 0; i < file->length; i ++)
//     {
//       printf("%c\n", file->contents[i]);
//       if (file->contents[i] == '\0')
//         printf("=================\n");
//     }

    Lexer* root = init_lexer(file->contents, &file->length);

//     Token* token = (void*) 0;
//     while ((token = lexer_get_token(root)) != (void*) 0)
//       printf(
//           "value: %s\t\t type: %d\n",
//           token->value, token->type);
//     printf("%s\n", lexer_get_token(root)->value);

    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);
    
    printf("======================\n");

    for (int i = 0; i < ast_tree->compound_v->size; i ++)
    {
      visitor_visit(global_env, ast_tree->compound_v->items[i]);
    }
    printf("\n======================\n");

    // For Develop....
    for (int i = 0; i < ast_tree->compound_v->size; i ++)
    {
      AST* checked_ast_tree = ast_tree->compound_v->items[i];

      switch (checked_ast_tree->type)
      {
        case AST_VALUE:
          print_value(checked_ast_tree);
          break;
        case AST_FUNCTION:
        {
          printf("function: %s\n", checked_ast_tree->function_v->name);
          AST_function* checked_function = checked_ast_tree->function_v;
          printf("arguments:\n");
          for (int i = 0; i < checked_function->args_size; i ++)
          {
            switch (checked_function->args[i]->type)
            {
              case AST_INT:
                printf("\t->value: %d\n", checked_function->args[i]->int_v->value);
                break;
              case AST_STRING:
                printf("\t->value: %s\n", checked_function->args[i]->string_v->value);
                break;
              case AST_VARIABLE:
                printf("\t->variable: %s\n",
                    checked_function->args[i]->variable_v->name);
//                 printf("\t\t->value: %d\n",
//                     checked_function->args[i]->variable_v->value->int_v->value);
                break;
            }
          }
        }
        break;
        case AST_VARIABLE:
          print_variable(checked_ast_tree->variable_v);
          break;
      }
    }
  }
  return 0;
}
