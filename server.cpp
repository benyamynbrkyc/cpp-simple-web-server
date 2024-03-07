#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#define PORT 8080  // where the clients can reach at (this is the listening port from servers i worked on)
int main() {
    // establishing a connection and binding a socket to a port
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Cannot create socket");
        return 0;
    }

    if (server_fd == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;

    // htonl (whatever this is) converts a long integer (e.g. address) to a network representation
    // htons (whatever this is, again:)) converts a short integer (e.g. port) to a network representation

    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 0;
    }

    if (listen(server_fd, 3) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    socklen_t addrlen = sizeof(address);

    while (1) {
        printf("\n\n=========== Waiting for a connection ===========\n\n");

        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[1024] = {0};

        int valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        if (valread < 0) {
            printf("No bytes are there to read.");
        }

        const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";

        write(new_socket, hello, strlen(hello));

        close(new_socket);
    }

    return 0;
}