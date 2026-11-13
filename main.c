#include "chat.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
    char cmd[256];
    bool run = true;

    while (run == true) {
        printf("> main menu: ");
        fflush(stdout);
        scanf("%255s", cmd);
        
        switch (parse_command(cmd))
        {
        case CMD_HELP:
            print_help();
            break;
        case CMD_LISTEN:
            printf("Listening on Port...\n");
            // enter chat loop
            break;
        case CMD_CONNECT:
            printf("Connect IP Port...\n");
            // enter connect loop
            break;
        case CMD_EXIT:
            run = false;
            break;
        case CMD_UNKNOWN:
            printf("Unknown command: %s\n", cmd);
            break;
        }
    }
}