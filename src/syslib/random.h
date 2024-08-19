#ifndef RANDOM_H
#define RANDOM_H

#include "utilities/utils.h"
#include "loader/config.h"
#include "syslib/print.h"
#include "loader/error_log.h"
#include <stdlib.h>

AST_value_stack* random_function(Envs* envs, AST_function* ast_function);

#endif
