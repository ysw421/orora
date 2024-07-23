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
  file->length = ftell(f);
  fseek(f, 0, SEEK_SET);

  file->contents = (char*) malloc(file->length * sizeof(char) + 1);
  fread(file->contents, 1, file->length, f);

  fclose(f);
  return file;
}

