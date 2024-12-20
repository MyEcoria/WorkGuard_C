/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** print a character
*/
#ifndef SOCKET_H
    #define SOCKET_H
    #define PORT 8070
    #define MAX_EVENTS 1024
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
typedef struct {
    int sizesock;
    int sd;
    int bindo;
    int listo;
    struct epoll_event events[MAX_EVENTS];
    int epfd;
    struct sockaddr_in *network;
} server_t;
void init_server(server_t *server);
void all_sockets(server_t *server);
int create_epollo(int sd);
int create_socket(struct sockaddr_in *network);
int write_in_socket(int accepto, char resp[]);
int all_sockets_detected(server_t *server, int i);
void all_sockets(server_t *server);
int setup_nonblock(int fd);
int create_bind(int sd, struct sockaddr_in *network);
int create_listen(int sd);
char *get_pow(void);
int accept_connection(int sd,
    struct sockaddr_in *network, int sizesock);
#endif
