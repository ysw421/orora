#ifndef AST_H
#define AST_H

#include <stddef.h>
#include "../../Lexer/token.h"
#include "../parser_T.h"

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

typedef struct ast_t
{
  enum
  {
    // ====Why I use suffix====
    // 'AST_STRING' vs 'STRING'
    //  -> AST          -> Token
    // =========================
    AST_COMPOUND,       // 00: struct ast_t**
    AST_VARIABLE,       // 01:
    AST_FUNCTION,       // 02:
    AST_STRING,         // 03:
    AST_INT,            // 04:
    AST_FLOAT,          // 05:
    AST_NOOP,           // 06: Similar with NULL
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
  struct ast_float_t* float_v;

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
AST_variable* init_ast_variable(char* name, size_t length);
AST_function* init_ast_function(char* name, size_t length);
AST_string* init_ast_string(char* value);
AST_int* init_ast_int(int value);
AST_float* init_ast_float(double value);

AST_compound* ast_compound_add(AST_compound* compound, AST* ast);

typedef struct orora_value_type_t
{
  struct orora_value_type_t* next;
  char* name;
  int token_id;
  AST* (*parser_get_new_ast)(AST*, Token*);
} orora_value_type;

orora_value_type* push_value_type_list
  (
   orora_value_type** head, char* name, int token_id,
   AST* (*parser_get_new_ast)(AST*, Token*)
  );

#endif
