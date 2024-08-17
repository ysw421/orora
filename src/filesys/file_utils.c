#include "filesys/file.h"
#include "loader/error_log.h"
#include "utilities/utils.h"
#include <stdio.h>
#include <stdlib.h>

File* file_open(const char* file_path)
{
  FILE* f = fopen(file_path, "rb");
  if (!f)
  {
    const char* error_message = "에러, 파일 ";
    error_message = const_strcat(error_message, file_path);
    error_message = const_strcat(error_message, "를 찾을 수 없습니다.");
    orora_error(error_message, (void*) 0);
  }

  File* file = (File*) malloc(sizeof(File));

  fseek(f, 0, SEEK_END);
  size_t size = ftell(f);

  file->contents = malloc(size+1);
  memset(file->contents, 0, size+1);
  fseek(f, 0, SEEK_SET);
  fread(file->contents, size, 1, f);

  file->length = size;

  fclose(f);
  return file;
}

