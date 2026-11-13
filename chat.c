#include "chat.h"
#include <string.h>
#include <stdio.h>
#include <netdb.h>

Command parse_command(const char *s) {
    if (strcmp(s, "help")    == 0) return CMD_HELP;
    if (strcmp(s, "listen")  == 0) return CMD_LISTEN;
    if (strcmp(s, "connect") == 0) return CMD_CONNECT;
    if (strcmp(s, "exit")    == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

//code logic
void print_help() {
    printf("Help\n");
}

int chat_listen() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(NULL, "8080", &hints, &res);
    if (status != 0) {
        printf("Get addrinfo failed: %s\n", gai_strerror(status));
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket");
        freeaddrinfo(status);
        return 1;
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    freeaddrinfo(res);

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    printf("You are istening on port 8080...\n");
}
