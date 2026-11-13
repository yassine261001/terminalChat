#include "chat.h"
#include <stdio.h>

int main() {
    char cmd[256];
    bool run = true;

    while (run == true) {
        printf("> main menu: ");
        fflush(stdout);
        scanf("%255s", cmd);
        
        switch (parse_command(cmd)) {
  
        case CMD_HELP:
            print_help();
            break;

        case CMD_LISTEN: {
            int sock = chat_listen();
            if (sock != -1) chat_loop(sock);
            break;
        }

        case CMD_CONNECT: {
            int sock = chat_connect();
            if (sock != -1) chat_loop(sock);
            break;
        }

        case CMD_EXIT:
            run = false;
            break;

        case CMD_UNKNOWN:
            printf("Unknown command: %s\n", cmd);
            break;
        }
    }
}