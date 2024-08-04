#include "os.h"
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

#ifdef OS_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_PATH 1024

char* get_lib_path()
{
  return "/usr/lib/orora/";
}

char* get_executable_path()
{
  char* path = malloc(MAX_PATH);
  if (path == (void*) 0)
  {
    return (void*) 0;
  }

#ifdef OS_WINDOWS
  if (GetModuleFileName((void*) 0, path, MAX_PATH) == 0)
  {
    free(path);
    return (void*) 0;
  }
#else
  ssize_t count = readlink("/proc/self/exe", path, MAX_PATH);
  if (count == -1)
  {
      free(path);
      return (void*) 0;
  }
  path[count] = '\0';
#endif

  char* dir = dirname(path);
  if (dir == (void*) 0)
  {
    free(path);
    return (void*) 0;
  }

  char* result = strdup(dir);
  free(path);
  return result;
}

char* get_data_directory()
{
  char* data_dir = getenv("ORORA_DATA_DIR");
  if (data_dir != (void*) 0)
    return strdup(data_dir);

  FILE* config_file;
  char config_path[MAX_PATH];
  char line[MAX_PATH];

  #ifdef OS_WINDOWS
  snprintf(config_path, sizeof(config_path), "%s\\orora.conf", getenv("APPDATA"));
  #else
  snprintf(config_path, sizeof(config_path), "%s/.orora.conf", getenv("HOME"));
  #endif

  config_file = fopen(config_path, "r");
  if (config_file != (void*) 0)
  {
    while (fgets(line, sizeof(line), config_file))
    {
      if (strncmp(line, "DATA_DIR=", 9) == 0)
      {
        char* path = line + 9;
        char* newline = strchr(path, '\n');
        if (newline != (void*) 0)
        {
          *newline = '\0';
        }
        fclose(config_file);
        return strdup(path);
      }
    }
    fclose(config_file);
  }

  #ifdef OS_WINDOWS
  return strdup("C:\\Program Files\\Orora\\data");
  #else
  return strdup("/usr/local/share/orora");
  #endif
}
