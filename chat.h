#pragma once

typedef enum {  CMD_HELP,
                CMD_LISTEN,
                CMD_CONNECT,
                CMD_EXIT,
                CMD_UNKNOWN } Command;

Command parse_command(const char *s);

void print_help();
int chat_listen();
int chat_connect();