#include <stdio.h>
#include <string.h>
#include <unistd.h> // close()
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints)); // zero out hints, no garbage values used
    hints.ai_family = AF_UNSPEC; // allows IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // fills local IP automatically

// finding address to bind on this machine
    int gai_result = getaddrinfo(NULL, "8080", &hints, &res);
    if (gai_result != 0) {
        printf("getaddrinfo failed: %s\n", gai_strerror(gai_result));
        return 1;

    }

    // listening socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(res); //if error, free the address info
        return 1;
    }

    // allow reuse of address/port after restart
    int value = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));

    // bind socket to port 8080
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    freeaddrinfo(res);

    // listening call
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        close(sockfd);
        return 1;
    }

    printf("Server listening on port 8080... \n");

    // main server loop - accept and handle one client at a time
    while (true) {
        struct sockaddr_storage client_addr;
        socklen_t addr_size = sizeof(client_addr);

        int new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_size);
        if (new_sockfd == -1) {
            perror("accept");
            continue; // skip client, serve others
        }

        // receive from client
        char buf[1024];
        int n = recv(new_sockfd, buf, sizeof(buf) - 1, 0);
        if (n > 0) {
            buf[n] = '\0'; // null terminate
            printf("Received: %s\n", buf);
        }

        // send reply back
        char *reply = "Message from server";
        send(new_sockfd, reply, strlen(reply), 0);

        close(new_sockfd); //done with client, loop back
    }

    // unreachable while loop above runs forever
    close(sockfd);
    return 0;
}