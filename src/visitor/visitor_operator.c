#include "visitor/visitor_operator.h"

typedef AST_value_stack* (*operator_t)(
    AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
;

struct operation_t {
  int op1;
  int op2;
  operator_t opt;
};

// Operator
AST_value_stack* visitor_operator_plus(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_minus(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_product(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_div(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_mod(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_colon(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_power(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_equal(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_less(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_greater(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_comma(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_in(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);


// Plus
AST_value_stack* operator_plus_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_plus_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Minus
AST_value_stack* operator_minus_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_float_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_minus_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Product
AST_value_stack* operator_product_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_product_string_string(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Divison
AST_value_stack* operator_div_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_div_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Mod
AST_value_stack* operator_mod_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Colon
AST_value_stack* operator_colon_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Power
AST_value_stack* operator_power_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_power_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_power_float_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_power_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_power_matrix_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_power_string_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Equal
AST_value_stack* operator_equal_null_null(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_string_string(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_equal_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Less
AST_value_stack* operator_less_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_float_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_less_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Greater
AST_value_stack* operator_greater_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_int_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_float_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_int_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_bool_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_float_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_float_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_bool_float(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_greater_bool_bool(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// Comma
AST_value_stack* operator_comma_v_v(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_comma_v_m(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_comma_m_v(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_comma_m_m(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

// In
AST_value_stack* operator_in_int_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_float_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_string_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_bool_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_matrix_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_null_matrix(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* operator_in_v_string(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);


AST_value_stack* visitor_operator(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2,
    struct operation_t operation[],
    size_t operation_size
    )
{
  int op1 = operand1->type;
  int op2 = operand2->type;

  for (int i = 0; i < operation_size; i++)
  {
    if (op1 == operation[i].op1 && op2 == operation[i].op2)
      return operation[i].opt(result, operand1, operand2);
  }

  orora_error("에러, 정의되지 않은 연산", (void*) 0);

  return (void*) 0;
}

AST_value_stack* visitor_operator_plus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t plus_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_plus_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_plus_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_plus_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_plus_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_plus_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_plus_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_plus_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_plus_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_plus_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_plus_matrix_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_plus_matrix},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_plus_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_plus_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_plus_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_plus_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_plus_matrix},
  };

  return visitor_operator(result, operand1, operand2, plus_op, sizeof(plus_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_minus(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t minus_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_minus_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_minus_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_minus_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_minus_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_minus_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_minus_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_minus_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_minus_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_minus_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_minus_matrix_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_minus_matrix},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_minus_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_minus_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_minus_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_minus_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_minus_matrix},
  };

  return visitor_operator(result, operand1, operand2, minus_op, sizeof(minus_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t product_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_product_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_product_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_product_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_product_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_product_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_product_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_product_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_product_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_product_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_product_matrix_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_product_matrix},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_product_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_product_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_STRING,AST_VALUE_STRING, operator_product_string_string},
  };

  return visitor_operator(result, operand1, operand2, product_op, sizeof(product_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_dot_product(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t product_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_product_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_product_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_product_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_product_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_product_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_product_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_product_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_product_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_product_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_product_matrix_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_product_matrix},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_product_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_product_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_product_matrix},
    {AST_VALUE_STRING,AST_VALUE_STRING, operator_product_string_string},
  };

  return visitor_operator(result, operand1, operand2, product_op, sizeof(product_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_div(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t div_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_div_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_div_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_div_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_div_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_div_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_div_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_div_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_div_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_div_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_div_matrix_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_div_matrix},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_div_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_div_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_div_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_div_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_div_matrix},
  };

  return visitor_operator(result, operand1, operand2, div_op, sizeof(div_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_mod(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t mod_op[] = {
    {AST_VALUE_INT, AST_VALUE_INT, operator_mod_int_int},
  };

  return visitor_operator(result, operand1, operand2, mod_op, sizeof(mod_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_colon(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t colon_op[] = {
    {AST_VALUE_INT, AST_VALUE_INT, operator_colon_int_int},
  };

  return visitor_operator(result, operand1, operand2, colon_op, sizeof(colon_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_power(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t power_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_power_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_power_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_power_float_int},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_power_float_float},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_power_matrix_int},
    {AST_VALUE_STRING,AST_VALUE_INT,    operator_power_string_int},
  };

  return visitor_operator(result, operand1, operand2, power_op, sizeof(power_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_equal(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t equal_op[] = {
    {AST_VALUE_NULL,  AST_VALUE_NULL,   operator_equal_null_null},
    {AST_VALUE_STRING,AST_VALUE_STRING, operator_equal_string_string},
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_equal_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_equal_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_equal_int_float},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_equal_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_equal_int_bool},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_equal_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_equal_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_equal_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_equal_bool_bool},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_equal_matrix_matrix},
  };

  return visitor_operator(result, operand1, operand2, equal_op, sizeof(equal_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_less(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t less_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_less_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_less_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_less_float_int},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_less_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_less_bool_int},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_less_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_less_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_less_bool_float},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_less_bool_bool},
  };

  return visitor_operator(result, operand1, operand2, less_op, sizeof(less_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_greater(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t greater_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_greater_int_int},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_greater_int_float},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_greater_float_int},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_greater_int_bool},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_greater_bool_int},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_greater_float_float},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_greater_float_bool},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_greater_bool_float},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_greater_bool_bool},
  };

  return visitor_operator(result, operand1, operand2, greater_op, sizeof(greater_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_comma(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t comma_op[] = {
    {AST_VALUE_INT,   AST_VALUE_INT,    operator_comma_v_v},
    {AST_VALUE_INT,   AST_VALUE_FLOAT,  operator_comma_v_v},
    {AST_VALUE_INT,   AST_VALUE_STRING, operator_comma_v_v},
    {AST_VALUE_INT,   AST_VALUE_BOOL,   operator_comma_v_v},
    {AST_VALUE_INT,   AST_VALUE_MATRIX, operator_comma_v_m},
    {AST_VALUE_INT,   AST_VALUE_NULL,   operator_comma_v_v},
    {AST_VALUE_FLOAT, AST_VALUE_INT,    operator_comma_v_v},
    {AST_VALUE_FLOAT, AST_VALUE_FLOAT,  operator_comma_v_v},
    {AST_VALUE_FLOAT, AST_VALUE_STRING, operator_comma_v_v},
    {AST_VALUE_FLOAT, AST_VALUE_BOOL,   operator_comma_v_v},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX, operator_comma_v_m},
    {AST_VALUE_FLOAT, AST_VALUE_NULL,   operator_comma_v_v},
    {AST_VALUE_STRING,AST_VALUE_INT,    operator_comma_v_v},
    {AST_VALUE_STRING,AST_VALUE_FLOAT,  operator_comma_v_v},
    {AST_VALUE_STRING,AST_VALUE_STRING, operator_comma_v_v},
    {AST_VALUE_STRING,AST_VALUE_BOOL,   operator_comma_v_v},
    {AST_VALUE_STRING,AST_VALUE_MATRIX, operator_comma_v_m},
    {AST_VALUE_STRING,AST_VALUE_NULL,   operator_comma_v_v},
    {AST_VALUE_BOOL,  AST_VALUE_INT,    operator_comma_v_v},
    {AST_VALUE_BOOL,  AST_VALUE_FLOAT,  operator_comma_v_v},
    {AST_VALUE_BOOL,  AST_VALUE_STRING, operator_comma_v_v},
    {AST_VALUE_BOOL,  AST_VALUE_BOOL,   operator_comma_v_v},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX, operator_comma_v_m},
    {AST_VALUE_BOOL,  AST_VALUE_NULL,   operator_comma_v_v},
    {AST_VALUE_MATRIX,AST_VALUE_INT,    operator_comma_m_v},
    {AST_VALUE_MATRIX,AST_VALUE_FLOAT,  operator_comma_m_v},
    {AST_VALUE_MATRIX,AST_VALUE_STRING, operator_comma_m_v},
    {AST_VALUE_MATRIX,AST_VALUE_BOOL,   operator_comma_m_v},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX, operator_comma_m_m},
    {AST_VALUE_MATRIX,AST_VALUE_NULL,   operator_comma_m_v},
    {AST_VALUE_NULL,  AST_VALUE_INT,    operator_comma_v_v},
    {AST_VALUE_NULL,  AST_VALUE_FLOAT,  operator_comma_v_v},
    {AST_VALUE_NULL,  AST_VALUE_STRING, operator_comma_v_v},
    {AST_VALUE_NULL,  AST_VALUE_BOOL,   operator_comma_v_v},
    {AST_VALUE_NULL,  AST_VALUE_MATRIX, operator_comma_v_m},
    {AST_VALUE_NULL,  AST_VALUE_NULL,   operator_comma_v_v}
  };

  return visitor_operator(result, operand1, operand2, comma_op, sizeof(comma_op)/sizeof(struct operation_t));
}

AST_value_stack* visitor_operator_in(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  struct operation_t in_op[] = {
    {AST_VALUE_INT,   AST_VALUE_MATRIX,    operator_in_int_matrix},
    {AST_VALUE_FLOAT, AST_VALUE_MATRIX,    operator_in_float_matrix},
    {AST_VALUE_STRING,AST_VALUE_MATRIX,    operator_in_string_matrix},
    {AST_VALUE_BOOL,  AST_VALUE_MATRIX,    operator_in_bool_matrix},
    {AST_VALUE_MATRIX,AST_VALUE_MATRIX,    operator_in_matrix_matrix},
    {AST_VALUE_NULL,  AST_VALUE_MATRIX,    operator_in_null_matrix},
    {AST_VALUE_INT,   AST_VALUE_STRING,    operator_in_v_string},
    {AST_VALUE_FLOAT, AST_VALUE_STRING,    operator_in_v_string},
    {AST_VALUE_STRING,AST_VALUE_STRING,    operator_in_v_string},
    {AST_VALUE_BOOL,  AST_VALUE_STRING,    operator_in_v_string},
    {AST_VALUE_MATRIX,AST_VALUE_STRING,    operator_in_v_string},
    {AST_VALUE_NULL,  AST_VALUE_STRING,    operator_in_v_string},
  };

  return visitor_operator(result, operand1, operand2, in_op, sizeof(in_op)/sizeof(struct operation_t));
}


// Plus
AST_value_stack* operator_plus_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    operand1->value.int_v->value + operand2->value.int_v->value;

  return result;
}

AST_value_stack* operator_plus_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.float_v->value =
      operand1->value.int_v->value + operand2->value.float_v->value;
  else
    result->value.float_v->value =
      operand1->value.float_v->value + operand2->value.int_v->value;

  return result;
}

AST_value_stack* operator_plus_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.int_v->value =
      operand1->value.int_v->value + (operand2->value.bool_v->value ? 1 : 0);
  else
    result->value.int_v->value =
      (operand1->value.bool_v->value ? 1 : 0) + operand2->value.int_v->value;

  return result;
}

AST_value_stack* operator_plus_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value =
    operand1->value.float_v->value + operand2->value.float_v->value;

  return result;
}

AST_value_stack* operator_plus_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_FLOAT)
    result->value.int_v->value = 
      operand1->value.float_v->value + (operand2->value.bool_v->value ? 1 : 0);
  else
    result->value.int_v->value = 
      (operand1->value.bool_v->value ? 1 : 0) + operand2->value.float_v->value;

  return result;
}

AST_value_stack* operator_plus_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    (operand1->value.bool_v->value ? 1 : 0) 
    + (operand2->value.bool_v->value ? 1 : 0);

  return result;
}

AST_value_stack* operator_plus_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix_value = operand1->value.matrix_v;
  if (
      matrix_value->row_size != operand2->value.matrix_v->row_size
      || matrix_value->col_size != operand2->value.matrix_v->col_size
     )
  {
    orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
  }

  int matrix_size = 
    matrix_value->row_size * matrix_value->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix_value->row_size;
  result->value.matrix_v->col_size = matrix_value->col_size;

  result->value.matrix_v->value = 
    malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i ++)
  {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_plus(
        new_value->value.value_v->stack, 
        matrix_value->value[i]->value.value_v->stack, 
        operand2->value.matrix_v->value[i]->value.value_v->stack
        );
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

AST_value_stack* operator_plus_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix_value = 
    operand1->type == AST_VALUE_MATRIX ?
    operand1->value.matrix_v
    : operand2->value.matrix_v;
  int matrix_size = 
    matrix_value->row_size * matrix_value->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix_value->row_size;
  result->value.matrix_v->col_size = matrix_value->col_size;

  result->value.matrix_v->value = 
    malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i ++)
  {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_plus(
        new_value->value.value_v->stack, 
        operand1->type == AST_VALUE_MATRIX ? 
        matrix_value->value[i]->value.value_v->stack
        : operand1, 
        operand2->type == AST_VALUE_MATRIX ? 
        matrix_value->value[i]->value.value_v->stack
        : operand2
        );
    //         free(matrix_value->value[i]->value.value_v);
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

// Minus
AST_value_stack* operator_minus_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    operand1->value.int_v->value - operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_minus_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.float_v->value =
      operand1->value.int_v->value - operand2->value.float_v->value;
  else
    result->value.float_v->value =
      operand1->value.float_v->value - operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_minus_float_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.float_v->value =
      operand1->value.int_v->value - operand2->value.float_v->value;
  else
    result->value.float_v->value =
      operand1->value.float_v->value - operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_minus_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.int_v->value =
      operand1->value.int_v->value - (operand2->value.bool_v->value ? 1 : 0);
  else
    result->value.int_v->value =
      operand1->value.float_v->value - (operand2->value.bool_v->value ? 1 : 0);
  return result;
}

AST_value_stack* operator_minus_bool_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.int_v->value =
      (operand1->value.bool_v->value ? 1 : 0) - operand2->value.int_v->value;
  else
    result->value.int_v->value =
      (operand1->value.bool_v->value ? 1 : 0) - operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_minus_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value =
    operand1->value.float_v->value - operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_minus_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_FLOAT)
    result->value.float_v->value =
      operand1->value.float_v->value - (operand2->value.bool_v->value ? 1.0 : 0.0);
  else
    result->value.float_v->value =
      (operand1->value.bool_v->value ? 1.0 : 0.0) - operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_minus_bool_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_FLOAT)
    result->value.float_v->value =
      (operand1->value.bool_v->value ? 1.0 : 0.0) - operand2->value.float_v->value;
  else
    result->value.float_v->value =
      (operand1->value.bool_v->value ? 1.0 : 0.0) - operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_minus_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    (operand1->value.bool_v->value ? 1 : 0) - (operand2->value.bool_v->value ? 1 : 0);
  return result;
}

AST_value_stack* operator_minus_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix1 = operand1->value.matrix_v;
  AST_matrix* matrix2 = operand2->value.matrix_v;

  if (matrix1->row_size != matrix2->row_size || matrix1->col_size != matrix2->col_size) {
    orora_error("에러, 행렬 뺄셈시 크기가 같아야 함", (void*) 0);
    return (void*) 0;
  }

  int matrix_size = matrix1->row_size * matrix1->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix1->row_size;
  result->value.matrix_v->col_size = matrix1->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_minus(
        new_value->value.value_v->stack,
        matrix1->value[i]->value.value_v->stack,
        matrix2->value[i]->value.value_v->stack
        );
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

AST_value_stack* operator_minus_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix = (operand1->type == AST_VALUE_MATRIX) ? operand1->value.matrix_v : operand2->value.matrix_v;
  int matrix_size = matrix->row_size * matrix->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix->row_size;
  result->value.matrix_v->col_size = matrix->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_minus(
        new_value->value.value_v->stack,
        (operand1->type == AST_VALUE_MATRIX) ? matrix->value[i]->value.value_v->stack : operand1,
        (operand2->type == AST_VALUE_MATRIX) ? matrix->value[i]->value.value_v->stack : operand2
        );
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

// Product
//     AST_matrix* matrix_value = operand1->value.matrix_v;
//     if (
//         matrix_value->row_size != operand2->value.matrix_v->row_size
//         || matrix_value->col_size != operand2->value.matrix_v->col_size
//        )
//     {
//       orora_error("에러, 행렬 덧셈시 크기가 같아야 함", (void*) 0);
// //         printf("에러, 행렬 덧셈시 크기가 같아야 함\n");
// //         exit(1);
//     }
// 
//     int matrix_size = 
//       matrix_value->row_size * matrix_value->col_size;
// 
//     result->type = AST_VALUE_MATRIX;
//     result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
//     result->value.matrix_v->row_size = matrix_value->row_size;
//     result->value.matrix_v->col_size = matrix_value->col_size;
// 
//     result->value.matrix_v->value = 
//       malloc(matrix_size * sizeof(struct ast_t));
// 
//     for (int i = 0; i < matrix_size; i ++)
//     {
//       AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
//       new_value->value.value_v = init_ast_value();
//       new_value->value.value_v->size = 1;
//       new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
//       visitor_operator_product(
//           new_value->value.value_v->stack, 
//           matrix_value->value[i]->value.value_v->stack, 
//           operand2->value.matrix_v->value[i]->value.value_v->stack
//         );
//       result->value.matrix_v->value[i] = new_value;
//     }
// 
//     return result;
AST_value_stack* operator_product_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    operand1->value.int_v->value * operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_product_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.float_v->value =
      operand1->value.int_v->value * operand2->value.float_v->value;
  else
    result->value.float_v->value =
      operand1->value.float_v->value * operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_product_float_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.float_v->value =
      operand1->value.int_v->value * operand2->value.float_v->value;
  else
    result->value.float_v->value =
      operand1->value.float_v->value * operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_product_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  if (operand1->type == AST_VALUE_INT)
    result->value.int_v->value =
      operand1->value.int_v->value * (operand2->value.bool_v->value ? 1 : 0);
  else
    result->value.int_v->value =
      operand1->value.float_v->value * (operand2->value.bool_v->value ? 1 : 0);
  return result;
}

