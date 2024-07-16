#include "server/client.h"

void run_client(int write_fd, int read_fd) {
  char input[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  bool is_quit = false;
  while (1)
  {
    printf(DIALOGUE);
    fflush(stdout);
    if (fgets(input, BUFFER_SIZE, stdin) == (void*) 0)
      break;

    if (strcmp(input, "quit\n") == 0)
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

    ssize_t num_read = read(read_fd, response, BUFFER_SIZE - 1);
    if (num_read >= 0)
    {
      response[num_read] = '\0';
      printf("%s", response);
    }
    else
    {
      perror("Read from orora server failed");
      break;
    }
  }
  if (!is_quit)
    printf("\n");
}

