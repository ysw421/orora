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
#include <unistd.h>
#include "parser/parser_value.h"
#include "loader/env.h"
#include "loader/color.h"
#include "visitor/visitor.h"
#include "server/status.h"
#include "syslib/print.h"
#include <setjmp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libwebsockets.h>

#define PID_FILE "/tmp/orora_daemon.pid"
#define RESULT_FILE "/tmp/orora_result.txt"
#define FIFO_NAME "/tmp/orora_fifo"
#define BUFFER_SIZE 1024
#define PORT_FILE "/tmp/orora_port.txt"
#define SOCKET_PATH "/tmp/orora_socket"

// #define BUFFER_SIZE 4096
#define MAX_PAYLOAD_SIZE 4096
#define MAX_PORT_ATTEMPTS 20
#define MAX_RETRY 5

#define ORORA_PROMPT "orora>> "
#define DIALOGUE "%s%s%s", ORORA_COLOR_H, ORORA_PROMPT, ORORA_COLOR_RESET

int read_port_from_file();
void run_daemon();
int is_end_interactive_line(Lexer* root);
void send_ws_message(const char* message);

#endif

