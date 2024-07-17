#include "utilities/utils.h"

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
  if (buffer == NULL)
    return NULL;
  
  snprintf(buffer, 32, "%f", value);

  return buffer;
}