AST_value_stack* operator_product_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value =
    operand1->value.float_v->value * operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_product_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value =
    operand1->value.float_v->value * (operand2->value.bool_v->value ? 1.0 : 0.0);
  return result;
}

AST_value_stack* operator_product_bool_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  if (operand1->type == AST_VALUE_FLOAT)
    result->value.float_v->value =
      (operand1->value.bool_v->value ? 1.0 : 0.0) * operand2->value.float_v->value;
  else
    result->value.float_v->value =
      (operand1->value.bool_v->value ? 1.0 : 0.0) * operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_product_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    (operand1->value.bool_v->value ? 1 : 0) * (operand2->value.bool_v->value ? 1 : 0);
  return result;
}

AST_value_stack* operator_product_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix1 = operand1->value.matrix_v;
  AST_matrix* matrix2 = operand2->value.matrix_v;

  if (matrix1->col_size != matrix2->row_size) {
    orora_error("에러, 행렬의 곱셈시 첫 행렬의 열과 두번째 행렬의 행의 크기가 같아야 함", (void*) 0);
    return (void*) 0;
  }

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix1->row_size;
  result->value.matrix_v->col_size = matrix2->col_size;
  int result_size = result->value.matrix_v->row_size * result->value.matrix_v->col_size;
  result->value.matrix_v->value = malloc(result_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix1->row_size; i++) {
    for (int j = 0; j < matrix2->col_size; j++) {
      AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
      new_value->value.value_v = init_ast_value();
      new_value->value.value_v->size = 1;
      new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
      new_value->value.value_v->stack->type = AST_VALUE_INT;
      new_value->value.value_v->stack->value.int_v = malloc(sizeof(struct ast_int_t));
      new_value->value.value_v->stack->value.int_v->value = 0;

      for (int k = 0; k < matrix1->col_size; k++) {
        AST_value_stack* temp_result = malloc(sizeof(AST_value_stack));
        visitor_operator_product(
            temp_result,
            matrix1->value[i * matrix1->col_size + k]->value.value_v->stack,
            matrix2->value[k * matrix2->col_size + j]->value.value_v->stack
            );

        AST_value_stack* sum_result = malloc(sizeof(AST_value_stack));
        visitor_operator_plus(
            sum_result,
            new_value->value.value_v->stack,
            temp_result
            );

        free(new_value->value.value_v->stack);
        new_value->value.value_v->stack = sum_result;

        free(temp_result);
      }

      result->value.matrix_v->value[i * matrix2->col_size + j] = new_value;
    }
  }

  return result;
}

