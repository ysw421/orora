#ifndef MAIN_H
#define MAIN_H

#include "Parser/Ast/ast.h"

#define EXTENTION ".orora"

// type tree...
extern orora_value_type* value_type_list;

void error(char* error_message, Parser* parser, Token* token);

#endif
