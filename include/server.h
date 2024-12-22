/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** print a character
*/
#ifndef SERVER_H
    #define SERVER_H
    #define PORT 3080
    #include "h2o.h"
typedef struct {
    h2o_globalconf_t config;
    h2o_context_t ctx;
    h2o_accept_ctx_t accept_ctx;
} server_t;
h2o_pathconf_t *register_handler(h2o_hostconf_t *hostconf,
    const char *path, int (*on_req)(h2o_handler_t *, h2o_req_t *));
int chunked_test(h2o_handler_t *self, h2o_req_t *req);
void on_accept(uv_stream_t *listener, int status);
int create_listener(server_t *server);
void start_server(server_t *server);
#endif
