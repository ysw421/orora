#include "server/daemon.h"
#include "lib/print.h"

volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    running = 0;
}

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

void run_daemon() {
  signal(SIGTERM, handle_signal);
  signal(SIGINT, handle_signal);

  char buffer[BUFFER_SIZE];

  Envs* global_env = init_envs((void*) 0, init_env());
  Envs* root_envs = init_envs(global_env, init_env());

  while (running)
  {
    ssize_t num_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
    if (num_read > 0)
    {
      buffer[num_read] = '\0';
      syslog(LOG_NOTICE, "Received command: %s", buffer);

      off_t* len_p = (off_t*) malloc(sizeof(off_t));
      *len_p = (off_t) num_read;
      
      Lexer* root = init_lexer(buffer, len_p);
      Parser* parser = init_parser(root);
      AST* ast_tree = parser_parse(parser);
      
      const char* result = (void*) 0;
      AST_value_stack* new_value = visitor_visit(root_envs, ast_tree->value.compound_v->items[0])->is_return;
      if (new_value)
      {
        result = visitor_print_function_value(new_value);
        result = const_strcat(result, "\n");
        size_t num_written = write(STDOUT_FILENO, result, strlen(result));
        if (num_written == -1)
        {
          syslog(LOG_ERR, "Write error: %s", strerror(errno));
          break;
        }
      }
      else
      {
        write(STDOUT_FILENO, "", 1);
      }
      fflush(stdout);

      syslog(LOG_NOTICE, "Response sent: %s", result);
    }
    else if (num_read == 0)
    {
      syslog(LOG_NOTICE, "Parent closed pipe, exiting");
      break;
    }
    else
    {
      syslog(LOG_ERR, "Read error: %s", strerror(errno));
      break;
    }
  }
  syslog(LOG_NOTICE, "Orora exiting");
}
