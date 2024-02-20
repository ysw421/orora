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

typedef struct ast_code_t
{
  struct ast_compound_t* code;
} AST_code;

typedef struct ast_while_t
{
  struct ast_t* condition;
  struct ast_t* code;
} AST_while;

typedef struct ast_if_t
{
  struct ast_t* condition;
  struct ast_t* code;
} AST_if;

typedef struct ast_cases_t
{
  bool is_have_otherwise;
  int size;
  struct ast_t** codes;
  struct ast_t** conditions;
} AST_cases;

typedef struct ast_return_t
{
  struct ast_t* value;
} AST_return;

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

//   enum
//   {
//     AST_FUNCTION_TYPE_SINGLE,     // e.g. f(x):=3x+2
//     AST_FUNCTION_TYPE_DEFAULT,    // e.g. f(x):=\begin{function}
//                                   //              (code)
//                                   //            \end{function}
//     AST_FUNCTION_TYPE_NULL = 99
//   } type;

  struct ast_t* code;     // I wonder
                          //    I should use a Lexer struct
                          //    or AST struct... or Token...
                          //
                          // When type is AST_FUNCTION_TYPE_SINGLE...
                          //    -> code->type is value...
                          // When type is AST_FUNCTION_TYPE_DEFAULT...
                          //    -> code->type is compound...
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
              // ToDo...
              // Create a new type that unlimited length
} AST_int;

typedef struct ast_float_t
{
  double value;   // double
} AST_float;

typedef struct ast_bool_t
{
  bool value;
} AST_bool;

typedef struct ast_matrix_t
{
  int row_size;
  int col_size;
  struct ast_t** value;
} AST_matrix;

typedef struct ast_value_stack_t
{
  enum
  {
    AST_VALUE_VARIABLE,
    AST_VALUE_FUNCTION,
    AST_VALUE_INT,
    AST_VALUE_FLOAT,
    AST_VALUE_STRING,
    AST_VALUE_BOOL,
    AST_VALUE_MATRIX,
    AST_VALUE_PLUS,
    AST_VALUE_MINUS,
    AST_VALUE_PRODUCT,
    AST_VALUE_DOT_PRODUCT,
    AST_VALUE_DIV,
    AST_VALUE_NEG,              //  !
    AST_VALUE_LPAR,
    AST_VALUE_RPAR,
    AST_VALUE_EQUAL,
    AST_VALUE_LESS,
    AST_VALUE_GREATER,
    AST_VALUE_LESSEQUAL,
    AST_VALUE_GREATEREQUAL,
    AST_VALUE_NOTEQUAL,
    AST_VALUE_OR,
    AST_VALUE_AND,
    AST_VALUE_LBRACE,
    AST_VALUE_RBRACE,
    AST_VALUE_NULL
  } type;

  union
  {
    struct ast_string_t* string_v;
    struct ast_int_t* int_v;
    struct ast_float_t* float_v;
    struct ast_bool_t* bool_v;
    struct ast_matrix_t* matrix_v;
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
    AST_WHILE,          // 07:
    AST_IF,             // 08:
    AST_CODE,           // 09:
    AST_CASES,          // 10:
    AST_NULL,           // 11:
    AST_BREAK,          // 12:
    AST_CONTINUE,       // 13:
    AST_RETURN,         // 14:
    AST_BOOL,           // 15:
    AST_MATRIX,         // 16:
    AST_NOOP = 99       // 99: Similar with NULL
  } type;

  union
  {
    struct ast_compound_t* compound_v;
    struct ast_variable_t* variable_v;
    struct ast_function_t* function_v;
    struct ast_string_t* string_v;
    struct ast_int_t* int_v;    // I can't use a 'int'
                                //    by variable's name... 😢
                                //        Because it is a type's name...
    struct ast_float_t* float_v;
    struct ast_bool_t* bool_v;
    struct ast_matrix_t* matrix_v;
    struct ast_value_t* value_v;
    struct ast_while_t* while_v;
    struct ast_if_t* if_v;
    struct ast_code_t* code_v;
    struct ast_cases_t* cases_v;
    struct ast_return_t* return_v;
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
AST_code* init_ast_code();
AST_while* init_ast_while();
AST_if* init_ast_if();
AST_cases* init_ast_cases();
AST_return* init_ast_return();
AST_value_stack* init_ast_value_stack(int type, Token* token);
AST_value* init_ast_value();
AST_variable* init_ast_variable(char* name, size_t length);
AST_function* init_ast_function(char* name, size_t length);

AST_string* init_ast_string(Parser* parser);
AST_int* init_ast_int(Parser* parser);
AST_float* init_ast_float(Parser* parser);
AST_bool* init_ast_bool(Parser* parser);
AST_matrix* init_ast_matrix(Parser* parser, bool is_minus);

AST_compound* ast_compound_add(AST_compound* compound, AST* ast);

typedef struct orora_value_type_t
{
  struct orora_value_type_t* next;
  AST* (*parser_get_new_ast)(AST*, Parser*);
  AST_value_stack* (*parser_get_new_ast_value_stack)(Parser*, bool);
  bool (*is_check_type)(Parser* parser);
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
   orora_value_type** head, 
   AST* (*parser_get_new_ast)(AST*, Parser*),
   AST_value_stack* (*parser_get_new_ast_value_stack)(Parser*, bool),
   bool (*is_check_type)(Parser* parser),
   int ast_type_id,
   int env_variable_type_id,
   int ast_value_type_id,
   struct env_variable_t* (*visitor_set_value_Env_variable_from_AST_value_stack)
   (struct env_variable_t*, AST_value_stack*),
   AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable
   (AST_value_stack* new_value_stack, struct env_variable_t* env_variable)
  );

#endif

