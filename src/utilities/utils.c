#include "utilities/utils.h"
#include <arpa/inet.h>

const char* const_strcat(const char* str1, const char* str2)
{
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  
  char* result = (char*) malloc(len1 + len2 + 1);
  if (!result)
      return NULL;
  
  strcpy(result, str1);
  strcat(result, str2);
  
  return result;
}

const char* float_to_string(float value)
{
  char* buffer = (char*) malloc(32 * sizeof(char));
  if (buffer == (void*) 0)
    return (void*) 0;
  
  snprintf(buffer, 32, "%f", value);

  return buffer;
}

const char* int_to_string(int value)
{
  char* buffer = (char*) malloc(32 * sizeof(char));
  if (buffer == (void*) 0)
    return (void*) 0;
  
  snprintf(buffer, 32, "%d", value);

  return buffer;
}

typedef struct {
  uint32_t status;
  uint32_t length;
} MessageHeader;

size_t orora_write(const char* message, uint32_t status)
{
  size_t message_len = strlen(message);
  
  MessageHeader header;
  header.status = htonl(status);
  header.length = htonl((uint32_t) message_len);
  
  ssize_t header_written = write(STDOUT_FILENO, &header, sizeof(header));
  if (header_written != sizeof(header))
  {
    perror("Failed to write header");
    return -1;
  }

  ssize_t message_written = write(STDOUT_FILENO, message, message_len);
  if (message_written != message_len)
  {
    perror("Failed to write message");
    return -1;
  }
  
  if (fflush(stdout) != 0)
  {
    perror("Failed to flush stdout");
    return -1;
  }

  fsync(STDOUT_FILENO);
  
  return header_written + message_written;
}

