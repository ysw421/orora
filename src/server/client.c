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

void run_client(int port)
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

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nORORA Server Connection Failed \n");
    return;
  }

  set_nonblocking(sock);

  printf("%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
  printf("Version 0.0.1\n"
    "(C) 2023 Orora Project\n\n");

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
    else if (!strcmp(input, "exit()\n"))
    {
      printf("Quitting...\n");
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
      else
        fprintf(stderr, "%s", response);

      free(response);
    }
    free(input);
  }

  rl_clear_history();
  close(sock);
}

