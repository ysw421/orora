#ifndef CONFIG_H
#define CONFIG_H

#include "loader/env.h"

extern bool INTERACTIVE_MODE;
extern int ORORA_PORT;
void rum_init_or(Envs* root_envs);

#endif
