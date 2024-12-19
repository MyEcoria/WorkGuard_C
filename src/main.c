#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "../include/http.h"
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int setup_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("webserver (flags)");
        return -1;
    }
    int change = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (change == -1) {
        perror("webserver (change)");
        return -1;
    }
    return 0;
}

int main()
{
    char buffer[2048];
    int listen_sock, conn_sock, nfds, epollfd;
    char resp[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/txt\r\n\r\n"
                  "hello, world\r\n";
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("webserver (socket)");
        return 84;
    }

    struct sockaddr_in network;
    int sizesock = sizeof(struct sockaddr_in);
    network.sin_family = AF_INET;
    network.sin_port = htons(PORT);
    network.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindo = bind(sd, (struct sockaddr *)&network, sizeof(struct sockaddr_in));
    if (bindo == -1) {
        perror("webserver (bind)");
        return 84;
    }
    printf("Success bind on: %d\n", PORT);

    int listo = listen(sd, SOMAXCONN);
    if (listo != 0) {
        perror("webserver (listen)");
        return 84;
    }
    if (setup_nonblock(sd) == -1) {
        close(sd);
        return 84;
    }
    struct epoll_event ev, events[MAX_EVENTS];
    int epfd = epoll_create(MAX_EVENTS);
    ev.data.fd = sd;
    ev.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sd, &ev) == -1) {
        perror("webserver (epoll_ctl)");
        close(epfd);
        close(sd);
        return 84;
    }
    for (;;) {
        int nfds = epoll_wait(epfd, events, 5, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sd) {
                int accepto = accept(sd, (struct sockaddr *)&network, (socklen_t *)&sizesock);
                if (accepto < 0) {
                    perror("webserver (accept)");
                    continue;
                }
                if (setup_nonblock(accepto) == -1) {
                    close(sd);
                    return 84;
                }
                int writo = write(accepto, resp, strlen(resp));
                if (writo == -1) {
                    perror("webserver (write)");
                    continue;
                }
                shutdown(accepto, SHUT_WR);
                close(accepto);
            }
        }
    }
    close(epfd);
    close(sd);
    return 0;
}
