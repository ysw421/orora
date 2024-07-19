#include "server/daemon.h"
#include "lib/print.h"
#include "utilities/utils.h"

jmp_buf interactive_mode_buf;

volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    running = 0;
}

bool is_end_interactive_line(Lexer* lexer)
{
  Token* token = (void*) 0;
  int lpar = 0;
  int rpar = 0;
  int lbrace = 0;
  int rbrace = 0;
  int lsqb = 0;
  int rsqb = 0;
  int begin = 0;
  int end = 0;

  while ((token = lexer_get_token(root)) != (void*) 0)
  {
    if (token->type == TOKEN_LPAREN)
      lpar ++;
    else if (token->type == TOKEN_RPAREN)
      rpar ++;
    else if (token->type == TOKEN_LBRACE)
      lbrace ++;
    else if (token->type == TOKEN_RBRACE)
      rbrace ++;
    else if (token->type == TOKEN_LSQB)
      lsqb ++;
    else if (token->type == TOKEN_RSQB)
      rsqb ++;
    else if (token->type == TOKEN_BEGIN)
      begin ++;
    else if (token->type == TOKEN_END)
      end ++;
  }
  if (lpar == rpar && lbrace == rbrace && lsqb == rsqb && begin == end)
    return true;
  return false;
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
#include "develop/develop_mode.h"
      print_tokens(root);
      Parser* parser = init_parser(root);
      AST* ast_tree = parser_parse(parser);
      
      if (setjmp(interactive_mode_buf) == 0)
      {
        AST_value_stack* new_value = visitor_visit(root_envs, ast_tree->value.compound_v->items[0])->output;
        if (new_value)
        {
          if (!(new_value->type == AST_VALUE_NULL && ast_tree->value.compound_v->items[0]->type == AST_FUNCTION))
            visitor_print_function_value(new_value);
          orora_write("\n", ORORA_STATUS_SUCCESS);
        }
        else
          orora_write("", ORORA_STATUS_SUCCESS);
      }
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

