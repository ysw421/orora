#include "parser/parser_id.h"
#include <stdlib.h>
#include <stdarg.h>
#include "loader/main.h"
#include "loader/error_log.h"
#include "utilities/utils.h"
#include <string.h>

AST* parser_get_satisfy
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env);
AST* parser_get_return
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env);
AST* parser_get_condition(Parser* parser);
AST* parser_get_condition_and_code
(Parser* parser, AST* ast, Token* token,
 Token* s_token, char* code,
 AST* new_ast_node, GET_COMPOUND_ENV* compound_env);
AST* parser_get_for_condition_and_code
(Parser* parser, AST* ast, Token* token,
 Token* s_token, AST* new_ast_node, GET_COMPOUND_ENV* compound_env);
AST* parser_get_while
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env);
AST* parser_get_for
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env);
AST* parser_get_if
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env);
AST* parser_get_code
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env, char* code);
AST* parser_get_cases
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env);

GET_COMPOUND_ENV* init_get_compound_env(GET_COMPOUND_ENV* env)
{
  GET_COMPOUND_ENV* new_env = malloc(sizeof(GET_COMPOUND_ENV));
  new_env->is_allow_linebreak = false;
  new_env->is_in_parentheses = false;
  new_env->is_in_braces = false;
  new_env->is_usefull_comma = false;
  new_env->is_usefull_end = (void*) 0;
  new_env->is_size_one = false;

  if (env)
  {
    new_env->is_usefull_break = env->is_usefull_break;
    new_env->is_usefull_continue = env->is_usefull_continue;
    new_env->is_usefull_return = env->is_usefull_return;
  }
  else
  {
    new_env->is_usefull_break = false;
    new_env->is_usefull_continue = false;
    new_env->is_usefull_return = false;
  }

  return new_env;
}

Parser* after_get_parser(Parser* parser, size_t* parser_size)
{
  Token* token = parser->tokens[*parser_size - 2];
  Token* next_token = parser->tokens[*parser_size - 1];

  if (
        token->type == TOKEN_LEFT 
        && next_token 
        && next_token->type == TOKEN_LPAR
      )
  {
    if (token->col_first != next_token->col_first)
    {
      orora_error("에러, '\\left'와 '('가 다른 줄에 존재함.", (void*) 0);
//       printf("에러, '\\left'와 '('가 다른 줄에 존재함.");
//       exit(1);
    }
    int new_value_length = 6 + next_token->row_char_first - token->row_char;
    char* new_value = malloc((new_value_length + 1) * sizeof(char));
    new_value[0] = '\0';
    strcat(new_value, "\\left");
    for (int i = 0; i < new_value_length - 6; i ++)
    {
      strcat(new_value, " ");
    }
    strcat(new_value, "(");
    parser->tokens[*parser_size - 2]->value = new_value;
    parser->tokens[*parser_size - 2]->length = new_value_length;
    parser->tokens[*parser_size - 2]->type = TOKEN_LPAR;
    parser->tokens[*parser_size - 2]->col = next_token->col;
    parser->tokens[*parser_size - 2]->row_char = next_token->row_char;

    *parser_size = *parser_size - 1;
    parser->tokens = realloc(parser->tokens, *parser_size * sizeof(Token*));
  }

  else if (
            token->type == TOKEN_RIGHT
            && next_token
            && next_token->type == TOKEN_RPAR
          )
  {
    if (token->col_first != next_token->col_first)
    {
      orora_error("에러, '\\right'와 ')'가 다른 줄에 존재함.", (void*) 0);
//       printf("에러, '\\right'와 ')'가 다른 줄에 존재함.");
//       exit(1);
    }
    int new_value_length = 7 + next_token->row_char_first - token->row_char;
    char* new_value = malloc((new_value_length + 1) * sizeof(char));
    new_value[0] = '\0';
    strcat(new_value, "\\right");
    for (int i = 0; i < new_value_length - 7; i ++)
    {
      strcat(new_value, " ");
    }
    strcat(new_value, ")");
    parser->tokens[*parser_size - 2]->value = new_value;
    parser->tokens[*parser_size - 2]->length = new_value_length;
    parser->tokens[*parser_size - 2]->type = TOKEN_RPAR;
    parser->tokens[*parser_size - 2]->col = next_token->col;
    parser->tokens[*parser_size - 2]->row_char = next_token->row_char;

    *parser_size = *parser_size - 1;
    parser->tokens = realloc(parser->tokens, *parser_size * sizeof(Token*));
  }

  return parser;
}

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*) malloc(sizeof(struct parser_t));
  parser->tokens = malloc(sizeof(Token*));
  size_t parser_size = 0;
  while (true)
  {
    Token* new_token = lexer_get_token(lexer);
    if (new_token)
    {
      parser_size ++;
      parser->tokens = realloc(parser->tokens, parser_size * sizeof(Token*));
      parser->tokens[parser_size - 1] = new_token;
      if (parser_size >= 2)
        parser = after_get_parser(parser, &parser_size);
      continue;
    }
    break;
  }
  parser->size = parser_size;
  parser->pointer = 0;

  parser->lexer = lexer;
  parser->prev_token = (void*) 0;
  parser->token = parser->size == 0 ? (void*) 0 : parser->tokens[0];
  parser->next_token = parser->size <= 1 ? (void*) 0 : parser->tokens[1];
  parser->row_size = 0;
  parser->row_tokens = malloc(sizeof(Token*));
  parser->row_tokens[0] = malloc(sizeof(Token));

  if (!parser->token)
  {
//     printf("에러, 파일이 비어있음\n");
    if (!INTERACTIVE_MODE)
//       continue;
//       orora_write("", STA
//     else
      exit(1);
  }
  return parser;
}

