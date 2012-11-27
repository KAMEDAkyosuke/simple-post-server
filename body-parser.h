#ifndef BODY_PARSER_H
#define BODY_PARSER_H

#include "stringlist.h"

#include "simple-post-server.h"

#ifdef __cplusplus
extern "C" {
#endif

post_content_t* body_parser_parse(char* body);

#ifdef __cplusplus
}
#endif

#endif
