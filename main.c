#include <stdio.h>

#include "simple-post-server.h"

void cb(post_content_t* post_content){
    printf("filename = %s\n", post_content->filename);
    printf("body = %s\n", post_content->body);
}

int main(int argc, char** argv)
{
    puts("hello world");

    start_server(8080, cb);
    return 0;
}
