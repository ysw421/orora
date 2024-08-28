#ifndef GET_SPACE_H
#define GET_SPACE_H

#include "utilities/utils.h"
#include "loader/config.h"
#include "syslib/print.h"
#include "loader/error_log.h"

#include "visitor/visitor.h"

AST_value_stack* get_space_function(Envs* envs, AST_function* ast_function);

#endif
