#ifndef HTTP_PARSER_HELPER_H
#define HTTP_PARSER_HELPER_H

#include <stdbool.h>

#include "http-parser/http_parser.h"
#include "stringlist.h"

typedef struct {
  http_parser parser;
  http_parser_settings settings;
  stringlist_t* body;
  bool is_done;
} http_parser_helper_t;

#ifdef __cplusplus
extern "C" {
#endif

http_parser_helper_t* http_parser_helper_init(void);

void http_parser_helper_free(http_parser_helper_t* helper);

#ifdef __cplusplus
}
#endif

#endif
