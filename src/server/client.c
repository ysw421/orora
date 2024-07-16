#include "server/client.h"

void run_client() {
  char input[BUFFER_SIZE];
  
  while (1) {
    printf(DIALOGUE);
    fflush(stdout);
    if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
      break;
    }
    
    if (strcmp(input, "quit\n") == 0) {
      break;
    }
    
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
      perror("Failed to open FIFO");
      continue;
    }
    
    write(fd, input, strlen(input));
    close(fd);
    
    sleep(1);
    FILE *result_file = fopen(RESULT_FILE, "r");
    if (result_file != NULL) {
      char result[BUFFER_SIZE];
      while (fgets(result, BUFFER_SIZE, result_file) != NULL) {
        printf("%s", result);
      }
      fclose(result_file);
    } else {
      printf("Command execution failed\n");
    }
  }
}

