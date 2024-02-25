#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include "os/os.h"

typedef struct
{
  char* contents;
  off_t length;
} File;

File* openfile(const char* file_path);

#endif
