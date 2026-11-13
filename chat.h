#pragma once

typedef enum {  CMD_HELP,
                CMD_LISTEN,
                CMD_CONNECT,
                CMD_EXIT,
                CMD_UNKNOWN} Command;

Command parse_command(const char *s);

//declare functions
int print_help();