#ifndef DAEMON_H
#define DAEMON_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "parser/parser_value.h"
#include "loader/env.h"
#include "loader/color.h"
#include "visitor/visitor.h"

#define PID_FILE "/tmp/orora_daemon.pid"
#define RESULT_FILE "/tmp/orora_result.txt"
#define FIFO_NAME "/tmp/orora_fifo"
#define BUFFER_SIZE 1024
#define PORT_FILE "/tmp/orora_port.txt"
#define SOCKET_PATH "/tmp/orora_socket"

#define DIALOGUE "%sorora> %s", ORORA_COLOR_H, ORORA_COLOR_RESET

int read_port_from_file();
void run_daemon();

#endif

