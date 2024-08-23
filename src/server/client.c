#include "server/client.h"
#include "utilities/utils.h"
#include "loader/error_log.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int custom_tab_insert(int count, int key)
{
  rl_insert_text("\t");
  return 0;
}

void set_nonblocking(int sock)
{
  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

jmp_buf env;

void segfault_handler(int signal) 
{
  orora_error("에러, 메모리 문제", (void*) 0);
  longjmp(env, 1);
}

extern sig_atomic_t running;
extern jmp_buf interactive_mode_buf;
extern Envs* root_envs;
extern int port;

void* internal_client_thread(void* arg)
{
  fprintf(stderr, "%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
  fprintf(stderr, "Version 0.0.1\n"
      "(C) 2023 Orora Project\n");
  //   fprintf(stderr,"Connected to Orora Server : port %d\n", port);
  fprintf(stderr,"Type 'exit' to quit\n\n");

  char buffer[BUFFER_SIZE];
  size_t len_input = 0;
  bool is_first_line = true;
  bool is_more_line = false;
  char* prompt = (char*) const_strcat(const_strcat(ORORA_COLOR_H, ORORA_PROMPT), ORORA_COLOR_RESET);

  //   while (running)
  //   {
  //     char* input;
  //     char* read;
  //     if (is_more_line)
  //     {
  //       read = readline("        ");
  //     }
  //     else
  //       read = readline(prompt);
  //     if (read == (void*) 0)
  //       continue;
  //     else if (!strcmp(read, "exit"))
  //     {
  //       fprintf(stderr, "Quitting...\n");
  //       exit(0);
  //     }
  //     if (*read)
  //       add_history(read);
  // 
  //     for (int i = 0; i < strlen(read); i++)
  //     {
  //       buffer[i] = read[i];
  //     }
  //     size_t num_read = strlen(read);
  //     buffer[num_read] = '\0';
  // 
  //     syslog(LOG_NOTICE, "Received data from internal client: %s", buffer);
  // 
  //     if (is_first_line)
  //     {
  //       input = malloc(num_read + 1);
  //       if (input == (void*) 0)
  //       {
  //         syslog(LOG_ERR, "Memory allocation failed");
  //         break;
  //       }
  //       memcpy(input, buffer, num_read);
  //       len_input = num_read;
  //       input[len_input] = '\0';
  //       is_first_line = false;
  //     }
  //     else
  //     {
  //       char *temp = realloc(input, len_input + num_read + 1);
  //       if (temp == (void*) 0)
  //       {
  //         syslog(LOG_ERR, "Memory reallocation failed");
  //         free(input);
  //         break;
  //       }
  //       input = temp;
  //       memcpy(input + len_input, buffer, num_read);
  //       len_input += num_read;
  //       input[len_input] = '\0';
  //     }
  // 
  //     char last_char = input[len_input];
  //     input[len_input] = '\0';
  //     syslog(LOG_NOTICE, "Received command: %s", input);
  // 
  //     off_t* len_p = (off_t*) malloc(sizeof(off_t));
  //     *len_p = (off_t) len_input;
  // 
  //     Lexer* root = init_lexer(input, len_p);
  // 
  //     Lexer* lexer_null_checker = malloc(sizeof(Lexer));
  //     memcpy(lexer_null_checker, root, sizeof(Lexer));
  //     if (lexer_get_token(lexer_null_checker) == (void*) 0)
  //     {
  //       free(lexer_null_checker);
  //       //       orora_write("", ORORA_STATUS_SUCCESS);
  //       is_first_line = true;
  //       continue;
  //     }
  //     free(lexer_null_checker);
  // 
  //     // #include "develop/develop_mode.h"
  //     //     printf("root_size: %d\n", root->size);
  //     //     print_tokens(root);
  //     int right_space = is_end_interactive_line(root);
  //     if (right_space == 0)
  //     {
  //       is_first_line = true;
  //     }
  //     else
  //     {
  //       len_input ++;
  //       input = realloc(input, len_input + 1);
  //       input[len_input-1] = '\n';
  //       input[len_input] = last_char;
  //       //       orora_write(int_to_string(right_space), ORORA_STATUS_MORE);
  //       is_more_line = true;
  //       //       orora_write("", ORORA_STATUS_SUCCESS);
  //       continue;
  //     }
  //     is_more_line = false;
  //     // #include "develop/develop_mode.h"
  //     //       print_tokens(root);
  //     Parser* parser = init_parser(root);
  //     AST* ast_tree = parser_parse(parser);
  // 
  //     if (setjmp(interactive_mode_buf) == 0)
  //     {
  //       AST* ast = ast_tree->value.compound_v->items[0];
  //       AST_value_stack* new_value = visitor_visit(root_envs, ast)->output;
  //       if (new_value)
  //       {
  //         if (new_value->type != AST_VALUE_NULL || !(ast->type == AST_FUNCTION
  //               || ast->type == AST_CODE
  //               || ast->type == AST_FOR
  //               || ast->type == AST_WHILE
  //               || ast->type == AST_CASES
  //               || (ast->type == AST_VALUE && (ast->value.value_v->stack->type == AST_VALUE_CODE)
  //                 )
  //               )
  //            )
  //         {
  //           const char* result = visitor_print_function_value(new_value);
  //           //                     fprintf(stderr, "%s\n", result);
  // 
  //           char ws_message[BUFFER_SIZE];
  //           snprintf(ws_message, BUFFER_SIZE, "{\"type\":\"internal_result\",\"result\":\"%s\"}", result);
  //           send_ws_message(ws_message);
  //         }
  //         fprintf(stderr, "\n");
  //       }
  //     }
  //     free(input);
  //     input = (void*) 0;
  //     len_input = 0;
  //   }

  signal(SIGSEGV, segfault_handler);

  char* input = (void*) 0;
  while (running)
  {
    if (setjmp(env) == 0)
    {
      char* read;
      size_t num_read;

      // Error handling label
loop_start:

      if (is_more_line)
      {
        read = readline("        ");
      }
      else
        read = readline(prompt);

      if (read == (void*) 0)
        continue;
      else if (!strcmp(read, "exit"))
      {
        fprintf(stderr, "Quitting...\n");
        exit(0);
      }
      if (*read)
        add_history(read);

      num_read = strlen(read);
      if (num_read >= BUFFER_SIZE) {
        syslog(LOG_ERR, "Input too long");
        free(read);
        goto loop_start;
      }

      strncpy(buffer, read, num_read);
      buffer[num_read] = '\0';

      syslog(LOG_NOTICE, "Received data from internal client: %s", buffer);

      if (is_first_line)
      {
        input = malloc(num_read + 1);
        if (input == (void*) 0)
        {
          syslog(LOG_ERR, "Memory allocation failed");
          free(read);
          goto loop_start;
        }
        memcpy(input, buffer, num_read);
        len_input = num_read;
        input[len_input] = '\0';
        is_first_line = false;
      }
      else
      {
        char *temp = realloc(input, len_input + num_read + 1);
        if (temp == (void*) 0)
        {
          syslog(LOG_ERR, "Memory reallocation failed");
//           free(input);
          free(read);
          goto loop_start;
        }
        input = temp;
        memcpy(input + len_input, buffer, num_read);
        len_input += num_read;
        input[len_input] = '\0';
      }

      char last_char = input[len_input - 1];
      input[len_input] = '\0';
      syslog(LOG_NOTICE, "Received command: %s", input);

      off_t* len_p = (off_t*) malloc(sizeof(off_t));
      if (len_p == (void*) 0) 
      {
        syslog(LOG_ERR, "Memory allocation failed for len_p");
//         free(input);
        free(read);
        goto loop_start;
      }
      *len_p = (off_t) len_input;

      Lexer* root = init_lexer(input, len_p);
      if (root == (void*) 0)
      {
        syslog(LOG_ERR, "Lexer initialization failed");
//         free(input);
        free(read);
        free(len_p);
        goto loop_start;
      }
      int right_space = is_end_interactive_line(root);

      Lexer* lexer_null_checker = malloc(sizeof(Lexer));
      if (lexer_null_checker == (void*) 0)
      {
        syslog(LOG_ERR, "Memory allocation failed for lexer_null_checker");
//         free(input);
        free(read);
        free(len_p);
        free(root);
        goto loop_start;
      }
      memcpy(lexer_null_checker, root, sizeof(Lexer));
      if (lexer_get_token(lexer_null_checker) == (void*) 0)
      {
        free(lexer_null_checker);
//         free(input);
        free(read);
        free(len_p);
        free(root);
        is_first_line = true;
        goto loop_start;
      }
      free(lexer_null_checker);

      if (right_space == 0)
      {
        is_first_line = true;
      }
      else
      {
        len_input++;
        is_more_line = true;
        char *temp = realloc(input, len_input + 1);
        if (temp == (void*) 0) 
        {
          syslog(LOG_ERR, "Memory reallocation failed");
//           free(input);
          free(read);
          free(len_p);
          free(root);
          goto loop_start;
        }
        input = temp;
        input[len_input-1] = '\n';
        input[len_input] = last_char;
        free(read);
        free(len_p);
        free(root);
        continue;
      }
      is_more_line = false;

      Parser* parser = init_parser(root);
      if (parser == (void*) 0) 
      {
        syslog(LOG_ERR, "Parser initialization failed");
//         free(input);
        free(read);
        free(len_p);
        free(root);
        goto loop_start;
      }

      AST* ast_tree = parser_parse(parser);
      if (ast_tree == (void*) 0) 
      {
        syslog(LOG_ERR, "Parsing failed");
//         free(input);
        free(read);
        free(len_p);
        free(root);
        free(parser);
        goto loop_start;
      }

      if (setjmp(interactive_mode_buf) == 0)
      {
        AST* ast = ast_tree->value.compound_v->items[0];
        AST_value_stack* new_value = visitor_visit(root_envs, ast)->output;
        if (new_value)
        {
          if (new_value->type != AST_VALUE_NULL || !(ast->type == AST_FUNCTION
                || ast->type == AST_CODE
                || ast->type == AST_FOR
                || ast->type == AST_WHILE
                || ast->type == AST_CASES
                || (ast->type == AST_VALUE && (ast->value.value_v->stack->type == AST_VALUE_CODE)
                  )
                )
             )
          {
            const char* result = visitor_print_function_value(new_value);

            char ws_message[BUFFER_SIZE];
            snprintf(ws_message, BUFFER_SIZE, "{\"type\":\"internal_result\",\"result\":\"%s\"}", result);
            send_ws_message(ws_message);
          }
          fprintf(stderr, "\n");
        }
      }

//       free(input);
      free(read);
      free(len_p);
      free(root);
      free(parser);
      free(ast_tree);
//       input = (void*) 0;
      len_input = 0;
    }
  }
  input = (void*) 0;

  return (void*) 0;
}

