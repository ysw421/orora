#include "server/daemon.h"
#include "develop/develop_mode.h"

volatile sig_atomic_t running = 1;

void handle_signal(int sig) {
    running = 0;
}

void run_daemon() {
  umask(0);
  openlog("orora_daemon", LOG_PID, LOG_DAEMON);
  syslog(LOG_NOTICE, "Orora Server started.");
  
  pid_t sid = setsid();
  if (sid < 0) {
    syslog(LOG_ERR, "Failed to create new session");
    exit(EXIT_FAILURE);
  }
  
  if ((chdir("/")) < 0) {
    syslog(LOG_ERR, "Failed to change working directory");
    exit(EXIT_FAILURE);
  }
  
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  FILE *pid_file = fopen(PID_FILE, "w");
  if (pid_file) {
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);
  } else {
    syslog(LOG_ERR, "Failed to open PID file");
  }

  Envs* global_env = init_envs((void*) 0, init_env());
  Envs* root_envs = init_envs(global_env, init_env());
  
  while (running) {
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
      syslog(LOG_ERR, "Failed to open FIFO: %s", strerror(errno));
      sleep(5);
      continue;
    }
    
    char buffer[BUFFER_SIZE];
    ssize_t num_read = read(fd, buffer, BUFFER_SIZE);
    close(fd);
    
    if (num_read > 0) {
      buffer[num_read] = '\0';
      syslog(LOG_NOTICE, "Received command: %s", buffer);
      
      off_t* len_p = (off_t*) malloc(sizeof(off_t));
      *len_p = (off_t)num_read;
      
      Lexer* root = init_lexer(buffer, len_p);
      Parser* parser = init_parser(root);
      AST* ast_tree = parser_parse(parser);
      
//       for (int i = 0; i < ast_tree->value.compound_v->size; i ++) {
//         visitor_visit(root_envs, ast_tree->value.compound_v->items[i]);
//       }

      bool is_return = false;
      const char* result = (void*) 0;
      AST_value_stack* new_value = visitor_visit(root_envs, ast_tree->value.compound_v->items[0])
                                                  ->is_return;
      if (new_value)
      {
        is_return = true;
        result = visitor_print_function_value(new_value);
      }
      
      FILE *result_file = fopen(RESULT_FILE, "w");
      if (result_file != NULL) {
        if (is_return)
          fprintf(result_file, "%s\n", result);
//         fprintf(result_file, "%s\n", "실행 결과");
        fclose(result_file);
      } else {
        syslog(LOG_ERR, "Failed to open result file");
      }
      
      free(len_p);
      free(root);
      free(parser);
      free(ast_tree);
      
      syslog(LOG_NOTICE, "Command execution completed");
    }
  }
  
  free(global_env);
  free(root_envs);
  unlink(PID_FILE);
  syslog(LOG_NOTICE, "Orora Server stopped.");
  closelog();
}

