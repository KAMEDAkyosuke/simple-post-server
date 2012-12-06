#include "simple-post-server.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include "intlist.h"
#include "stringlist.h"
#include "http-parser-helper.h"
#include "http-parser/http_parser.h"
#include "body-parser.h"

static void post_content_free(post_content_t* post_content)
{
    free(post_content->filename);
    free(post_content->body);
    free(post_content);
}

static int get_tcp_socket()
{
    int socket_fd;
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

static void set_enable_port_reuse(int sock)
{
    int on = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1){
        perror("setsockopt error");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

static void bind_socket(int sock, int port)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*) &addr, sizeof(addr)) == -1){
        perror("bind error");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

static void prepare_listen(int sock, int backlog)
{
    if(listen(sock, backlog) != 0){
        perror("listen error");
        exit(EXIT_FAILURE);
    }
}

static int accept_socket(int fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
    if(client_sd < 0){
        perror("accept error");
        exit(EXIT_FAILURE);
    }
    return client_sd;
}

static void read_socket(int fd, void (*post_cb) (post_content_t*))
{
    char buf[1024] = {};
    http_parser_helper_t* helper = http_parser_helper_init();
    while(true){
        memset(&buf, 0, sizeof(char) * 1024);
        ssize_t len = read(fd, &buf, sizeof(char) * 1024);
        if(len < 0){
            perror("read error");
            exit(EXIT_FAILURE);
        }
        else{
            http_parser_execute(&(helper->parser), &(helper->settings), buf, len);
            if(helper->is_done){
                break;
            }
        }
    }
    char* body = stringlist_concat(helper->body);
    post_content_t* post_content = body_parser_parse(body);
    post_cb(post_content);
    post_content_free(post_content);
    stringlist_concat_free(body);
    http_parser_helper_free(helper);
}

void start_server(int port, void (*post_cb) (post_content_t*))
{
    int listen_fd;
    listen_fd = get_tcp_socket();
    set_enable_port_reuse(listen_fd);
    bind_socket(listen_fd, port);
    prepare_listen(listen_fd, 5);

    intlist_t* socklist = intlist_add(NULL, listen_fd);

    fd_set readfds;
    int r;
    int max;
    while(true){
        FD_ZERO(&readfds);
        intlist_t* cursor = socklist;
        while(cursor != NULL){
            FD_SET(cursor->value, &readfds);
            cursor = cursor->next;
        }
        max = intlist_max(socklist);
        r = select(max + 1,
                   &readfds,
                   NULL,
                   NULL,
                   NULL);
        if(r == -1){
            perror("select error");
            exit(EXIT_FAILURE);
        }
        else{
            intlist_t* cursor = socklist;
            while(cursor != NULL){
                if(FD_ISSET(cursor->value, &readfds)){
                    if(cursor->value == listen_fd){
                        int client_fd = accept_socket(cursor->value);
                        intlist_add(socklist, client_fd);
                    }
                    else{
                        read_socket(cursor->value, post_cb);
                        // TODO: write HTTP/1.1 200 OK if read success
                        close(cursor->value);
                        intlist_del(socklist, cursor->value);
                    }
                }
                cursor = cursor->next;
            }
        }
    }
}
