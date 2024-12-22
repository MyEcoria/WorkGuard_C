/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** print a character
*/
#include <errno.h>
#include <limits.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "../include/h2o.h"
#include "../include/h2o/http1.h"
#include "../include/h2o/http2.h"
#include "../include/h2o/memcached.h"
#include "../../../include/server.h"

h2o_pathconf_t *register_handler(h2o_hostconf_t *hostconf,
    const char *path, int (*on_req)(h2o_handler_t *, h2o_req_t *))
{
    h2o_pathconf_t *pathconf = h2o_config_register_path(hostconf, path, 0);
    h2o_handler_t *handler = h2o_create_handler(pathconf, sizeof(*handler));

    handler->on_req = on_req;
    return pathconf;
}

int chunked_test(h2o_handler_t *self, h2o_req_t *req)
{
    static h2o_generator_t generator = {NULL, NULL};
    h2o_iovec_t body;

    if (!h2o_memis(req->method.base, req->method.len, H2O_STRLIT("GET")))
        return -1;
    body = h2o_strdup(&req->pool, "hello world\n", SIZE_MAX);
    req->res.status = 200;
    req->res.reason = "OK";
    h2o_add_header(&req->pool, &req->res.headers, H2O_TOKEN_CONTENT_TYPE,
    NULL, H2O_STRLIT("text/plain"));
    h2o_start_response(req, &generator);
    h2o_send(req, &body, 1, 1);
    return 0;
}

void on_accept(uv_stream_t *listener, int status)
{
    uv_tcp_t *conn;
    h2o_socket_t *sock;
    server_t *server = listener->data;

    if (status != 0)
        return;
    conn = h2o_mem_alloc(sizeof(*conn));
    uv_tcp_init(listener->loop, conn);
    if (uv_accept(listener, (uv_stream_t *)conn) != 0) {
        uv_close((uv_handle_t *)conn, (uv_close_cb)free);
        return;
    }
    sock = h2o_uv_socket_create((uv_handle_t *)conn, (uv_close_cb)free);
    h2o_accept(&server->accept_ctx, sock);
}

int create_listener(server_t *server)
{
    static uv_tcp_t listener;
    struct sockaddr_in addr;
    int r;

    uv_tcp_init(server->ctx.loop, &listener);
    uv_ip4_addr("127.0.0.1", 7891, &addr);
    r = uv_tcp_bind(&listener, (struct sockaddr *)&addr, 0);
    if (r != 0) {
        fprintf(stderr, "uv_tcp_bind:%s\n", uv_strerror(r));
        uv_close((uv_handle_t *)&listener, NULL);
        return r;
    }
    listener.data = server;
    r = uv_listen((uv_stream_t *)&listener, 128, on_accept);
    if (r != 0) {
        fprintf(stderr, "uv_listen:%s\n", uv_strerror(r));
        uv_close((uv_handle_t *)&listener, NULL);
        return r;
    }
    return 0;
}

void start_server(server_t *server)
{
    h2o_hostconf_t *hostconf;
    uv_loop_t loop;

    signal(SIGPIPE, SIG_IGN);
    h2o_config_init(&server->config);
    hostconf = h2o_config_register_host(&server->config,
    h2o_iovec_init(H2O_STRLIT("default")), 65535);
    register_handler(hostconf, "/", chunked_test);
    uv_loop_init(&loop);
    h2o_context_init(&server->ctx, &loop, &server->config);
    server->accept_ctx.ctx = &server->ctx;
    server->accept_ctx.hosts = server->config.hosts;
    if (create_listener(server) != 0) {
        fprintf(stderr, "failed to listen to 127.0.0.1:%d:%s\n",
        PORT, strerror(errno));
        exit(84);
    }
    uv_run(server->ctx.loop, UV_RUN_DEFAULT);
}
