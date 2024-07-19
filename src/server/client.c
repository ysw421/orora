#include "server/client.h"
#include "utilities/utils.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

// #define BUFFER_SIZE 1024

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
    printf("\nConnection Failed \n");
    return;
  }

  set_nonblocking(sock);

  printf("%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
  printf("Version 0.0.1\n"
    "(C) 2023 Orora Project\n\n");

  char* prompt = (char*) const_strcat(const_strcat(ORORA_COLOR_H, ORORA_PROMPT), ORORA_COLOR_RESET);
  while (1)
  {
//     char *input = readline(DIALOGUE);
    char *input = readline(prompt);
    if (input == (void*) 0)
        continue;
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

    bool success_received = false;
    int count = -1;
    while (!success_received)
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
          printf("Server disconnected\n");
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
          printf("Server disconnected\n");
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
  //     printf("Status: %u\n", status);
  //     printf("Response: %s\n", response);

      if (status == ORORA_STATUS_SUCCESS)
      {
        success_received = true;
        if (strlen(response) == 0 && count > 0)
          printf("\n");
        else
          printf("%s", response);
      }
      else
        printf("%s", response);

      free(response);
    }
    free(input);
  }

  rl_clear_history();
  close(sock);
}


// #include "server/client.h"
// #include <arpa/inet.h>
// #include <fcntl.h>
// 
// void set_nonblocking(int sock) {
//     int flags = fcntl(sock, F_GETFL, 0);
//     fcntl(sock, F_SETFL, flags | O_NONBLOCK);
// }
// 
// void run_client(int write_fd, int read_fd)
// {
//   int sock = 0;
//     struct sockaddr_in serv_addr;
// 
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         printf("\n Socket creation error \n");
//         return;
//     }
// 
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(ORORA_PORT);
// 
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         printf("\nInvalid address/ Address not supported \n");
//         return;
//     }
// 
//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//         printf("\nConnection Failed \n");
//         return;
//     }
// 
//   printf("%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
//   printf("Version 0.0.1\n"
//       "(C) 2023 Orora Project\n\n");
// 
//   char input[BUFFER_SIZE];
//   char response[BUFFER_SIZE];
// 
//   bool is_quit = false;
// 
//   set_nonblocking(sock);
// 
//   while (1)
//   {
//     printf(DIALOGUE);
//     fflush(stdout);
//     if (fgets(input, BUFFER_SIZE, stdin) == (void*) 0)
//       break;
// 
//     if (strcmp(input, "exit\n") == 0)
//     {
//       printf("Quitting...\n");
//       is_quit = true;
//       break;
//     }
// 
//     ssize_t num_written = send(sock, input, strlen(input), 0);
//         if (num_written == -1)
//         {
//             perror("Write to orora server failed");
//             break;
//         }
// 
//         MessageHeader header;
//         ssize_t header_read = 0;
//         while (header_read < sizeof(header)) {
//             ssize_t n = recv(sock, ((char*)&header) + header_read, sizeof(header) - header_read, 0);
//             if (n > 0) {
//                 header_read += n;
//             } else if (n == 0) {
//                 printf("Server disconnected\n");
//                 return;
//             } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
//                 perror("Failed to read header");
//                 return;
//             }
//         }
// 
//         uint32_t status = ntohl(header.status);
//         uint32_t length = ntohl(header.length);
// 
//         char* response = malloc(length + 1);
//         if (!response) {
//             perror("Failed to allocate memory for response");
//             return;
//         }
// 
//         ssize_t total_read = 0;
//         while (total_read < length) {
//             ssize_t n = recv(sock, response + total_read, length - total_read, 0);
//             if (n > 0) {
//                 total_read += n;
//             } else if (n == 0) {
//                 printf("Server disconnected\n");
//                 free(response);
//                 return;
//             } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
//                 perror("Failed to read message");
//                 free(response);
//                 return;
//             }
//         }
// 
//         response[length] = '\0';
//         printf("Status: %u\n", status);
//         printf("Response: %s\n", response);
// 
//         free(response);
//     }
//     ssize_t num_written = send(sock, input, strlen(input), 0);
//         if (num_written == -1)
//         {
//             perror("Write to orora server failed");
//             break;
//         }
// 
//         ssize_t num_read;
//         response[0] = '\0';
// 
//         bool is_error = false;
//         while (1)
//         {
//             num_read = recv(sock, response, BUFFER_SIZE - 1, 0);
//             fprintf(stderr, "num_read: %ld : %s\n", num_read, response);
//             if (num_read > 0)
//             {
//                 response[num_read] = '\0';
//                 printf("%s", response);
//                 break;
//             }
//             else if (num_read == 0)
//             {
//                 printf("Server disconnected\n");
//                 is_error = true;
//                 break;
//             }
//             else
//             {
//                 perror("Read from orora server failed");
//                 is_error = true;
//                 break;
//             }
//         }
//         if (is_error)
//             break;
//     }
//     ssize_t num_written = write(write_fd, input, strlen(input));
//     if (num_written == -1)
//     {
//       perror("Write to orora server failed");
//       break;
//     }
// 
//     ssize_t num_read;
//     response[0] = '\0';
// 
//     bool is_error = false;
//     while (strncmp(response, CODE_SUCCESS, 3))
//     {
//       num_read = read(read_fd, response, BUFFER_SIZE - 1);
//       if (num_read >= 3)
//       {
//         response[num_read] = '\0';
// 
//         memmove(response, response + 3, strlen(response) - 2);
//         printf("%s", response);
//         break;
//       }
//       else
//       {
//         perror("Read from orora server failed");
//         is_error = true;
//         break;
//       }
//     }
//     if (is_error)
//       break;
//   }
//   if (!is_quit)
//     printf("\n");
// }

// void run_client(int write_fd, int read_fd)
// {
//   printf("%sOrora Programming Language%s\n", ORORA_COLOR_H, ORORA_COLOR_RESET);
//   printf("Version 0.0.1\n"
//       "(C) 2023 Orora Project\n\n");
// 
//   char input[BUFFER_SIZE];
//   char response[BUFFER_SIZE];
// 
//   bool is_quit = false;
//   while (1)
//   {
//     printf(DIALOGUE);
//     fflush(stdout);
//     if (fgets(input, BUFFER_SIZE, stdin) == (void*) 0)
//       break;
// 
//     if (strcmp(input, "exit\n") == 0)
//     {
//       printf("Quitting...\n");
//       is_quit = true;
//       break;
//     }
// 
//     ssize_t num_written = write(write_fd, input, strlen(input));
//     if (num_written == -1)
//     {
//       perror("Write to orora server failed");
//       break;
//     }
// 
//     ssize_t num_read;
//     response[0] = '\0';
// 
//     bool is_error = false;
//     while (strncmp(response, CODE_SUCCESS, 3))
//     {
//       num_read = read(read_fd, response, BUFFER_SIZE - 1);
//       if (num_read >= 3)
//       {
//         response[num_read] = '\0';
// 
//         memmove(response, response + 3, strlen(response) - 2);
//         printf("%s", response);
//         break;
//       }
//       else
//       {
//         perror("Read from orora server failed");
//         is_error = true;
//         break;
//       }
//     }
//     if (is_error)
//       break;
//   }
//   if (!is_quit)
//     printf("\n");
// }

