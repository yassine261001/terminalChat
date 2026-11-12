#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void chat() {
    char command[256];

    while (true) {
        printf("> enter 'listen' or 'connect': ");
        if (scanf("%255s", command) != 1) {
            break;
        }

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "listen") == 0) {
            printf("> You are now listening on Port 8080...\n");
        } else if (strcmp(command, "connect") == 0) {
            printf("> Enter an IP address to connect: ");
        } else {
            printf("> Unknown command: %s\n", command);
        }
    }
}


int main() {

    char command[256];

    while (true) {
        printf("> ");
        if (scanf("%255s", command) != 1) {
            break;
        }

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "start") == 0) {
            chat();
        } else {
            printf("> Unknown command: %s\n", command);
        }
    }
}
