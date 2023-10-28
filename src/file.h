#ifndef FILE_H
#define FILE_H

typedef struct
{
  char* contents;
  unsigned long int length;
} File;

File* openfile(const char* file_path);

#endif
