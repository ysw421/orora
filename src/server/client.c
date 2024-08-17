#include "server/client.h"
#include "utilities/utils.h"
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

  while (running)
  {
    char* input;
    char* read;
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

    for (int i = 0; i < strlen(read); i++)
    {
      buffer[i] = read[i];
    }
    size_t num_read = strlen(read);
    buffer[num_read] = '\0';

    syslog(LOG_NOTICE, "Received data from internal client: %s", buffer);

    if (is_first_line)
    {
      input = malloc(num_read + 1);
      if (input == (void*) 0)
      {
        syslog(LOG_ERR, "Memory allocation failed");
        break;
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
        free(input);
        break;
      }
      input = temp;
      memcpy(input + len_input, buffer, num_read);
      len_input += num_read;
      input[len_input] = '\0';
    }

    char last_char = input[len_input];
    input[len_input] = '\0';
    syslog(LOG_NOTICE, "Received command: %s", input);

    off_t* len_p = (off_t*) malloc(sizeof(off_t));
    *len_p = (off_t) len_input;

    Lexer* root = init_lexer(input, len_p);

    Lexer* lexer_null_checker = malloc(sizeof(Lexer));
    memcpy(lexer_null_checker, root, sizeof(Lexer));
    if (lexer_get_token(lexer_null_checker) == (void*) 0)
    {
      free(lexer_null_checker);
      //       orora_write("", ORORA_STATUS_SUCCESS);
      is_first_line = true;
      continue;
    }
    free(lexer_null_checker);

    // #include "develop/develop_mode.h"
    //     printf("root_size: %d\n", root->size);
    //     print_tokens(root);
    int right_space = is_end_interactive_line(root);
    if (right_space == 0)
    {
      is_first_line = true;
    }
    else
    {
      len_input ++;
      input = realloc(input, len_input + 1);
      input[len_input-1] = '\n';
      input[len_input] = last_char;
      //       orora_write(int_to_string(right_space), ORORA_STATUS_MORE);
      is_more_line = true;
      //       orora_write("", ORORA_STATUS_SUCCESS);
      continue;
    }
    is_more_line = false;
    // #include "develop/develop_mode.h"
    //       print_tokens(root);
    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);

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
          fprintf(stderr, "\n");
          //                     fprintf(stderr, "%s\n", result);

          // Send the result via WebSocket
          char ws_message[BUFFER_SIZE];
          snprintf(ws_message, BUFFER_SIZE, "{\"type\":\"internal_result\",\"result\":\"%s\"}", result);
          send_ws_message(ws_message);
        }
      }
    }
    free(input);
    input = (void*) 0;
    len_input = 0;
  }

  return (void*) 0;
}

void run_client()
{
  int sock = 0;
  struct sockaddr_in serv_addr;
//   char input[BUFFER_SIZE];

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

//   if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
//   {
//     printf("\nInvalid address/ Address not supported \n");
//     return;
//   }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nORORA Server Connection Failed \n");
    return;
  }

  set_nonblocking(sock);

  fprintf(stderr, "%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
  fprintf(stderr, "Version 0.0.1\n"
    "(C) 2023 Orora Project\n");
  fprintf(stderr,"Connected to Orora Server : port %d\n", port);
  fprintf(stderr,"Type 'exit' to quit\n\n");

  rl_initialize();
  rl_bind_key('\t', custom_tab_insert);

  bool is_success_received = false;
  bool is_more_line = false;
  int right_space = 0;

  char* prompt = (char*) const_strcat(const_strcat(ORORA_COLOR_H, ORORA_PROMPT), ORORA_COLOR_RESET);
  while (1)
  {
    char* input;
    if (is_more_line)
    {
      input = readline("        ");
//       for (int i = 0; i < right_space; i++)
//         fprintf(stderr, "\t");
    }
    else
      input = readline(prompt);
    if (input == (void*) 0)
        continue;
    else if (!strcmp(input, "exit"))
    {
      fprintf(stderr, "Quitting...\n");
      exit(0);
    }
    if (*input)
        add_history(input);
//     printf(DIALOGUE);
//     fflush(stdout);
//     if (fgets(input, BUFFER_SIZE, stdin) == (void*) 0)
//       continue;

    ssize_t num_written = send(sock, input, strlen(input), 0);
    if (num_written == -1)
    {
      perror("Write to orora server failed");
      free(input);
      break;
    }

    is_success_received = false;
    is_more_line = false;
    int count = -1;
    while (!is_success_received && !is_more_line)
    {
      count++;
      MessageHeader header;
      ssize_t header_read = 0;
      while (header_read < sizeof(header))
      {
        ssize_t n = recv(sock, ((char*) &header) + header_read, sizeof(header) - header_read, 0);
        if (n > 0)
        {
          header_read += n;
        }
        else if (n == 0)
        {
          return;
        }
        else if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
          perror("Failed to read header");
          return;
        }
      }

      uint32_t status = ntohl(header.status);
      uint32_t length = ntohl(header.length);

      char* response = malloc(length + 1);
      if (!response)
      {
        perror("Failed to allocate memory for response");
        return;
      }

      ssize_t total_read = 0;
      while (total_read < length)
      {
        ssize_t n = recv(sock, response + total_read, length - total_read, 0);
        if (n > 0)
        {
          total_read += n;
        }
        else if (n == 0)
        {
          free(response);
          return;
        }
        else if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
          perror("Failed to read message");
          free(response);
          return;
        }
      }

      response[length] = '\0';

      if (status == ORORA_STATUS_SUCCESS)
      {
        is_success_received = true;
        if (strlen(response) == 0 && count > 0)
          fprintf(stderr, "\n");
        else
          fprintf(stderr, "%s", response);
      }
      else if (status == ORORA_STATUS_MORE)
      {
        is_more_line = true;
        right_space = atoi(response);
        if (right_space < 0)
          right_space = 0;
      }
      else if (status == ORORA_STATUS_PROGRESS)
      {
      }
      else
      {
        fprintf(stderr, "%s", response);
      }

      free(response);
    }
    free(input);
  }

  rl_clear_history();
  close(sock);
}

