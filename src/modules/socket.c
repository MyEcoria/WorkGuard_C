/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** print a character
*/
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "../../include/http.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

int create_socket(struct sockaddr_in *network)
{
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd == -1) {
        perror("webserver (socket)");
        exit(84);
    }
    network->sin_family = AF_INET;
    network->sin_port = htons(PORT);
    network->sin_addr.s_addr = htonl(INADDR_ANY);
    return sd;
}

int write_in_socket(int accepto, char resp[])
{
    int writo = write(accepto, resp, strlen(resp));

    if (writo == -1) {
        perror("webserver (write)");
        close(accepto);
        return -1;
    }
    shutdown(accepto, SHUT_WR);
    return writo;
}

int all_sockets_detected(server_t *server, int i)
{
    char *resp = get_pow();
    int accepto = 0;
    int writo = 0;

    if (server->events[i].data.fd == server->sd) {
        accepto = accept_connection(server->sd,
        server->network, server->sizesock);
        if (accepto == -1)
            return -1;
        writo = write_in_socket(accepto, resp);
        if (writo == -1)
            return -1;
        close(accepto);
    }
    return 0;
}

void all_sockets(server_t *server)
{
    int nfds = 0;

    for (;;) {
        nfds = epoll_wait(server->epfd, server->events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++) {
            all_sockets_detected(server, i);
        }
    }
    close(server->epfd);
    close(server->sd);
}