Parser* parser_set(Parser* parser, size_t pointer)
{
  if (!parser || pointer < 0 || pointer > parser->size)
  {
    orora_error("에러, 잘못된 parser 설정", (void*) 0);
//     printf("에러, 잘못된 parser 설정\n");
//     exit(1);
  }
  parser->pointer = pointer;
  parser->prev_token = (pointer >= 1)
                          ? parser->tokens[pointer - 1]
                          : (void*) 0;
  parser->token = pointer == parser->size
                          ? (void*) 0
                          : parser->tokens[pointer];
  parser->next_token = (pointer + 1 < parser->size)
                          ? parser->tokens[pointer + 1]
                          : (void*) 0;

  return parser;
}

Parser* parser_advance(Parser* parser, int type)
{
  if (parser->token->type != type)
  {
    const char* error_message = "에러, ";
    error_message = const_strcat(error_message, parser->token->value);
    error_message = const_strcat(error_message, "의 타입은 ");
    error_message = const_strcat(error_message, int_to_string(parser->token->type));
    error_message = const_strcat(error_message, "가 아닌 ");
    error_message = const_strcat(error_message, int_to_string(type));
    error_message = const_strcat(error_message, "여야함\nprev: ");
    error_message = const_strcat(error_message, parser->prev_token ? parser->prev_token->value : "");
    error_message = const_strcat(error_message, ", next: ");
    error_message = const_strcat(error_message, parser->next_token ? parser->next_token->value : "");
    orora_error(error_message, (void*) 0);
//     printf(
//         "에러, %s의 타입은 %d가 아닌 %d여야함\nprev: %s, next: %s\n",
//         parser->token->value,
//         parser->token->type,
//         type,
//         parser->prev_token ? parser->prev_token->value : "",
//         parser->next_token ? parser->next_token->value : ""
//     );
//     exit(1);
  }
//   free(parser->prev_token);
  parser->prev_token = parser->token;
  parser->token = parser->next_token ? parser->next_token : (void*) 0;
  parser->pointer ++;
  if (parser->pointer + 2 <= parser->size)
    parser->next_token = parser->tokens[parser->pointer + 1];
  else
    parser->next_token = (void*) 0;

  if (!parser->token)
    return parser;

  if (parser->prev_token->col == parser->token->col_first)
  {
    parser->row_size ++;
    parser->row_tokens = realloc(parser->row_tokens,
        parser->row_size * sizeof(Token*));
    parser->row_tokens[parser->row_size - 1] = parser->prev_token;
  }
  else
  {
    free(parser->row_tokens);
    parser->row_size = 0;
    parser->row_tokens = malloc(sizeof(Token*));
  }

  return parser;
}

AST* parser_get_compound_end(AST* ast, GET_COMPOUND_ENV* compound_env)
{
//   free(compound_env);
//   for cases command...
  return ast;
}

