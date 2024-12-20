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

char *get_pow(void)
{
    char *hello;

    hello = "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE html> <html lang=\"fr\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Logo Centré</title> <style> body { margin: 0; height: 100vh; display: flex; justify-content: center; align-items: center; background-color: #f0f0f0; } .logo { width: 200px; height: 200px; border-radius: 50%; box-shadow: 0 4px 8px rgba(0,0,0,0.1); transition: transform 0.3s ease; } .logo:hover { transform: scale(1.1); } </style> </head> <body> <img src=\"https://nanbet.io/logo.png\" alt=\"Logo\" class=\"logo\"> <script> document.cookie = \"nom=valeur\"; </script> </body> </html>";
    return hello;
}

char *get_page(void)
{
    char *hello;

    hello = "HTTP/1.0 200 OK\r\n"
    "Server: webserver-c\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE html><html lang=\"fr\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width,initial-scale=1\"><title>Logo Centré</title><style>body{margin:0;height:100vh;display:flex;justify-content:center;align-items:center;background-color:#f0f0f0}.logo{width:200px;height:200px;border-radius:50%;box-shadow:0 4px 8px rgba(0,0,0,.1);transition:transform .3s ease}.logo:hover{transform:scale(1.1)}</style></head><body><img src=\"https://nanbet.io/logo.png\" alt=\"Logo\" class=\"logo\"><script>// Fonction pour définir un cookie function setCookie(name, value, days) { const date = new Date(); date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000)); const expires = \"expires=\" + date.toUTCString(); document.cookie = name + \"=\" + value + \";\" + expires + \";path=/\"; } // Définir le cookie pow=abcde (valide pour 30 jours) setCookie('pow', 'abcde', 30);</script></body></html>\r\n";
    return hello;
}
