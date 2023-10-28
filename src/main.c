#include "main.h"
#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
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
    while ((token = lexer_get_token(root)) != (void*) 0)
      printf(
          "value: %s\t\t col_first: %zd\t\t col: %zd\t\t row_char_first: %zd\t\t row_char: %zd\n",
          token->value, token->col_first, token->col, token->row_char_first, token->row_char);
//     printf("%s\n", lexer_get_token(root)->value);

    Parser* parser = init_parser(root);
    parser_parse(parser, root_envs);
  }
  return 0;
}