AST* parser_get_compound(Parser* parser, GET_COMPOUND_ENV* compound_env)
{
  AST* ast = init_ast(AST_COMPOUND, (void*) 0, (void*) 0);
  ast->value.compound_v = init_ast_compound();

  Token* token = parser->token;
  bool is_first_turn = true;

  while (token)
  {
    if (compound_env->is_size_one && !is_first_turn)
    {
      break;
    }
    else if (
          !compound_env->is_allow_linebreak
//         !compound_env->is_in_parentheses
//         && !compound_env->is_in_braces
          && parser->prev_token 
          && parser->prev_token->col == token->col_first
          && !is_first_turn
          && !(compound_env->is_usefull_end 
               && parser_is_end(parser, compound_env->is_usefull_end))
        )
    {
      const char* error_message = "에러, 각 명령어는 줄바꿈으로 구분됨:: ";
      error_message = const_strcat(error_message, token->value);
      error_message = const_strcat(error_message, " 전:: 줄: ");
      error_message = const_strcat(error_message, int_to_string(token->row_char_first));
      orora_error(error_message, (void*) 0);
//       printf("에러, 각 명령어는 줄바꿈으로 구분됨:: %s 전:: 줄: %ld\n",
//           token->value, token->col_first + 1);
//       exit(1);
    }

    if (is_first_turn)
      is_first_turn = false;

    bool is_break = true;
    switch (token->type)
    {
      case TOKEN_BEGIN:
        Token* s_token = parser->token;
        char* code = parser_is_begin(parser, 3, 
                        "while", 
                        "if",
                        "for"
                     );
        if (code)
        {
          if (!strcmp(code, "while"))
          {
            ast_compound_add(
                  ast->value.compound_v, 
                  parser_get_while(
                      parser, 
                      ast, 
                      token, 
                      s_token,
                      compound_env
                    )
                );
            token = parser->token;

            continue;
          }
          else if (!strcmp(code, "for"))
          {
            ast_compound_add(
                  ast->value.compound_v, 
                  parser_get_for(
                      parser, 
                      ast, 
                      token, 
                      s_token,
                      compound_env
                    )
                );
            token = parser->token;

            continue;
          }
          else if (!strcmp(code, "if"))
          {
            ast_compound_add(
                  ast->value.compound_v, 
                  parser_get_if(
                      parser, 
                      ast, 
                      token, 
                      s_token,
                      compound_env
                    )
                );
            token = parser->token;

            continue;
          }
        }
        else
          is_break = false;
        break;

      case TOKEN_END:
        if (compound_env->is_usefull_end 
            && parser_is_end(parser, compound_env->is_usefull_end)
           )
          return parser_get_compound_end(ast, compound_env);
        break;

      case TOKEN_COMMA:
        if (compound_env->is_usefull_comma)
          return parser_get_compound_end(ast, compound_env);
        break;

      case TOKEN_RPAR:
        if (compound_env->is_in_parentheses)
        {
          return parser_get_compound_end(ast, compound_env);
        }
        break;

      case TOKEN_RBRACE:
        if (compound_env->is_in_braces)
        {
          return parser_get_compound_end(ast, compound_env);
        }
        break;

      case TOKEN_SATISFY:
        AST* new_satisfy_ast = 
          parser_get_satisfy(parser, ast, compound_env);
        if (new_satisfy_ast)
        {
          ast_compound_add(ast->value.compound_v, new_satisfy_ast);
          token = parser->token;
          
          continue;
        }
        else
        {
          free(new_satisfy_ast);
          printf("satisfy 사용법: 'satisfy (variable name):(condition1), (condition2)\n");
          exit(1);
        }
        break;

      case TOKEN_RETURN:
        if (compound_env->is_usefull_return)
        {
          AST* new_return_ast = parser_get_return(parser, ast, compound_env);
          if (new_return_ast)
          {
            ast_compound_add(ast->value.compound_v, new_return_ast);
            token = parser->token;

            continue;
          }
          else
          {
            free(new_return_ast);
            printf("return이 잘못됨...\n");
            exit(1);
          }
        }
        else
        {
          orora_error("에러, return을 사용할 수 없는 위치임", (void*) 0);
//           printf("에러, return을 사용할 수 없는 위치임\n");
//           exit(1);
        }
        break;

      case TOKEN_BREAK:
        if (compound_env->is_usefull_break)
        {
          AST* new_break_ast = init_ast(AST_BREAK, ast, parser->token);
          parser = parser_advance(parser, TOKEN_BREAK);
          token = parser->token;

          ast_compound_add(ast->value.compound_v, new_break_ast);

          continue;
        }
        else
        {
          orora_error("에러, break를 사용할 수 없는 위치임", (void*) 0);
//           printf("에러, break를 사용할 수 없는 위치임\n");
//           exit(1);
        }
        break;

      case TOKEN_CONTINUE:
        if (compound_env->is_usefull_continue)
        {
          AST* new_continue_ast = init_ast(AST_CONTINUE, ast, parser->token);
          parser = parser_advance(parser, TOKEN_CONTINUE);
          token = parser->token;

          ast_compound_add(ast->value.compound_v, new_continue_ast);

          continue;
        }
        else
        {
          orora_error("에러, continue를 사용할 수 없는 위치임", (void*) 0);
//           printf("에러, continue를 사용할 수 없는 위치임\n");
//           exit(1);
        }
        break;

      case TOKEN_DEFINE:
        const char* error_message = "에러, ";
        error_message = const_strcat(error_message, token->value);
        error_message = const_strcat(error_message, "의 주어가 존재하지 않음");
        orora_error(error_message, (void*) 0);
//         printf("에러, '%s'의 주어가 존재하지 않음\n", token->value);
//         exit(1);
        break;

      default:
        is_break = false;
        break;
    }
    if (is_break)
      break;

    // check value
    Token* stoken = parser->token;
    GET_VALUE_ENV* new_get_value_env = init_get_value_env();
    if (compound_env->is_in_parentheses)
    {
      new_get_value_env->is_in_parentheses = true;
    }
    AST* value_node =
      parser_get_value(
          &parser, 
          ast, 
          token, 
          new_get_value_env, 
          compound_env
        );
    token = parser->token;
    if (value_node)
    {
      if (value_node->value.value_v->size == 1
          && value_node->value.value_v->stack->type == AST_VALUE_VARIABLE)
      {
        ast_compound_add(ast->value.compound_v,
            parser_parse_variable(
                parser, 
                ast, 
                parser->prev_token, 
                compound_env
              )
            );
        token = parser->token;
      }
      else if (value_node->value.value_v->size == 1
          && value_node->value.value_v->stack->type == AST_VALUE_FUNCTION)
      {
        ast_compound_add(ast->value.compound_v,
            parser_parse_function(
                parser, 
                ast, 
                stoken,
                value_node->value.value_v->stack->value.function_v,
                compound_env
              )
            );
        token = parser->token;
      }
      else
      {
        free(stoken);
        ast_compound_add(ast->value.compound_v, value_node);
        token = parser->token;
      }

//       free(value_node);
      continue;
    }
    else
    {
      free(value_node);
      const char* error_message = "에러, 설정되지 않은 token이 있음: ";
      error_message = const_strcat(error_message, token->value);
      orora_error(error_message, (void*) 0);
//       printf("에러, 설정되지 않은 token이 있음: %s\n", token->value);
//       exit(1);
    }
    free(value_node);
    // -----------
  }

  return parser_get_compound_end(ast, compound_env);
}

