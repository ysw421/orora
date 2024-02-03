#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdbool.h>
#include "../../Lexer/token.h"
#include "../parser_T.h"
// #include "../../env.h"

typedef struct ast_compound_t
{
  size_t size;
  struct ast_t** items;
} AST_compound;

typedef struct ast_variable_t
{
  enum
  {
    AST_VARIABLE_DEFINE,
    AST_VARIABLE_SATISFY,
    AST_VARIABLE_VALUE,
    AST_VARIABLE_NULL = 99
  } ast_type;

  char* name;
  int name_length;
  struct ast_t* value;
  int satisfy_size;
  struct ast_t** satisfy;
} AST_variable;

typedef struct ast_function_t
{
  char* name;
  int name_length;
  struct ast_t** args;
  int args_size;

  enum
  {
    AST_FUNCTION_DEFINE,
    AST_FUNCTION_VALUE,
    AST_FUNCTION_NULL = 99
  } ast_type;

  enum
  {
    AST_FUNCTION_TYPE_SINGLE,     // e.g. f(x):=3x+2
    AST_FUNCTION_TYPE_DEFAULT,    // e.g. f(x):=\begin{function}
                                  //              (code)
                                  //            \end{function}
    AST_FUNCTION_TYPE_NULL = 99
  } type;

  struct ast_t** codes;   // I wonder
                          //    I should use a Lexer struct
                          //    or AST struct... or Token...
  size_t codes_size;
} AST_function;

typedef struct ast_string_t
{
  char* value;
  char* real_value;
  size_t value_length;
} AST_string;

typedef struct ast_int_t
{
  int value;  // I should not use a pointer...??
} AST_int;

typedef struct ast_float_t
{
  double value;   // double
} AST_float;

typedef struct ast_value_stack_t
{
  enum
  {
    AST_VALUE_VARIABLE,
    AST_VALUE_FUNCTION,
    AST_VALUE_INT,
    AST_VALUE_FLOAT,
    AST_VALUE_STRING,
    AST_VALUE_PLUS,
    AST_VALUE_MINUS,
    AST_VALUE_PRODUCT,
    AST_VALUE_DOT_PRODUCT,
    AST_VALUE_DIV,
    AST_VALUE_NEG,              //  !
    AST_VALUE_LPAR,
    AST_VALUE_RPAR,
  } type;

  union
  {
    struct ast_string_t* string_v;
    struct ast_int_t* int_v;
    struct ast_float_t* float_v;
    struct ast_variable_t* variable_v;
    struct ast_function_t* function_v;
  } value;

  struct ast_value_stack_t* next;

  size_t col;
  size_t col_first;
  size_t row;
  size_t row_char;
  size_t row_char_first;
} AST_value_stack;

typedef struct ast_value_t
{
  size_t size;
  struct ast_value_stack_t* stack;
} AST_value;

typedef struct ast_t
{
  enum
  {
    // ====Why I use suffix==========
    // 'AST_STRING' vs 'TOKEN_STRING'
    //  -> AST          -> Token
    // ==============================
    AST_COMPOUND,       // 00: struct ast_t**
    AST_VARIABLE,       // 01:
    AST_FUNCTION,       // 02:
    AST_STRING,         // 03:
    AST_INT,            // 04:
    AST_FLOAT,          // 05:
    AST_VALUE,          // 06:
    AST_NOOP,           // 07: Similar with NULL
  } type;

  union
  {
    struct ast_compound_t* compound_v;
    /* --VARIABLE-- */
    struct ast_variable_t* variable_v;
    /* --FUNCTION-- */
    struct ast_function_t* function_v;
    /* ---STRING--- */
    struct ast_string_t* string_v;
    /* ----INT----- */
    struct ast_int_t* int_v;   // I can't use a 'int'
                            //    by variable's name... 😢
                            //        Because it is a type's name...
    /* ---FLOAT---- */
    struct ast_float_t* float_v;
    /* ---VALUE---- */
    struct ast_value_t* value_v;
  } value;

  struct ast_t* parent;

  size_t col;
  size_t col_first;
  size_t row;
  size_t row_char;
  size_t row_char_first;
} AST;

typedef struct
{
  AST* ast;
  Parser* parser;
} AST_PARSER;


AST* init_ast(int type, AST* parent, Token* token);
AST_PARSER* init_ast_parser(AST* ast, Parser* parser);

AST_compound* init_ast_compound();
AST_value_stack* init_ast_value_stack(int type, Token* token);
AST_value* init_ast_value();
AST_variable* init_ast_variable(char* name, size_t length);
AST_function* init_ast_function(char* name, size_t length);
AST_string* init_ast_string(Token* token);
AST_int* init_ast_int(Token* token);
AST_float* init_ast_float(Token* token);

AST_compound* ast_compound_add(AST_compound* compound, AST* ast);

typedef struct orora_value_type_t
{
  struct orora_value_type_t* next;
  char* name;
  int token_id;
  AST* (*parser_get_new_ast)(AST*, Token*);
  AST_value_stack* (*parser_get_new_ast_value_stack)(Token*, bool);
  bool (*is_check_type)(Token*);
  int ast_type_id;
  int env_variable_type_id;
  int ast_value_type_id;
  struct env_variable_t* (*visitor_set_value_Env_variable_from_AST_value_stack)
    (struct env_variable_t*, AST_value_stack*);
  AST_value_stack* (*visitor_set_value_AST_value_stack_from_Env_variable)
    (AST_value_stack* new_value_stack, struct env_variable_t* env_variable);
} orora_value_type;

orora_value_type* push_value_type_list
  (
   orora_value_type** head, char* name, int token_id,
   AST* (*parser_get_new_ast)(AST*, Token*),
   AST_value_stack* (*parser_get_new_ast_value_stack)(Token*, bool),
   bool (*is_check_type)(Token*),
   int ast_type_id,
   int env_variable_type_id,
   int ast_value_type_id,
   struct env_variable_t* (*visitor_set_value_Env_variable_from_AST_value_stack)
   (struct env_variable_t*, AST_value_stack*),
   AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable
   (AST_value_stack* new_value_stack, struct env_variable_t* env_variable)
  );

#endif
