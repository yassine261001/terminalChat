#include "chat.h"
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>

Command parse_command(const char *s) {
    if (strcmp(s, "help")    == 0) return CMD_HELP;
    if (strcmp(s, "listen")  == 0) return CMD_LISTEN;
    if (strcmp(s, "connect") == 0) return CMD_CONNECT;
    if (strcmp(s, "exit")    == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

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
        return -1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket");
        freeaddrinfo(res);
        return -1;
    }

    int value = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        freeaddrinfo(res);
        close(sockfd);
        return -1;
    }

    freeaddrinfo(res);

    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        return -1;
    }

    printf("You are listening on port 8080...\n");

    int conn = accept(sockfd, NULL, NULL);
    if (conn == -1) {
        perror("accept");
        close(sockfd);
        return -1;
    }

    close(sockfd);
    printf("Connected!\n");
    return conn;
}


int chat_connect() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char ip_addr[256];
    printf("> Enter an IP address: ");
    fflush(stdout);
    scanf("%255s", ip_addr);

    int status = getaddrinfo(ip_addr, "8080", &hints, &res);
    if (status != 0) {
        printf("getaddrinfo failed: %s}\n", gai_strerror(status));
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        freeaddrinfo(res);
        close(sockfd);
        return 1;
    }

    freeaddrinfo(res);
    return sockfd;
}