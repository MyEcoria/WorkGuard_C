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

int setup_nonblock(int fd)
{
    int flags = 0;
    int change = 0;

    flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("webserver (flags)");
        return -1;
    }
    change = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (change == -1) {
        perror("webserver (change)");
        return -1;
    }
    return 0;
}

int create_bind(int sd, struct sockaddr_in *network)
{
    int bindo = 0;

    bindo = bind(sd, (struct sockaddr *)network, sizeof(struct sockaddr_in));
    if (bindo == -1) {
        perror("webserver (bind)");
        close(sd);
        exit(84);
    }
    printf("Success bind on: %d\n", PORT);
    return bindo;
}

int create_listen(int sd)
{
    int listo = listen(sd, SOMAXCONN);
    int non_block = 0;

    if (listo != 0) {
        perror("webserver (listen)");
        close(sd);
        return 84;
    }
    non_block = setup_nonblock(sd);
    if (non_block == -1) {
        close(sd);
        return 84;
    }
    return listo;
}

char *get_resp(void)
{
    char *hello;

    hello = "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/txt\r\n\r\n"
    "hello, world\r\n";
    return hello;
}
