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

int create_epollo(int sd)
{
    int epfd = epoll_create(MAX_EVENTS);
    struct epoll_event ev;
    int epollo = 0;

    ev.data.fd = sd;
    ev.events = EPOLLIN | EPOLLET;
    epollo = epoll_ctl(epfd, EPOLL_CTL_ADD, sd, &ev);
    if (epollo == -1) {
        perror("webserver (epoll_ctl)");
        close(epfd);
        close(sd);
        exit(84);
    }
    return epfd;
}
