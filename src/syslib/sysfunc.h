#ifndef SYSFUNC_H
#define SYSFUNC_H

#include "utilities/utils.h"
#include "loader/config.h"
#include "syslib/print.h"
#include "syslib/type.h"
#include "syslib/random.h"
#include "syslib/input.h"

AST_value_stack* check_sysfunc(Envs* envs, AST_function* ast_function);

#endif
