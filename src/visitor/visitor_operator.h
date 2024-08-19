#ifndef VISITOR_OPERATOR_H
#define VISITOR_OPERATOR_H

#include "parser/ast/ast.h"
#include "loader/env.h"
#include "loader/error_log.h"
#include <math.h>

AST_value_stack* operator_plus_int_int(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_minus(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_product(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_div(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_mod(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_power(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_equal(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_less(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_greater(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);
AST_value_stack* visitor_operator_comma(AST_value_stack* result, AST_value_stack* operand1, AST_value_stack* operand2);

#endif
