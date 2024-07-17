#include "server/client.h"

void run_client(int write_fd, int read_fd)
{
  printf("%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
  printf("Version 0.0.1\n"
      "(C) 2023 Orora Project\n\n");

  char input[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  bool is_quit = false;
  while (1)
  {
    printf(DIALOGUE);
    fflush(stdout);
    if (fgets(input, BUFFER_SIZE, stdin) == (void*) 0)
      break;

    if (strcmp(input, "exit\n") == 0)
    {
      printf("Quitting...\n");
      is_quit = true;
      break;
    }

    ssize_t num_written = write(write_fd, input, strlen(input));
    if (num_written == -1)
    {
      perror("Write to orora server failed");
      break;
    }

    ssize_t num_read;
    response[0] = '\0';

    bool is_error = false;
    while (strncmp(response, CODE_SUCCESS, 3))
    {
      num_read = read(read_fd, response, BUFFER_SIZE - 1);
      if (num_read >= 0)
      {
        response[num_read] = '\0';
        size_t response_len = strlen(response);
        if (response_len < 3)
        {
          perror("Read from orora server failed");
          is_error = true;
          break;
        }

        memmove(response, response + 3, strlen(response) - 2);
        printf("%s", response);
        break;
      }
      else
      {
        perror("Read from orora server failed");
        is_error = true;
        break;
      }
    }
    if (is_error)
      break;
  }
  if (!is_quit)
    printf("\n");
}

