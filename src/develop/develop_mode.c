#include "develop/develop_mode.h"
#include "utilities/utils.h"
#include "loader/error_log.h"

void print_function(AST_function* checked_function)
{
  fprintf(stderr, "function: %s\n", checked_function->name);
  fprintf(stderr, "  ->arguments:\n");
  for (int i = 0; i < checked_function->args_size; i ++)
  {
    if (checked_function->args[i]->type)
    {
      fprintf(stderr, "    ->");
      print_value(checked_function->args[i]);
      fprintf(stderr, "\n");
    }
    else if (checked_function->args[i]->type)
    {
      fprintf(stderr, "    ->");
      print_variable(checked_function->args[i]->value.variable_v);
      fprintf(stderr, "\n");
    }
  }
  if (checked_function->code)
  {
    fprintf(stderr, "  ->code:\n");
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
  fprintf(stderr, "value: ->size: %ld\n", checked->size);
  struct ast_value_stack_t* stack = checked->stack;
  
  for (int i = 0; i < checked->size; i ++)
  {
    switch (stack->type)
    {
      case AST_VALUE_INT:
        fprintf(stderr, "\t->int: %d\n", stack->value.int_v->value);
        break;
      case AST_VALUE_FLOAT:
        fprintf(stderr, "\t->float: %f\n", stack->value.float_v->value);
        break;
      case AST_VALUE_STRING:
        fprintf(stderr, "\t->string: %s\n", stack->value.string_v->real_value);
        break;
      case AST_VALUE_PLUS:
        fprintf(stderr, "\t->+\n");
        break;
      case AST_VALUE_MINUS:
        fprintf(stderr, "\t->-\n");
        break;
      case AST_VALUE_PRODUCT:
        fprintf(stderr, "\t->*\n");
        break;
      case AST_VALUE_DOT_PRODUCT:
        fprintf(stderr, "\t->dot\n");
        break;
      case AST_VALUE_DIV:
        fprintf(stderr, "\t->/\n");
        break;
      case AST_VALUE_VARIABLE:
        fprintf(stderr, "    ->");
        print_variable(stack->value.variable_v);
        fprintf(stderr, "\n");
        break;
      case AST_VALUE_FUNCTION:
        fprintf(stderr, "    ->");
        print_function(stack->value.function_v);
        fprintf(stderr, "\n");
        break;
      case AST_VALUE_NULL:
        fprintf(stderr, "\t->null\n");
        break;
      case AST_VALUE_BOOL:
        fprintf(stderr, "\t->bool: %s\n", stack->value.bool_v->value ? "true" : "false");
        break;
      case AST_VALUE_MATRIX:
        fprintf(stderr, "\t->matrix\n");
        break;
      case AST_VALUE_CIRCUMFLEX:
        fprintf(stderr, "\t->^\n");
        break;
      case AST_VALUE_MOD:
        fprintf(stderr, "\t->mod\n");
        break;
      case AST_VALUE_COMMA:
        fprintf(stderr, "\t->,\n");
        break;
      case AST_VALUE_UNDER:
        fprintf(stderr, "\t->_\n");
        break;

      default:
        fprintf(stderr, "\t->unkwon type: %d\n", stack->type);
        break;
    }

    stack = stack->next;
  }
}
 
void print_variable(AST_variable* node)
{
  fprintf(stderr, "variable: %s\n", node->name);
  if (node->value)
  {
    switch (node->value->type)
    {
      case AST_VALUE:
        fprintf(stderr, "    ->in variable");
        print_value(node->value);
        break;
      case AST_VARIABLE:
        AST* p = node->value;
        do
        {
          if (p->type)
          {
            fprintf(stderr, "    ->in variable");
            print_value(p);
            fprintf(stderr, "\n");
          }
          else if (p->type)
          {
            fprintf(stderr, "\t->variable: %s\n",
                    p->value.variable_v->name);
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
    fprintf(stderr, "    ->satisfy\n");
    for (int i = 0; i < node->satisfy_size; i ++)
    {
      fprintf(stderr, "    ->condition: ");
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

      default:
        orora_error("에러, 테스트 용 출력에 적용 안됨", (void*) 0);
        // fprintf(stderr, "에러, 테스트 용 출력에 적용 안됨\n");
        exit(1);
        break;
    }
  }
}

void print_file(File* file)
{
  for (int i = 0; i < file->length; i ++)
  {
    fprintf(stderr, "%c\n", file->contents[i]);
    if (file->contents[i] == '\0')
      fprintf(stderr, "=================\n");
  }
}

void print_tokens(Lexer* root)
{
  Token* token = (void*) 0;
  while ((token = lexer_get_token(root)) != (void*) 0)
    fprintf(stderr, "value: %s\t\t type: %d\n", token->value, token->type);
}

