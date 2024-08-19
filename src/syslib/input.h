#ifndef INPUT_H
#define INPUT_H

#include "utilities/utils.h"
#include "loader/config.h"
#include "syslib/print.h"
#include "loader/error_log.h"
#include <stdio.h>

AST_value_stack* input_function(Envs* envs, AST_function* ast_function);

#endif