AST_value_stack* operator_product_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix = (operand1->type == AST_VALUE_MATRIX) ? operand1->value.matrix_v : operand2->value.matrix_v;
  AST_value_stack* scalar = (operand1->type == AST_VALUE_MATRIX) ? operand2 : operand1;
  int matrix_size = matrix->row_size * matrix->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix->row_size;
  result->value.matrix_v->col_size = matrix->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_product(
        new_value->value.value_v->stack,
        matrix->value[i]->value.value_v->stack,
        scalar
        );
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

AST_value_stack* operator_product_string_string(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_STRING;
  AST_string* string_v = malloc(sizeof(struct ast_string_t));
  string_v->value_length =
    operand1->value.string_v->value_length +
    operand2->value.string_v->value_length - 1; // -1 for the extra quote
  string_v->real_value =
    malloc((string_v->value_length - 3) * sizeof(char));
  string_v->real_value[0] = '\0';

  strcat(string_v->real_value, operand1->value.string_v->real_value);
  strcat(string_v->real_value, operand2->value.string_v->real_value);

  string_v->value =
    malloc((string_v->value_length - 1) * sizeof(char));
  string_v->value[0] = '\0';
  strcat(string_v->value, "\"");
  strcat(string_v->value, string_v->real_value);
  strcat(string_v->value, "\"");
  result->value.string_v = string_v;
  return result;
}

