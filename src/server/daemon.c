#include "server/daemon.h"
#include "lib/print.h"
#include "utilities/utils.h"

jmp_buf interactive_mode_buf;

volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
  running = 0;
}

int is_end_interactive_line(Lexer* root)
{
  Lexer* lexer = malloc(sizeof(Lexer));
  memcpy(lexer, root, sizeof(Lexer));
  Token* token = (void*) 0;
  int lpar = 0;
  int rpar = 0;
  int lbrace = 0;
  int rbrace = 0;
  int lsqb = 0;
  int rsqb = 0;
  int begin = 0;
  int end = 0;

  while ((token = lexer_get_token(lexer)) != (void*) 0)
  {
    if (token->type == TOKEN_LPAR)
      lpar ++;
    else if (token->type == TOKEN_RPAR)
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
  free(lexer);
  if (lpar == rpar && lbrace == rbrace && lsqb == rsqb && begin == end)
    return 0;
  return lpar - rpar + lbrace - rbrace + lsqb - rsqb + begin - end;
}

void run_daemon()
{
//   signal(SIGTERM, handle_signal);
//   signal(SIGINT, handle_signal);

  char buffer[BUFFER_SIZE];

  Envs* global_env = init_envs((void*) 0, init_env());
  Envs* root_envs = init_envs(global_env, init_env());

  char* input = (char*) 0;
  size_t len_input = 0;
  bool is_first_line = true;

  while (running)
  {
    ssize_t num_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
    if (num_read <= 0)
    {
      if (num_read == 0)
        syslog(LOG_NOTICE, "Parent closed pipe, exiting");
      else
        syslog(LOG_ERR, "Read error: %s", strerror(errno));
      break;
    }

    if (is_first_line)
    {
      input = malloc(num_read + 1);
      if (input == (void*) 0)
      {
        syslog(LOG_ERR, "Memory allocation failed");
        break;
      }
      memcpy(input, buffer, num_read);
      len_input = num_read;
      input[len_input] = '\0';
      is_first_line = false;
    }
    else
    {
      char *temp = realloc(input, len_input + num_read + 1);
      if (temp == (void*) 0)
      {
        syslog(LOG_ERR, "Memory reallocation failed");
        free(input);
        break;
      }
      input = temp;
      memcpy(input + len_input, buffer, num_read);
      len_input += num_read;
      input[len_input] = '\0';
    }

    char last_char = input[len_input];
    input[len_input] = '\0';
    syslog(LOG_NOTICE, "Received command: %s", input);

    off_t* len_p = (off_t*) malloc(sizeof(off_t));
    *len_p = (off_t) len_input;
    
    Lexer* root = init_lexer(input, len_p);

    Lexer* lexer_null_checker = malloc(sizeof(Lexer));
    memcpy(lexer_null_checker, root, sizeof(Lexer));
    if (lexer_get_token(lexer_null_checker) == (void*) 0)
    {
      free(lexer_null_checker);
      orora_write("", ORORA_STATUS_SUCCESS);
      is_first_line = true;
      continue;
    }
    free(lexer_null_checker);

// #include "develop/develop_mode.h"
//     printf("root_size: %d\n", root->size);
//     print_tokens(root);
    int right_space = is_end_interactive_line(root);
    if (right_space == 0)
    {
      is_first_line = true;
    }
    else
    {
      len_input ++;
      input = realloc(input, len_input + 1);
      input[len_input-1] = '\n';
      input[len_input] = last_char;
      orora_write(int_to_string(right_space), ORORA_STATUS_MORE);
//       orora_write("", ORORA_STATUS_SUCCESS);
      continue;
    }
// #include "develop/develop_mode.h"
//       print_tokens(root);
    Parser* parser = init_parser(root);
    AST* ast_tree = parser_parse(parser);
    
    if (setjmp(interactive_mode_buf) == 0)
    {
      AST* ast = ast_tree->value.compound_v->items[0];
      AST_value_stack* new_value = visitor_visit(root_envs, ast)->output;
      if (new_value)
      {
        if (new_value->type != AST_VALUE_NULL || !(ast->type == AST_FUNCTION
              || ast->type == AST_CODE
              || ast->type == AST_FOR
              || ast->type == AST_WHILE
              || ast->type == AST_CASES
              || (ast->type == AST_VALUE && (ast->value.value_v->stack->type == AST_VALUE_CODE)
                )
              )
            )
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
  syslog(LOG_NOTICE, "Orora exiting");
}

