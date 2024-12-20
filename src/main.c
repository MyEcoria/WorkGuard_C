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
#include "../include/http.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

void handle_sigint() {
    printf("\nCtrl+C détecté. Exit...\n");
    exit(0);
}

int main(void)
{
    server_t server;

    signal(SIGINT, handle_sigint);
    init_server(&server);
    all_sockets(&server);
    free(server.network);
    return 0;
}
