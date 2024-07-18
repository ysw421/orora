#include "loader/main.h"
#include "filesys/file.h"
#include "lexer/lexer.h"
#include "parser/parser_value.h"
#include "loader/env.h"
#include "loader/color.h"
#include "visitor/visitor.h"
#include "loader/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <netdb.h>

#ifdef DEVELOP_MODE
#include "develop/develop_mode.h"
#endif

#define MAX_RETRY 5
#define START_PORT 8080

int ORORA_PORT;

int create_and_bind_socket(int *port)
{
  int server_socket, opt = 1;
  struct sockaddr_in server_addr;

  for (int i = 0; i < MAX_RETRY; i++)
  {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
      perror("Socket creation failed");
      continue;
    }

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
      perror("setsockopt(SO_REUSEADDR) failed");
      close(server_socket);
      continue;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(*port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
      if (errno == EADDRINUSE)
      {
        printf("Port %d is already in use. Trying next port.\n", *port);
        (*port)++;
        close(server_socket);
        continue;
      }
      else {
        perror("Bind failed");
        close(server_socket);
        return -1;
      }
    }

    return server_socket;
  }
  printf("Failed to find an available port after %d attempts.\n", MAX_RETRY);
  return -1;
}

orora_value_type* value_type_list;
int ORORA_VALUE_TYPE_NUM;
bool init_orora();

void handle_sigint(int sig)
{
  fprintf(stderr, "\n");
  fprintf(stderr, DIALOGUE);
}

void handle_sigterm(int sig)
{
  fprintf(stderr, "\n");
}

bool INTERACTIVE_MODE = 0;

void change_interactive_mode(bool mode)
{
  INTERACTIVE_MODE = mode;
}

int main(int argc, char** argv)
{
  if (!init_orora())
    return 1;

#ifndef GLOBAL_ENV_DEFINE
#define GLOBAL_ENV_DEFINE
  Envs* global_env = init_envs((void*) 0, init_env());
  Envs* root_envs = init_envs(global_env, init_env());
#endif

  if (argc == 2)
  {
    File* file = file_open(argv[1]);

#ifdef DEVELOP_MODE
//     print_file(file);
#endif

    Lexer* root = init_lexer(file->contents, &file->length);
    free(file);

#ifdef DEVELOP_MODE
//     print_tokens(root);
#endif

    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);

#ifdef DEVELOP_MODE
//     print_ast_tree(ast_tree);
#endif

    for (int i = 0; i < ast_tree->value.compound_v->size; i ++)
      visitor_visit(root_envs, ast_tree->value.compound_v->items[i]);
    printf("\n");

#ifdef DEVELOP_MODE
//     print_ast_tree(ast_tree);
#endif

    free(root);
    free(parser);
    free(ast_tree);
  }
  else if (argc == 1)
  {
    change_interactive_mode(1);

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigterm);
    fflush(stdout);

    int port = START_PORT;
    int server_socket = create_and_bind_socket(&port);
    if (server_socket == -1)
      exit(EXIT_FAILURE);

    if (listen(server_socket, 1) < 0)
    {
      perror("Listen failed");
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0)
    {
      perror("Fork failed");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
      struct sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);
      int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
      if (client_socket < 0)
      {
        perror("Accept failed");
        exit(EXIT_FAILURE);
      }

      dup2(client_socket, STDIN_FILENO);
      dup2(client_socket, STDOUT_FILENO);
      
      close(server_socket);
      close(client_socket);
      
      run_daemon();
      exit(0);
    }
    else
    {
      close(server_socket);
      ORORA_PORT = port;
      run_client(port);
    }
  }

  free(global_env);
  free(root_envs);

  return 0;
}


bool init_orora()
{
  /* string type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_string_ast,
     parser_get_new_string_ast_value_stack, is_string_ast, AST_STRING,
     ENV_VARIABLE_STRING, AST_VALUE_STRING,
     visitor_set_value_Env_variable_from_AST_value_stack_string,
     visitor_set_value_AST_value_stack_from_Env_variable_string
    );
  /* int type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_int_ast,
     parser_get_new_int_ast_value_stack, is_int_ast, AST_INT,
     ENV_VARIABLE_INT, AST_VALUE_INT,
     visitor_set_value_Env_variable_from_AST_value_stack_int,
     visitor_set_value_AST_value_stack_from_Env_variable_int
     );
  /* float type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_float_ast,
     parser_get_new_float_ast_value_stack, is_float_ast, AST_FLOAT,
     ENV_VARIABLE_FLOAT, AST_VALUE_FLOAT,
     visitor_set_value_Env_variable_from_AST_value_stack_float,
     visitor_set_value_AST_value_stack_from_Env_variable_float
    );
  /* null type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_null_ast,
     parser_get_new_null_ast_value_stack, is_null_ast, AST_NULL,
     ENV_VARIABLE_NULL, AST_VALUE_NULL,
     visitor_set_value_Env_variable_from_AST_value_stack_null,
     visitor_set_value_AST_value_stack_from_Env_variable_null
    );
  /* bool type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_bool_ast, 
     parser_get_new_bool_ast_value_stack, is_bool_ast, AST_BOOL, 
     ENV_VARIABLE_BOOL, AST_VALUE_BOOL, 
     visitor_set_value_Env_variable_from_AST_value_stack_bool, 
     visitor_set_value_AST_value_stack_from_Env_variable_bool
    );
  /* matrix type */
  push_value_type_list
    (
     &value_type_list, parser_get_new_matrix_ast, 
     parser_get_new_matrix_ast_value_stack, is_matrix_ast, AST_MATRIX, 
     ENV_VARIABLE_MATRIX, AST_VALUE_MATRIX, 
     visitor_set_value_Env_variable_from_AST_value_stack_matrix, 
     visitor_set_value_AST_value_stack_from_Env_variable_matrix
    );
  ORORA_VALUE_TYPE_NUM = 6;

  return true;
}
