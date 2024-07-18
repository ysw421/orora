#include "server/daemon.h"
#include "lib/print.h"
#include "utilities/utils.h"

jmp_buf interactive_mode_buf;

volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    running = 0;
}

void run_daemon()
{
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
      
      if (setjmp(interactive_mode_buf) == 0)
      {
        AST_value_stack* new_value = visitor_visit(root_envs, ast_tree->value.compound_v->items[0])->output;
        if (new_value)
        {
          visitor_print_function_value(new_value);
          orora_write("\n", ORORA_STATUS_SUCCESS);
        }
        else
          orora_write("", ORORA_STATUS_SUCCESS);
//         if (new_value)
//         {
//           result = visitor_print_function_value(new_value);
//           result = const_strcat(result, "\n");
//           size_t num_written = orora_write(result, ORORA_STATUS_SUCCESS);
//           if (num_written == -1)
//           {
//             syslog(LOG_ERR, "Write error: %s", strerror(errno));
//             break;
//           }
//           syslog(LOG_NOTICE, "Response sent: %s", result);
//         }
//         else
//         {
//           orora_write("", ORORA_STATUS_SUCCESS);
//           syslog(LOG_NOTICE, "Response sent: (empty)");
//         }
      }
//         if (new_value)
//         {
//           result = visitor_print_function_value(new_value);
//           result = const_strcat(result, "\n");
//           size_t num_written = orora_write(result, CODE_SUCCESS);
//           if (num_written == -1)
//           {
//             syslog(LOG_ERR, "Write error: %s", strerror(errno));
//             break;
//           }
//           syslog(LOG_NOTICE, "Response sent: %s", result);
//         }
//         else
//         {
//           orora_write("", CODE_SUCCESS);
//           syslog(LOG_NOTICE, "Response sent: (empty)");
//         }
//       }
      else
      {
        orora_write("", ORORA_STATUS_SUCCESS);
      }
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

// void run_daemon()
// {
//   signal(SIGTERM, handle_signal);
//   signal(SIGINT, handle_signal);
// 
//   char buffer[BUFFER_SIZE];
// 
//   Envs* global_env = init_envs((void*) 0, init_env());
//   Envs* root_envs = init_envs(global_env, init_env());
// 
//   while (running)
//   {
//     ssize_t num_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
//     if (num_read > 0)
//     {
//       buffer[num_read] = '\0';
//       syslog(LOG_NOTICE, "Received command: %s", buffer);
// 
//       off_t* len_p = (off_t*) malloc(sizeof(off_t));
//       *len_p = (off_t) num_read;
//       
//       Lexer* root = init_lexer(buffer, len_p);
//       Parser* parser = init_parser(root);
//       AST* ast_tree = parser_parse(parser);
//       
//       const char* result = (void*) 0;
//       if (setjmp(interactive_mode_buf) == 0)
//       {
//         AST_value_stack* new_value = visitor_visit(root_envs, ast_tree->value.compound_v->items[0])->output;
//         if (new_value)
//         {
//           result = visitor_print_function_value(new_value);
//           result = const_strcat(result, "\n");
//           size_t num_written = orora_write(result, CODE_SUCCESS);
//           if (num_written == -1)
//           {
//             syslog(LOG_ERR, "Write error: %s", strerror(errno));
//             break;
//           }
//           syslog(LOG_NOTICE, "Response sent: %s", result);
//         }
//         else
//         {
//           orora_write("", CODE_SUCCESS);
//           syslog(LOG_NOTICE, "Response sent: (empty)");
//         }
//       }
//       else
//       {
//         orora_write("", CODE_SUCCESS);
//       }
//     }
//     else if (num_read == 0)
//     {
//       syslog(LOG_NOTICE, "Parent closed pipe, exiting");
//       break;
//     }
//     else
//     {
//       syslog(LOG_ERR, "Read error: %s", strerror(errno));
//       break;
//     }
//   }
//   syslog(LOG_NOTICE, "Orora exiting");
// }
