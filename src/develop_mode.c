#ifdef DEVELOP_MODE

#include "main.h"

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
  if (checked_function->code)
  {
    printf("  ->code:\n");
    AST* ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
    ast->value.compound_v = init_ast_compound();
    ast->value.compound_v->size = 1;
    ast->value.compound_v->items[0] = checked_function->code;
    print_ast_tree(ast);
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

void print_ast_tree(AST* ast_tree)
{
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

//       default:
//         printf("에러, 테스트 용 출력에 적용 안됨\n");
//         exit(1);
//         break;
    }
  }
}

void print_file(File* file)
{
  for (int i = 0; i < file->length; i ++)
  {
    printf("%c\n", file->contents[i]);
    if (file->contents[i] == '\0')
      printf("=================\n");
  }
}

void print_tokens(Lexer* root)
{
  Token* token = (void*) 0;
  while ((token = lexer_get_token(root)) != (void*) 0)
    printf(
        "value: %s\t\t type: %d\n",
        token->value, token->type);
//   printf("%s\n", lexer_get_token(root)->value);
}

#include "./Visitor/visitor.h"
#include <string.h>
void visitor_print_function(Envs* envs, AST* ast)
{
  AST_function* ast_function = ast->value.function_v;

  if (!strcmp(ast_function->name, "print"))
  {
    for (int i = 0; i < ast_function->args_size; i ++)
    {
      AST* ast_function_arg = ast_function->args[i];
      switch (ast_function_arg->type)
      {
        case AST_VALUE:
          AST_value* ast_function_arg_value =
            ast_function_arg->value.value_v;
          if (ast_function_arg_value->size == 1)
          {
            switch (ast_function_arg_value->stack->type)
            {
              case AST_VALUE_STRING:
                printf("%s",
                    ast_function_arg_value->stack->value.
                    string_v->real_value);
                break;

              case AST_VALUE_INT:
                printf("%d",
                    ast_function_arg_value->stack->value.
                    int_v->value);
                break;

              case AST_VALUE_FLOAT:
                printf("%f",
                    ast_function_arg_value->stack->value.
                    float_v->value);
                break;

              case AST_VALUE_NULL:
                printf("(NULL)");
                break;
            }
          }
          else
          {
            AST_value_stack* new_value =
              visitor_get_value(envs, ast_function_arg_value);
            switch (new_value->type)
            {
              case AST_VALUE_STRING:
                printf("%s",
                    new_value->value.string_v->real_value);
                break;

              case AST_VALUE_INT:
                printf("%d",
                    new_value->value.int_v->value);
                break;

              case AST_VALUE_FLOAT:
                printf("%f",
                    new_value->value.float_v->value);
                break;

              case AST_VALUE_NULL:
                printf("(NULL)");
                break;
            }
          }
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

          switch (variable->type)
          {
            case ENV_VARIABLE_STRING:
              printf("%s", variable->value.string_v->real_value);
              break;

            case ENV_VARIABLE_INT:
              printf("%d", variable->value.int_v->value);
              break;

            case ENV_VARIABLE_FLOAT:
              printf("%f", variable->value.float_v->value);
              break;

            case ENV_VARIABLE_NULL:
              printf("(NULL)");
              break;
          }
          break;

        case AST_FUNCTION:
          AST_function* ast_function_arg_function =
            ast_function_arg->value.function_v;
          Env_function* function =
            visitor_get_function(envs, ast_function_arg_function);
          if (!function)
          {
            visitor_nondefine_function_error(ast_function_arg_function);
          }

          AST_value_stack* new_value =
            visitor_get_value_from_function(envs, 
                ast_function_arg_function, function);

          switch (new_value->type)
          {
            case AST_VALUE_STRING:
              printf("%s",
                  new_value->value.string_v->real_value);
              break;

            case AST_VALUE_INT:
              printf("%d",
                  new_value->value.int_v->value);
              break;

            case AST_VALUE_FLOAT:
              printf("%f",
                  new_value->value.float_v->value);
              break;

            case AST_VALUE_NULL:
              printf("(NULL)");
              break;
          }
          break;

        case AST_CODE:
        case AST_CASES:
          new_value = 
            visitor_get_value_from_ast(
                envs, 
                ast_function_arg
              );

          switch (new_value->type)
          {
            case AST_VALUE_STRING:
              printf("%s",
                  new_value->value.string_v->real_value);
              break;

            case AST_VALUE_INT:
              printf("%d",
                  new_value->value.int_v->value);
              break;

            case AST_VALUE_FLOAT:
              printf("%f",
                  new_value->value.float_v->value);
              break;

            case AST_VALUE_NULL:
              printf("(NULL)");
              break;
          }
          break;
      }
    }
  }
}

#endif

