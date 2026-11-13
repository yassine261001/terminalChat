#include "chat.h"
#include <string.h>
#include <stdio.h>

Command parse_command(const char *s) {
    if (strcmp(s, "help")    == 0) return CMD_HELP;
    if (strcmp(s, "listen")  == 0) return CMD_LISTEN;
    if (strcmp(s, "connect") == 0) return CMD_CONNECT;
    if (strcmp(s, "exit")    == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

//code logic
int print_help() {
    printf("Help\n");
}