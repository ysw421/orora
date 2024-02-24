#ifndef FILE_H
#define FILE_H

#include <stdint.h>

typedef struct
{
  char* contents;
  int64_t length;
} File;

File* openfile(const char* file_path);

#endif