AST* parser_parse(Parser* parser)
{
  parser = parser_set(parser, 0);
  AST* ast = 
    parser_get_compound(parser, init_get_compound_env((void*)0));
  // Root AST...

  return ast;
}

AST* parser_get_condition
(Parser* parser)
{
  bool is_error = true;
  AST* new_condition_ast = (void*) 0;

  Token* token = parser->token;
  if (token
      && token->type == TOKEN_LBRACE
      && parser->prev_token->col == token->col_first
     )
  {
    parser = parser_advance(parser, TOKEN_LBRACE);
    token = parser->token;

    GET_COMPOUND_ENV* new_env = init_get_compound_env((void*) 0);
    new_env->is_allow_linebreak = true;
    new_env->is_in_braces = true;

    new_condition_ast = 
      parser_get_compound(parser, new_env);
    token = parser->token;

    if (
        new_condition_ast 
        && new_condition_ast->type == AST_COMPOUND
        && new_condition_ast->value.compound_v->size == 1
        && token 
        && token->type == TOKEN_RBRACE
       )
    {
      parser = parser_advance(parser, TOKEN_RBRACE);
      is_error = false;
    }
  }
  
  if (is_error)
  {
    free(new_condition_ast);
    return (void*) 0;
  }
  else
    return new_condition_ast;
}

