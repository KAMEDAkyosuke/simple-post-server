#include "http-parser-helper.h"

#include <stdlib.h>

#include "http-parser/http_parser.h"

static int message_begin_cb (http_parser *p)
{
    return 0;
}

static int url_cb (http_parser *p, const char *buf, size_t len)
{
    return 0;
}

static int header_field_cb (http_parser *p, const char *buf, size_t len)
{
    return 0;
}

static int header_value_cb (http_parser *p, const char *buf, size_t len)
{
    return 0;
}

static int headers_complete_cb (http_parser *p)
{
    return 0;
}

static int body_cb (http_parser *p, const char *buf, size_t len)
{
    http_parser_helper_t* helper = (http_parser_helper_t*) p;
    helper->body = stringlist_add(helper->body, buf, len);
    return 0;
}

static int message_complete_cb (http_parser *p)
{
    http_parser_helper_t* helper = (http_parser_helper_t*) p;
    helper->is_done = true;
    return 0;
}

http_parser_helper_t* http_parser_helper_init()
{
    http_parser_helper_t* helper;
    helper = (http_parser_helper_t*) malloc(sizeof(http_parser_helper_t));

    http_parser_init(&helper->parser, HTTP_REQUEST);

    helper->settings.on_message_begin = message_begin_cb;
    helper->settings.on_url = url_cb;
    helper->settings.on_header_field = header_field_cb;
    helper->settings.on_header_value = header_value_cb;
    helper->settings.on_headers_complete = headers_complete_cb;
    helper->settings.on_body = body_cb;
    helper->settings.on_message_complete = message_complete_cb;    

    helper->body = NULL;
    helper->is_done = false;
    return helper;
}

void http_parser_helper_free(http_parser_helper_t* helper)
{
  stringlist_free(helper->body);
  helper->body = NULL;
  free(helper);
}
