#include "main.h"
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

static orora_value_type* value_type_list;

void init_orora()
{
  push_value_type_list(&value_type_list, "string", TOKEN_STRING);
  push_value_type_list(&value_type_list, "int", TOKEN_INT);
  push_value_type_list(&value_type_list, "float", TOKEN_FLOAT);

  orora_value_type* p = value_type_list;
  do
  {
    printf(":::%s\n", p->name);
    p = p->next;
  } while (p);
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

    Token* token = (void*) 0;
//     while ((token = lexer_get_token(root)) != (void*) 0)
//       printf(
//           "value: %s\t\t type: %d\n",
//           token->value, token->type);
//     printf("%s\n", lexer_get_token(root)->value);

    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);

    ast_tree->compound_v;
    for (int i = 0; i < ast_tree->compound_v->size; i ++)
    {
      AST* checked_ast_tree = ast_tree->compound_v->items[i];

      switch (checked_ast_tree->type)
      {
        case AST_INT:
        {
          printf("int: %d\n", checked_ast_tree->int_v->value);
        }
        break;
        case AST_FLOAT:
        {
          printf("float: %f\n", checked_ast_tree->float_v->value);
        }
        break;
        case AST_STRING:
        {
          printf("string: %s\n", checked_ast_tree->string_v->value);
        }
        break;
        case AST_VARIABLE:
        {
          printf("varialbe: %s\n", checked_ast_tree->variable_v->name);
          if (checked_ast_tree->variable_v->value == (void*) 0)
            break;
          switch (checked_ast_tree->variable_v->value->type)
          {
            case AST_INT:
              printf(
                  "\t->value: %d\n",
                  checked_ast_tree->variable_v->value->int_v->value);
              break;
            case AST_FLOAT:
              printf(
                  "\t->value: %f\n",
                  checked_ast_tree->variable_v->value->float_v->value);
              break;
            case AST_STRING:
              printf(
                  "\t->value: %s\n",
                  checked_ast_tree->variable_v->value->string_v->value);
              break;
            default: break;
          }
        }
      }
    }
  }
  return 0;
}