AST* parser_get_condition_and_code
(Parser* parser, AST* ast, Token* token,
 Token* s_token, char* code,
 AST* new_ast_node, GET_COMPOUND_ENV* compound_env)
{
  bool is_error = true;

  token = parser->token;

  AST* new_condition_ast = parser_get_condition(parser);

  if (new_condition_ast)
  {
    new_ast_node->value.while_v->condition = 
        new_condition_ast->value.compound_v->items[0];

    GET_COMPOUND_ENV* get_while_code_env = 
      init_get_compound_env(compound_env);
    get_while_code_env->is_usefull_end = code;
    if (!strcmp(code, "while"))
    {
      get_while_code_env->is_usefull_break = true;
      get_while_code_env->is_usefull_continue = true;
    }

    new_ast_node->value.while_v->code =
      parser_get_compound(parser, get_while_code_env);

    if (parser->prev_token->type == TOKEN_RBRACE)
    {
      size_t pointer = parser->pointer;
      parser = parser_set(parser, parser->pointer - 4);
      if (parser_is_end(parser, code))
      {
        is_error = false;
        parser = parser_set(parser, pointer);
      }
    }
  }

  if (is_error)
  {
    const char* error_message = "에러, ";
    error_message = const_strcat(error_message, code);
    error_message = const_strcat(error_message, "문 정의가 잘못됨");
    orora_error(error_message, (void*) 0);
//     printf("에러, %s문 정의가 잘못됨\n", code);
//     exit(1);
  }

  return new_ast_node;
}

AST* parser_get_for_condition_and_code
(Parser* parser, AST* ast, Token* token,
 Token* s_token, AST* new_ast_node, GET_COMPOUND_ENV* compound_env)
{
  bool is_error = true;

  token = parser->token;

  AST* new_init_ast = parser_get_condition(parser);
  AST* new_condition_ast = parser_get_condition(parser);
  AST* new_update_ast = parser_get_condition(parser);

  if (new_init_ast && new_condition_ast && new_update_ast)
  {
    new_ast_node->value.for_v->init = 
      new_init_ast->value.compound_v->items[0];
    new_ast_node->value.for_v->condition =
      new_condition_ast->value.compound_v->items[0];
    new_ast_node->value.for_v->update =
      new_update_ast->value.compound_v->items[0];

    GET_COMPOUND_ENV* get_for_code_env = 
      init_get_compound_env(compound_env);
    get_for_code_env->is_usefull_end = "for";

    get_for_code_env->is_usefull_break = true;
    get_for_code_env->is_usefull_continue = true;

    new_ast_node->value.for_v->code =
      parser_get_compound(parser, get_for_code_env);

    if (parser->prev_token->type == TOKEN_RBRACE)
    {
      size_t pointer = parser->pointer;
      parser = parser_set(parser, parser->pointer - 4);
      if (parser_is_end(parser, "for"))
      {
        is_error = false;
        parser = parser_set(parser, pointer);
      }
    }
  }

  if (is_error)
  {
    orora_error("에러, for문 정의가 잘못됨", (void*) 0);
//     printf("에러, for문 정의가 잘못됨\n");
//     exit(1);
  }

  return new_ast_node;
}

AST* parser_get_while
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env)
{
  // Warning! possibility error...: s_token
  AST* new_ast_node = init_ast(AST_WHILE, ast, s_token);
  new_ast_node->value.while_v = init_ast_while();

  return parser_get_condition_and_code(
              parser, ast, token,
              s_token, "while", new_ast_node,
              compound_env
          );
}

AST* parser_get_for
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env)
{
  AST* new_ast_node = init_ast(AST_FOR, ast, s_token);
  new_ast_node->value.for_v = init_ast_for();

  return parser_get_for_condition_and_code(
              parser, ast, token,
              s_token, new_ast_node,
              compound_env
          );
}

AST* parser_get_if
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env)
{
  AST* new_ast_node = init_ast(AST_IF, ast, s_token);
  new_ast_node->value.if_v = init_ast_if();

  return parser_get_condition_and_code(
              parser, ast, token,
              s_token, "if", new_ast_node,
              compound_env
          );
}

