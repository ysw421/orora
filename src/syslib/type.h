#ifndef TYPE_H
#define TYPE_H

#include "loader/env.h"
#include "loader/error_log.h"
#include "parser/ast/ast.h"
#include "visitor/visitor.h"
#include <string.h>

AST_value_stack* type_function(Envs* envs, AST_function* ast_function);

#endif
