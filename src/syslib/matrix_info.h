#ifndef MATRIX_INFO_H
#define MATRIX_INFO_H

#include "utilities/utils.h"
#include "loader/config.h"
#include "syslib/print.h"
#include "loader/error_log.h"

#include "visitor/visitor.h"

AST_value_stack* col_function(Envs* envs, AST_function* ast_function);
AST_value_stack* row_function(Envs* envs, AST_function* ast_function);

#endif