AST* parser_get_cases
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env)
{
  AST* new_ast_node = init_ast(AST_CASES, ast, s_token);
  AST_cases* new_ast_cases = init_ast_cases();
  new_ast_node->value.cases_v = new_ast_cases;
  new_ast_cases->codes = malloc(sizeof(struct ast_t*));
  new_ast_cases->conditions = malloc(sizeof(struct ast_t*));

  GET_COMPOUND_ENV* new_env = 
    init_get_compound_env(compound_env);
  new_env->is_size_one = true;

  bool is_error = false;
  int size = 0;
  
  do
  {
    size ++;
    new_ast_cases->size = size;

    AST* new_value_ast = 
      parser_get_compound(parser, new_env);
    token = parser->token;

    if (
        !new_value_ast 
        || !token 
        || token->type != TOKEN_AMPER
       )
    {
      is_error = true;
      break;
    }
    new_ast_cases->codes = 
      realloc(new_ast_cases->codes, size * sizeof(struct ast_t*));
    new_ast_cases->codes[size - 1] = 
      new_value_ast->value.compound_v->items[0];

    parser = parser_advance(parser, TOKEN_AMPER);
    token = parser->token;

    if (!token)
    {
      is_error = true;
      break;
    }

    if (token->type == TOKEN_OTHERWISE)
    {
      parser = parser_advance(parser, TOKEN_OTHERWISE);
      token = parser->token;

      new_ast_cases->is_have_otherwise = true;
      if (token->type == TOKEN_DOUBLE_BACKSLASH)
      {
        parser = parser_advance(parser, TOKEN_DOUBLE_BACKSLASH);
        token = parser->token;
        if (!(token->type == TOKEN_END && parser_is_end(parser, "cases")))
          is_error = true; 
      }
      else if (token->type != TOKEN_END || !parser_is_end(parser, "cases"))
        is_error = true;
      break;
    }
    else
    {
      AST* new_condition_ast = 
        parser_get_compound(parser, new_env);
      token = parser->token;

      if (!token)
      {
        is_error = true;
        break;
      }
      new_ast_cases->conditions = 
        realloc(new_ast_cases->conditions, size * sizeof(struct ast_t*));
      new_ast_cases->conditions[size - 1] = 
        new_condition_ast->value.compound_v->items[0];

      if (token->type == TOKEN_DOUBLE_BACKSLASH)
      {
        parser = parser_advance(parser, TOKEN_DOUBLE_BACKSLASH);
        token = parser->token;
        if (token->type == TOKEN_END && parser_is_end(parser, "cases"))
        {
          break;
        }
      }
      else if (token->type == TOKEN_END && parser_is_end(parser, "cases"))
      {
        break;
      }
      else
      {
        is_error = true;
        break;
      }
    }
  } while (token);

  if (is_error)
  {
    orora_error("에러, cases문에서 뭔가 잘못됨", (void*) 0);
//     printf("에러, cases문에서 뭔가 잘못됨\n");
//     exit(1);
  }

  return new_ast_node;
}

AST* parser_get_code
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env, char* code)
{
  AST* new_ast_node = init_ast(AST_CODE, ast, s_token);

  GET_COMPOUND_ENV* get_code_env = 
    init_get_compound_env((void*) 0);
  // code command do not pass break...
//     init_get_compound_env(compound_env);
  get_code_env->is_usefull_end = code;
  get_code_env->is_usefull_return = true;

  new_ast_node->value.code_v = init_ast_code();
  new_ast_node->value.code_v->code = 
    parser_get_compound(parser, get_code_env)->value.compound_v;

  bool is_error = true;
  if (parser->prev_token->type == TOKEN_RBRACE)
  {
    size_t pointer = parser->pointer;
    parser = parser_set(parser, parser->pointer - 4);
    if (parser_is_end(parser, code))
    {
      is_error = false;
      parser = parser_set(parser, pointer);
    }
  }

  if (is_error)
  {
    orora_error("에러, code문 정의가 잘못됨", (void*) 0);
//     printf("에러, code문 정의가 잘못됨\n");
//     exit(1);
  }

  return new_ast_node;
}