// Division
AST_value_stack* operator_div_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand2->value.int_v->value == 0) {
    orora_error("에러, 0으로 나눌 수 없음", (void*) 0);
    return (void*) 0;
  }
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    operand1->value.int_v->value / operand2->value.int_v->value;
  return result;
}

AST_value_stack* operator_div_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_INT) 
  {
    if (operand2->value.float_v->value == 0.0) 
    {
      orora_error("에러, 0으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    result->value.float_v->value =
      (float)operand1->value.int_v->value / operand2->value.float_v->value;
  } else 
  {
    if (operand2->value.float_v->value == 0.0) {
      orora_error("에러, 0으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    result->value.float_v->value =
      operand1->value.float_v->value / (float)operand2->value.int_v->value;
  }

  return result;
}

AST_value_stack* operator_div_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_INT)
  {
    if (!operand2->value.bool_v->value) 
    {
      orora_error("에러, 0(false)으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value =
      operand1->value.int_v->value / (operand2->value.bool_v->value ? 1 : 0);
  } else
  {
    if (!operand2->value.bool_v->value) 
    {
      orora_error("에러, 0(false)으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value =
      (operand1->value.bool_v->value ? 1 : 0) / operand2->value.int_v->value;
  }
  return result;
}

AST_value_stack* operator_div_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand2->value.float_v->value == 0.0) {
    orora_error("에러, 0으로 나눌 수 없음", (void*) 0);
    return (void*) 0;
  }
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value =
    operand1->value.float_v->value / operand2->value.float_v->value;
  return result;
}

AST_value_stack* operator_div_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_FLOAT)
  {
    if (!operand2->value.bool_v->value) 
    {
      orora_error("에러, 0(false)으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_FLOAT;
    result->value.float_v = malloc(sizeof(struct ast_float_t));
    result->value.float_v->value = operand1->value.float_v->value;
  } else
  {
    if (!operand2->value.bool_v->value) 
    {
      orora_error("에러, 0(false)으로 나눌 수 없음", (void*) 0);
      return (void*) 0;
    }
    result->type = AST_VALUE_INT;
    result->value.int_v = malloc(sizeof(struct ast_int_t));
    result->value.int_v->value = operand1->value.bool_v->value ? 1 : 0;
  }
  return result;
}

AST_value_stack* operator_div_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (!operand2->value.bool_v->value) {
    orora_error("에러, 0(false)으로 나눌 수 없음", (void*) 0);
    return (void*) 0;
  }
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value = operand1->value.bool_v->value ? 1 : 0;
  return result;
}

AST_value_stack* operator_div_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix1 = operand1->value.matrix_v;
  AST_matrix* matrix2 = operand2->value.matrix_v;

  if (matrix1->row_size != matrix2->row_size || matrix1->col_size != matrix2->col_size) {
    orora_error("에러, 행렬 나눗셈시 크기가 같아야 함", (void*) 0);
    return (void*) 0;
  }

  int matrix_size = matrix1->row_size * matrix1->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix1->row_size;
  result->value.matrix_v->col_size = matrix1->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    visitor_operator_div(
        new_value->value.value_v->stack,
        matrix1->value[i]->value.value_v->stack,
        matrix2->value[i]->value.value_v->stack
        );
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

AST_value_stack* operator_div_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix;
  AST_value_stack* scalar;
  int is_matrix_first;

  if (operand1->type == AST_VALUE_MATRIX) {
    matrix = operand1->value.matrix_v;
    scalar = operand2;
    is_matrix_first = 1;
  } else {
    matrix = operand2->value.matrix_v;
    scalar = operand1;
    is_matrix_first = 0;
  }

  int matrix_size = matrix->row_size * matrix->col_size;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix->row_size;
  result->value.matrix_v->col_size = matrix->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    if (is_matrix_first) {
      visitor_operator_div(
          new_value->value.value_v->stack,
          matrix->value[i]->value.value_v->stack,
          scalar
          );
    } else {
      visitor_operator_div(
          new_value->value.value_v->stack,
          scalar,
          matrix->value[i]->value.value_v->stack
          );
    }
    result->value.matrix_v->value[i] = new_value;
  }

  return result;
}

