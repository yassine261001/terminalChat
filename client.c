#include <stdio.h>
#include <string.h>
#include <unistd.h>         // close()
#include <sys/socket.h>     // socket(), connect(), send(), recv()
#include <netdb.h>          // get addrinfo(); struct addrinfo

int main() {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;        // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // TCP

    // change 127.0.0.1 to servers IP
    int gai_result = getaddrinfo("127.0.0.1", "8080", &hints, &res);
    if (gai_result != 0) {
        printf("getaddrinfo failed: %s\n", gai_strerror(gai_result));
        return 1;
    }

    // create socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // connect to the server
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        freeaddrinfo(res);
        close(sockfd);
        return 1;
    }
    freeaddrinfo(res); // free address once connected

    // send message
    char *msg = "This is my first message";
    send(sockfd, msg, strlen(msg), 0);
    

    // receive server reply
    char buf[1024];
    int n = recv(sockfd, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0'; // null terminate to print as string
        printf("Server replied: %s\n", buf);
    }

    // clean up
    close(sockfd);
    return 0;
}
