#ifndef SIMPLE_HTTP_SERVER_H
#define SIMPLE_HTTP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char* filename;
    char* body;
} post_content_t;

void start_server(int port, void (*post_cb) (post_content_t*));

#ifdef __cplusplus
}
#endif

#endif