// Mod
AST_value_stack* operator_mod_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand2->value.int_v->value == 0) {
    orora_error("에러, 0으로 나눌 수 없음", (void*) 0);
    return (void*) 0;
  }
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value =
    operand1->value.int_v->value % operand2->value.int_v->value;
  return result;
}

// Colon
AST_value_stack* operator_colon_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  if (operand1->value.int_v->value > operand2->value.int_v->value)
  {
    result->value.matrix_v->row_size = operand1->value.int_v->value - operand2->value.int_v->value + 1;
    result->value.matrix_v->col_size = 1;
    result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));
    int temp = operand1->value.int_v->value;

    while (temp >= operand2->value.int_v->value)
    {
      AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
      new_value->value.value_v = init_ast_value();
      new_value->value.value_v->size = 1;
      new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
      new_value->value.value_v->stack->type = AST_VALUE_INT;
      new_value->value.value_v->stack->value.int_v = malloc(sizeof(struct ast_int_t));
      new_value->value.value_v->stack->value.int_v->value = temp;
      result->value.matrix_v->value[operand1->value.int_v->value - temp] = new_value;
      temp--;
    }
  } else
  {
    result->value.matrix_v->row_size = operand2->value.int_v->value - operand1->value.int_v->value + 1;
    result->value.matrix_v->col_size = 1;
    result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));
    int temp = operand1->value.int_v->value;

    while (temp <= operand2->value.int_v->value)
    {
      AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
      new_value->value.value_v = init_ast_value();
      new_value->value.value_v->size = 1;
      new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
      new_value->value.value_v->stack->type = AST_VALUE_INT;
      new_value->value.value_v->stack->value.int_v = malloc(sizeof(struct ast_int_t));
      new_value->value.value_v->stack->value.int_v->value = temp;
      result->value.matrix_v->value[temp - operand1->value.int_v->value] = new_value;
      temp++;
    }
  }
  return result;
}

