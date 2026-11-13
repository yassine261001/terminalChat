#pragma once

typedef enum {  CMD_HELP,
                CMD_LISTEN,
                CMD_CONNECT,
                CMD_EXIT,
                CMD_UNKNOWN } Command;

Command parse_command(const char *s);

// command function called inside switch
void print_help();
int chat_listen();
int chat_connect();
void chat_loop(int sock);