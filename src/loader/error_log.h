#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loader/main.h"
#include "lexer/lexer.h"
#include "loader/config.h"

const char* orora_error(const char* error_message, Parser* parser);

#endif
