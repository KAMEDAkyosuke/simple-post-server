#include "body-parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    char* filename;
    stringlist_t* content;
} body_parser_t;

static void body_parser_free(body_parser_t* body)
{
    free(body->filename);
    stringlist_free(body->content);
}

static stringlist_t* split_with_lines(char* body)
{
    stringlist_t* lines = NULL;

    char* start = body;
    char* p = start;
    int len = 0;
    for(;;){
        if((*p) == '\0'){
            if(len != 0){
                lines = stringlist_add(lines, start, len);
            }
            break;
        }
        else{
            if((*p) == '\r' && (*(p + 1)) == '\n'){
                lines = stringlist_add(lines, start, len);
                start = p + 2;
                p = start;
                len = 0;
            }
            else if((*p) == '\n'){
                lines = stringlist_add(lines, start, len);
                start = p + 1;
                p = start;
                len = 0;
            }
            else{
                p++;
                len++;
            }
        }
    }
    return lines;
}

static void set_filename_if_find(body_parser_t* parser, char* line)
{
    const char* s = "filename=\"";
    char* p = strstr(line, s);
    if(p == NULL){
        return;
    }
    else{
        char* start = p + strlen(s);
        char* current = start;
        int len = 0;
        for(;;){
            if((*current) == '\"' ||
               (*current) == '\0' ||
               (*current) == '\r' ||
               (*current) == '\n'){
                break;
            }
            ++current;
            ++len;
        }
        if(len > 0){
            parser->filename = (char*) calloc(len+1, sizeof(char));
            memcpy(parser->filename, start, sizeof(char) * len);
        }
    }
}

post_content_t* body_parser_parse(char* body)
{
    post_content_t* post_content = (post_content_t*) malloc(sizeof(post_content_t));
    body_parser_t* parser = (body_parser_t*) malloc(sizeof(body_parser_t));
    parser->filename = NULL;
    parser->content = NULL;
    
    stringlist_t* lines = split_with_lines(body);
    char* boundary = lines->value;

    stringlist_t* tmp = lines->next;
    bool is_header = true;
    while(tmp != NULL){
        if(is_header){
            if(strcmp("", tmp->value) == 0){
                is_header = false;
            }
            else{
                set_filename_if_find(parser, tmp->value);
            }
        }
        else{
            if(strstr(tmp->value, boundary) != NULL){
                break;
            }
            else{
                parser->content = stringlist_add(parser->content, tmp->value, tmp->len);
            }
        }
        tmp = tmp->next;
    }
    stringlist_free(lines);

    post_content->filename = (char*)malloc(sizeof(char) * (strlen(parser->filename) + 1));
    strcpy(post_content->filename, parser->filename);
    char* s = stringlist_concat_with(parser->content, "\n");
    post_content->body = (char*)malloc(sizeof(char) * (strlen(s) + 1));
    strcpy(post_content->body, s);        
    stringlist_concat_free(s);
    body_parser_free(parser);
    free(parser);

    return post_content;
}