// Power
AST_value_stack* operator_power_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_INT;
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value = (int)pow(operand1->value.int_v->value,
      operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_power_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value = pow(operand1->value.int_v->value,
      operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_power_float_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value = pow(operand1->value.float_v->value,
      operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_power_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_FLOAT;
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value = pow(operand1->value.float_v->value,
      operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_power_matrix_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix = operand1->value.matrix_v;
  int exponent = operand2->value.int_v->value;

  // 전치 행렬 연산
  if (operand2->is_T == 1) {
    result->type = AST_VALUE_MATRIX;
    result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
    result->value.matrix_v->row_size = matrix->col_size;
    result->value.matrix_v->col_size = matrix->row_size;
    int matrix_size = matrix->row_size * matrix->col_size;
    result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

    for (int i = 0; i < matrix->row_size; i++) {
      for (int j = 0; j < matrix->col_size; j++) {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        *(new_value->value.value_v->stack) = *(matrix->value[j * matrix->row_size + i]->value.value_v->stack);
        result->value.matrix_v->value[i * matrix->col_size + j] = new_value;
      }
    }
    return result;
  }

  // 역행렬 연산 (지원하지 않음)
  if (exponent == -1) {
    orora_error("에러, 행렬의 역행렬은 아직 지원하지 않음", (void*) 0);
    return (void*) 0;
  }

  // 일반적인 거듭제곱 연산
  if (matrix->row_size != matrix->col_size) {
    orora_error("에러, 행렬 거듭제곱은 정사각 행렬에만 적용 가능", (void*) 0);
    return (void*) 0;
  }

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = matrix->row_size;
  result->value.matrix_v->col_size = matrix->col_size;
  int matrix_size = matrix->row_size * matrix->col_size;
  result->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

  // 단위 행렬로 초기화
  for (int i = 0; i < matrix_size; i++) {
    AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
    new_value->value.value_v = init_ast_value();
    new_value->value.value_v->size = 1;
    new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
    new_value->value.value_v->stack->type = AST_VALUE_INT;
    new_value->value.value_v->stack->value.int_v = malloc(sizeof(struct ast_int_t));
    new_value->value.value_v->stack->value.int_v->value = (i % (matrix->row_size + 1) == 0) ? 1 : 0;
    result->value.matrix_v->value[i] = new_value;
  }

  // 거듭제곱 연산
  for (int k = 0; k < exponent; k++) {
    AST_value_stack* temp = malloc(sizeof(AST_value_stack));
    temp->type = AST_VALUE_MATRIX;
    temp->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
    temp->value.matrix_v->row_size = matrix->row_size;
    temp->value.matrix_v->col_size = matrix->col_size;
    temp->value.matrix_v->value = malloc(matrix_size * sizeof(struct ast_t));

    for (int i = 0; i < matrix->row_size; i++) {
      for (int j = 0; j < matrix->col_size; j++) {
        AST* new_value = init_ast(AST_VALUE, (void*) 0, (void*) 0);
        new_value->value.value_v = init_ast_value();
        new_value->value.value_v->size = 1;
        new_value->value.value_v->stack = malloc(sizeof(AST_value_stack));
        new_value->value.value_v->stack->type = AST_VALUE_INT;
        new_value->value.value_v->stack->value.int_v = malloc(sizeof(struct ast_int_t));
        new_value->value.value_v->stack->value.int_v->value = 0;

        for (int m = 0; m < matrix->col_size; m++) {
          AST_value_stack* product = malloc(sizeof(AST_value_stack));
          visitor_operator_product(
              product,
              result->value.matrix_v->value[i * matrix->col_size + m]->value.value_v->stack,
              matrix->value[m * matrix->col_size + j]->value.value_v->stack
              );

          AST_value_stack* sum = malloc(sizeof(AST_value_stack));
          visitor_operator_plus(
              sum,
              new_value->value.value_v->stack,
              product
              );

          free(new_value->value.value_v->stack);
          new_value->value.value_v->stack = sum;

          free(product);
        }

        temp->value.matrix_v->value[i * matrix->col_size + j] = new_value;
      }
    }

    // 결과를 temp에서 result로 복사
    for (int i = 0; i < matrix_size; i++) {
      free(result->value.matrix_v->value[i]);
      result->value.matrix_v->value[i] = temp->value.matrix_v->value[i];
    }

    free(temp->value.matrix_v);
    free(temp);
  }

  return result;
}

AST_value_stack* operator_power_string_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  int repeat_count = operand2->value.int_v->value;

  if (repeat_count < 0) {
    orora_error("에러, 문자열 반복 횟수는 0 이상이어야 함", (void*) 0);
    return (void*) 0;
  }

  result->type = AST_VALUE_STRING;
  AST_string* string_v = malloc(sizeof(struct ast_string_t));

  size_t original_length = strlen(operand1->value.string_v->real_value);
  string_v->value_length = (original_length * repeat_count) + 3;    

  string_v->real_value = malloc(string_v->value_length - 3);
  string_v->real_value[0] = '\0';

  for (int i = 0; i < repeat_count; i++) {
    strcat(string_v->real_value, operand1->value.string_v->real_value);
  }

  string_v->value = malloc(string_v->value_length);
  string_v->value[0] = '\0';

  strcat(string_v->value, "\"");
  strcat(string_v->value, string_v->real_value);
  strcat(string_v->value, "\"");

  result->value.string_v = string_v;
  return result;
}

// Equal
AST_value_stack* operator_equal_null_null(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = true;
  return result;
}

AST_value_stack* operator_equal_string_string(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (strcmp(operand1->value.string_v->value, operand2->value.string_v->value) == 0);
  return result;
}

AST_value_stack* operator_equal_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.int_v->value == operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_equal_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_INT)
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      ((float)operand1->value.int_v->value == operand2->value.float_v->value);
  }
  else
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      (operand1->value.float_v->value == (float)operand2->value.int_v->value);
  }
  return result;
}

