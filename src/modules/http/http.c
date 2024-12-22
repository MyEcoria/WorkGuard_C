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
#include "../../../include/http.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

int accept_connection(int sd,
    struct sockaddr_in *network, int sizesock)
{
    int accepto = accept(sd, (struct sockaddr *)network,
    (socklen_t *)&sizesock);

    if (accepto < 0) {
        perror("webserver (accept)");
        return -1;
    }
    if (setup_nonblock(accepto) == -1) {
        close(accepto);
        return -1;
    }
    return accepto;
}

void init_server(server_t *server)
{
    server->sizesock = sizeof(struct sockaddr_in);
    server->network = malloc(sizeof(struct sockaddr_in));
    if (server->network == NULL) {
        perror("webserver (malloc)");
        exit(84);
    }
    server->sd = create_socket(server->network);
    server->bindo = create_bind(server->sd, server->network);
    server->listo = create_listen(server->sd);
    server->epfd = create_epollo(server->sd);
}
