#ifndef VISITOR_H
#define VISITOR_H

#include "../Parser/Ast/ast.h"
#include "../env.h"

void visitor_visit(Env* env, AST* ast);

#endif