AST* parser_get_satisfy
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env)
{
  Token* token = parser->token;
  parser = parser_advance(parser, TOKEN_SATISFY);
  GET_VALUE_ENV* value_env = init_get_value_env();
  int satisfy_col = parser->prev_token->col;

  AST* new_ast_node =
      init_ast(AST_VARIABLE, ast, parser->token);

  AST* main_value_ast = 
    parser_get_value(
      &parser, 
      ast, 
      token, 
      value_env, 
      compound_env
    );
  if (!main_value_ast)
  {
    free(main_value_ast);
    return (void*) 0;
  }

  AST_value* main_value = main_value_ast->value.value_v;
  token = parser->token;
  if (main_value->size == 1
      && main_value->stack->type == AST_VALUE_VARIABLE
      && satisfy_col == parser->prev_token->col_first
      && satisfy_col == token->col_first
      && parser->token && token->type == TOKEN_COLON)
  {
    parser = parser_advance(parser, TOKEN_COLON);
    token = parser->token;

    if (!token || satisfy_col != token->col_first)
    {
      return (void*) 0;
    }

    AST_variable* main_variable = main_value->stack->value.variable_v;
    do
    {
      AST* condition =
        parser_get_value(
            &parser, 
            ast, 
            token, 
            value_env, 
            compound_env
          );
      token = parser->token;
      if (!condition)
      {
        free(condition);
        break;
      }
      main_variable->satisfy_size ++;
      main_variable->satisfy =
        realloc(main_variable->satisfy,
            main_variable->satisfy_size * sizeof(struct ast_t*));
      main_variable->satisfy[main_variable->satisfy_size - 1]
        = condition;
      if (!token || satisfy_col != token->col_first
          || token->type != TOKEN_COMMA)
      {
        break;
      }
      else
      {
        parser = parser_advance(parser, TOKEN_COMMA);
        token = parser->token;
      }
    } while (token);
    token = parser->token;

    new_ast_node->value.variable_v = main_variable;
    new_ast_node->value.variable_v->ast_type = AST_VARIABLE_SATISFY;

    return new_ast_node;
  }

  free(new_ast_node);
  return (void*) 0;
}

AST* parser_get_return
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env)
{
  parser = parser_advance(parser, TOKEN_RETURN);
  Token* token = parser->token;

  int return_col = parser->prev_token->col;

  AST* new_ast_node =
      init_ast(AST_RETURN, ast, parser->token);
  new_ast_node->value.return_v = init_ast_return();

  AST* main_value_ast = 
    parser_get_value(
      &parser, 
      ast, 
      token, 
      init_get_value_env(), 
      compound_env
    );
  if (!main_value_ast)
  {
    free(main_value_ast);
    return (void*) 0;
  }

  if (return_col == parser->prev_token->col_first
      && return_col == token->col_first)
  {
    new_ast_node->value.return_v->value = main_value_ast;

    return new_ast_node;
  }

  return (void*) 0;
}

char* parser_is_begin(Parser* parser, int count, ...)
{
  size_t pointer = parser->pointer;
  if (
        pointer + 3 <= parser->size
        && parser->tokens[pointer]->type == TOKEN_BEGIN
        && parser->tokens[pointer + 1]->type == TOKEN_LBRACE
        && parser->tokens[pointer + 2]->type == TOKEN_ID
        && parser->tokens[pointer + 3]->type == TOKEN_RBRACE
      )
  {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i ++)
    {
      char* code = va_arg(args, char*);
      if (!strcmp(parser->tokens[pointer + 2]->value, code))
      {
        va_end(args);
        parser = parser_advance(parser, TOKEN_BEGIN);
        parser = parser_advance(parser, TOKEN_LBRACE);
        parser = parser_advance(parser, TOKEN_ID);
        parser = parser_advance(parser, TOKEN_RBRACE);

        return code;
      }
    }
    va_end(args);
    return (void*) 0;
  }
  else
    return (void*) 0;
}

bool parser_is_end(Parser* parser, char* code)
{
  size_t pointer = parser->pointer;
  if (
        pointer + 3 <= parser->size
        && parser->tokens[pointer]->type == TOKEN_END
        && parser->tokens[pointer + 1]->type == TOKEN_LBRACE
        && parser->tokens[pointer + 2]->type == TOKEN_ID
        && !strcmp(parser->tokens[pointer + 2]->value, code)
        && parser->tokens[pointer + 3]->type == TOKEN_RBRACE
      )
  {
    parser = parser_advance(parser, TOKEN_END);
    parser = parser_advance(parser, TOKEN_LBRACE);
    parser = parser_advance(parser, TOKEN_ID);
    parser = parser_advance(parser, TOKEN_RBRACE);

    return true;
  }

  return false;
}