AST_value_stack* operator_equal_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_INT)
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      (operand1->value.int_v->value == (operand2->value.bool_v->value ? 1 : 0));
  } else
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      ((operand1->value.bool_v->value ? 1 : 0) == operand2->value.int_v->value);
  }
  return result;
}

AST_value_stack* operator_equal_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value == operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_equal_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->type == AST_VALUE_FLOAT)
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      (operand1->value.float_v->value == (operand2->value.bool_v->value ? 1.0 : 0.0));
  } else
  {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = 
      ((operand1->value.bool_v->value ? 1.0 : 0.0) == operand2->value.float_v->value);
  }
  return result;
}

AST_value_stack* operator_equal_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.bool_v->value == operand2->value.bool_v->value);
  return result;
}

AST_value_stack* operator_equal_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST_matrix* matrix1 = operand1->value.matrix_v;
  AST_matrix* matrix2 = operand2->value.matrix_v;

  if (matrix1->row_size != matrix2->row_size || matrix1->col_size != matrix2->col_size) {
    result->type = AST_VALUE_BOOL;
    result->value.bool_v = malloc(sizeof(struct ast_bool_t));
    result->value.bool_v->value = false;
    return result;
  }

  int matrix_size = matrix1->row_size * matrix1->col_size;
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = true;

  for (int i = 0; i < matrix_size; i++) {
    AST_value_stack* comparison_result = malloc(sizeof(AST_value_stack));
    visitor_operator_equal(
        comparison_result,
        matrix1->value[i]->value.value_v->stack,
        matrix2->value[i]->value.value_v->stack
        );

    if (!comparison_result->value.bool_v->value) {
      result->value.bool_v->value = false;
      free(comparison_result);
      break;
    }

    free(comparison_result);
  }

  return result;
}

// Less
AST_value_stack* operator_less_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.int_v->value < operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_less_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((float)operand1->value.int_v->value < operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_less_float_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value < (float)operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_less_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.int_v->value < (operand2->value.bool_v->value ? 1 : 0));
  return result;
}

AST_value_stack* operator_less_bool_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((operand1->value.bool_v->value ? 1 : 0) < operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_less_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value < operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_less_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value < (operand2->value.bool_v->value ? 1.0 : 0.0));
  return result;
}

AST_value_stack* operator_less_bool_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((operand1->value.bool_v->value ? 1.0 : 0.0) < operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_less_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (!operand1->value.bool_v->value && operand2->value.bool_v->value);
  return result;
}

// Greater
AST_value_stack* operator_greater_int_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.int_v->value > operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_greater_int_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((float)operand1->value.int_v->value > operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_greater_float_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value > (float)operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_greater_int_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.int_v->value > (operand2->value.bool_v->value ? 1 : 0));
  return result;
}

AST_value_stack* operator_greater_bool_int(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((operand1->value.bool_v->value ? 1 : 0) > operand2->value.int_v->value);
  return result;
}

AST_value_stack* operator_greater_float_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value > operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_greater_float_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.float_v->value > (operand2->value.bool_v->value ? 1.0 : 0.0));
  return result;
}

AST_value_stack* operator_greater_bool_float(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    ((operand1->value.bool_v->value ? 1.0 : 0.0) > operand2->value.float_v->value);
  return result;
}

AST_value_stack* operator_greater_bool_bool(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = 
    (operand1->value.bool_v->value && !operand2->value.bool_v->value);
  return result;
}

// Comma
AST_value_stack* operator_comma_v_v(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  AST* v1 = init_ast(AST_VALUE, (void*) 0, (void*) 0);
  v1->value.value_v = init_ast_value();
  v1->value.value_v->size = 1;
  v1->value.value_v->stack = malloc(sizeof(AST_value_stack));
  *(v1->value.value_v->stack) = *operand1;
  AST* v2 = init_ast(AST_VALUE, (void*) 0, (void*) 0);
  v2->value.value_v = init_ast_value();
  v2->value.value_v->size = 1;
  v2->value.value_v->stack = malloc(sizeof(AST_value_stack));
  *(v2->value.value_v->stack) = *operand2;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = 2;
  result->value.matrix_v->col_size = 1;
  result->value.matrix_v->value = malloc(2 * sizeof(struct ast_t));
  result->value.matrix_v->value[0] = v1;
  result->value.matrix_v->value[1] = v2;
  return result;
}

AST_value_stack* operator_comma_v_m(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand2->value.matrix_v->col_size != 1) 
  {
    orora_error("에러, 행렬의 행 크기는 1이어야 함", (void*) 0);
    return (void*) 0;
  }

  AST* v1 = init_ast(AST_VALUE, (void*) 0, (void*) 0);
  v1->value.value_v = init_ast_value();
  v1->value.value_v->size = 1;
  v1->value.value_v->stack = malloc(sizeof(AST_value_stack));
  *(v1->value.value_v->stack) = *operand1;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = operand2->value.matrix_v->row_size + 1;
  result->value.matrix_v->col_size = 1;
  result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));
  result->value.matrix_v->value[0] = v1;

  for (int i = 0; i < operand2->value.matrix_v->row_size; i++) 
  {
    result->value.matrix_v->value[i+1] = operand2->value.matrix_v->value[i];
  }

  return result;
}

