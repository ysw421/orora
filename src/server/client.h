#ifndef CLIENT_H
#define CLIENT_H

#include "server/daemon.h"

void run_client();
void* internal_client_thread(void* arg);

#endif
