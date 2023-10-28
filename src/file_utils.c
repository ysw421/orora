#include "file.h"
#include <stdio.h>
#include <stdlib.h>

File* openfile(const char* file_path)
{
  File* file = (void*) 0;

  FILE* f = fopen(file_path, "rb");
  if (!f)
  {
    printf("에러, 파일 %s를 찾을 수 없습니다.\n", file_path);
    exit(2);
  }

  file = (File*) malloc(sizeof(File));

  fseek(f, 0, SEEK_END);
  file->length = ftell(f);
  fseek(f, 0, SEEK_SET);

  file->contents = (char*) malloc(file->length * sizeof(char) + 1);
  fread(file->contents, 1, file->length, f);

  fclose(f);
  return file;
}