AST_value_stack* operator_comma_m_v(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->value.matrix_v->col_size != 1)
  {
    orora_error("에러, 행렬의 행 크기는 1이어야 함", (void*) 0);
    return (void*) 0;
  }

  AST* v2 = init_ast(AST_VALUE, (void*) 0, (void*) 0);
  v2->value.value_v = init_ast_value();
  v2->value.value_v->size = 1;
  v2->value.value_v->stack = malloc(sizeof(AST_value_stack));
  *(v2->value.value_v->stack) = *operand2;

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = operand1->value.matrix_v->row_size + 1;
  result->value.matrix_v->col_size = 1;
  result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * sizeof(struct ast_t));

  for (int i = 0; i < operand1->value.matrix_v->row_size; i++)
  {
    result->value.matrix_v->value[i] = operand1->value.matrix_v->value[i];
  }
  result->value.matrix_v->value[operand1->value.matrix_v->row_size] = v2;

  return result;
}

AST_value_stack* operator_comma_m_m(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  if (operand1->value.matrix_v->col_size != operand2->value.matrix_v->col_size) {
    orora_error("에러, 행렬의 행 크기가 같아야 함", (void*) 0);
    return (void*) 0;
  }

  result->type = AST_VALUE_MATRIX;
  result->value.matrix_v = malloc(sizeof(struct ast_matrix_t));
  result->value.matrix_v->row_size = operand1->value.matrix_v->row_size + operand2->value.matrix_v->row_size;
  result->value.matrix_v->col_size = operand1->value.matrix_v->col_size;
  result->value.matrix_v->value = malloc(result->value.matrix_v->row_size * result->value.matrix_v->col_size * sizeof(struct ast_t));

  for (int i = 0; i < operand1->value.matrix_v->col_size; i++) 
  {
    for (int j = 0; j < operand1->value.matrix_v->row_size; j++) 
    {
      result->value.matrix_v->value[i * result->value.matrix_v->row_size + j] = operand1->value.matrix_v->value[i * operand1->value.matrix_v->row_size + j];
    }
    for (int j = 0; j < operand2->value.matrix_v->row_size; j++) 
    {
      result->value.matrix_v->value[i * result->value.matrix_v->row_size + j + operand1->value.matrix_v->row_size] = operand2->value.matrix_v->value[i * operand2->value.matrix_v->row_size + j];
    }
  }

  return result;
}

// In
AST_value_stack* operator_in_int_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    if (operand1->value.int_v->value == operand2->value.matrix_v->value[i]->value.value_v->stack->value.int_v->value) 
    {
      result->value.bool_v->value = true;
      break;
    }
  }

  return result;
}

AST_value_stack* operator_in_float_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    if (operand1->value.float_v->value == operand2->value.matrix_v->value[i]->value.value_v->stack->value.float_v->value) 
    {
      result->value.bool_v->value = true;
      break;
    }
  }

  return result;
}

AST_value_stack* operator_in_string_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    if (strcmp(operand1->value.string_v->value, operand2->value.matrix_v->value[i]->value.value_v->stack->value.string_v->value) == 0) 
    {
      result->value.bool_v->value = true;
      break;
    }
  }

  return result;
}

AST_value_stack* operator_in_bool_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    if (operand1->value.bool_v->value == (operand2->value.matrix_v->value[i]->value.value_v->stack->value.bool_v->value ? 1 : 0)) 
    {
      result->value.bool_v->value = true;
      break;
    }
  }

  return result;
}

AST_value_stack* operator_in_matrix_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    AST_value_stack* comparison_result = malloc(sizeof(AST_value_stack));
    visitor_operator_equal(
        comparison_result,
        operand1,
        operand2->value.matrix_v->value[i]->value.value_v->stack
        );

    if (comparison_result->value.bool_v->value) 
    {
      result->value.bool_v->value = true;
      free(comparison_result);
      break;
    }

    free(comparison_result);
  }

  return result;
}

AST_value_stack* operator_in_null_matrix(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;
  
  for (int i = 0; i < operand2->value.matrix_v->row_size * operand2->value.matrix_v->col_size; i++) 
  {
    if (operand2->value.matrix_v->value[i]->type == AST_NULL) 
    {
      result->value.bool_v->value = true;
      break;
    }
  }

  return result;
}

AST_value_stack* operator_in_v_string(AST_value_stack* result,
    AST_value_stack* operand1,
    AST_value_stack* operand2)
{
  result->type = AST_VALUE_BOOL;
  result->value.bool_v = malloc(sizeof(struct ast_bool_t));
  result->value.bool_v->value = false;

  if (strcmp(operand2->value.string_v->real_value, "real number") == 0)
  {
    if (operand1->type == AST_VALUE_INT || operand1->type == AST_VALUE_FLOAT)
    {
      result->value.bool_v->value = true;
    }
  }
  else if (strcmp(operand2->value.string_v->real_value, "integer") == 0)
  {
    if (operand1->type == AST_VALUE_INT)
    {
      result->value.bool_v->value = true;
    }
  }
  else if (strcmp(operand2->value.string_v->real_value, "natural number") == 0)
  {
    if (operand1->type == AST_VALUE_INT && operand1->value.int_v->value >= 0)
    {
      result->value.bool_v->value = true;
    }
  }
  else
  {
    orora_error("에러, 정의되지 않은 공간", (void*) 0);
  }
  return result;
}
