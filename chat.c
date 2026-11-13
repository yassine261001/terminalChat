#include "chat.h"
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

Command parse_command(const char *s) {
    if (strcmp(s, "help")    == 0) return CMD_HELP;
    if (strcmp(s, "listen")  == 0) return CMD_LISTEN;
    if (strcmp(s, "connect") == 0) return CMD_CONNECT;
    if (strcmp(s, "exit")    == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

void print_help() {
    printf( "\n"
            "Commands:\n"
            " 'listen'            Wait for a friend to connect (port 8080)\n"
            " 'connect'           Connect to a friend (ask for their IP)\n"
            " 'exit'              Quit the program\n"
            "\n"
            "In chat:\n"
            " '/quit'             Leave the chat and return to main menu\n"
            "\n");
    
}

// listen and bind
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

// connecting 
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

void chat_loop(int sock) {
    char buf[1024];

    // throw away leftover "enter" from scanf in the menu
    int c;
    while ((c = getchar()) != '\n' && c!= EOF);

    struct pollfd fds[2] = {
        { .fd = 0,    .events = POLLIN }, // user
        { .fd = sock, .events = POLLIN }, // participant
    };

    printf("> Chat started. Enter /quit to leave.\n");

    while (true) {
        poll(fds, 2, -1);

        // user types something
        if (fds[0].revents & POLLIN) {
            if (fgets(buf, sizeof buf, stdin) == NULL) break;
            if (strcmp(buf, "/quit\n") == 0) break;
            send(sock, buf, strlen(buf), 0);

            printf("\033[A\r\033[K");       // up one line, to start, erase it
            printf("You: %s", buf);         // rewrite it as "You: ..."
            printf("> ");                   // fresh input prompt
            fflush(stdout);
        }

        if (fds[1].revents & POLLIN) {
            int n = recv(sock, buf, sizeof buf - 1, 0);
            if (n <= 0) {
                printf("\nFriend disconnected\n");
                break;
            }
            buf[n] = '\0';
            printf("\r\033[K");
            printf("Friend: %s", buf);
            printf("> ");
            fflush(stdout);
        }
    }
    close(sock);
    printf("Left chat.\n");
}