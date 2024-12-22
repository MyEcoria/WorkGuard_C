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
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include "../include/server.h"

void handle_sigint(int sig)
{
    printf("\nCtrl+C détecté. Exit... (%d)\n", sig);
    exit(0);
}

int main(void)
{
    server_t *server = malloc(sizeof(server_t));

    signal(SIGINT, handle_sigint);
    if (server == NULL)
        return 84;
    start_server(server);
    return 0;
}
