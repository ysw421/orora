#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef struct ast_compound_t
{
  size_t size;
  struct ast_t** items;
} AST_compound;

typedef struct ast_variable_t
{
  char* name;
  size_t name_length;
  struct ast_t* value;
} AST_variable;

typedef struct ast_function_t
{
  char* name;
  size_t name_length;
  struct ast_t** args;
  size_t args_size;
  struct ast_t** codes;   // I wonder
                          //    I should use a Lexer struct
                          //    or AST struct... or Token...
  size_t codes_length;
} AST_function;

typedef struct ast_string_t
{
  char* value;
  size_t value_length;
} AST_string;

typedef struct ast_int_t
{
  int value;  // I should not use a pointer...??
} AST_int;

typedef struct ast_float_t
{
  float value;
} AST_float;

typedef struct ast_t
{
  enum
  {
    // ====Why I use suffix====
    // 'AST_STRING' vs 'STRING'
    //  -> AST          -> Token
    // =========================
    AST_COMPOUND,     // 01: struct ast_t**
    AST_VARIABLE,     // 02:
    AST_FUNCTION,     // 03:
    AST_STRING,       // 04:
    AST_INT,          // 05:
    AST_FLOAT,        // 06:
    AST_NOOP,         // 07: Similar with NULL
  } type;

  /* --COMPOUND-- */
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
  struct ast_float_t float_v;

  struct ast_t* parent;

  size_t col;
  size_t col_first;
  size_t row;
  size_t row_char;
  size_t row_char_first;
} AST;


AST* init_ast(int type, AST* parent, size_t col, size_t col_first,
              size_t row, size_t row_char, size_t row_char_first);

AST_compound* init_ast_compound();
AST_variable* init_ast_variable();
AST_function* init_ast_function();
AST_string* init_ast_string(char* value);
AST_int* init_ast_int(int value);
AST_float* init_ast_float(float value);

AST_compound* ast_compound_add(AST_compound* compound, AST* ast);

#endif
