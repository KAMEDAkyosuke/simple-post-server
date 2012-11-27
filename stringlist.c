#include "stringlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

stringlist_t* stringlist_add(stringlist_t* list, const char* value, const int len)
{
    stringlist_t* new_item = (stringlist_t*) malloc(sizeof(stringlist_t));
    if(new_item == NULL){
        return NULL;
    }
    
    new_item->next = NULL;
    new_item->value = (char*) malloc(sizeof(char) * (len + 1));
    if(new_item->value == NULL){
        free(new_item);
        new_item = NULL;
        stringlist_free(list);
        list = NULL;
        
        return NULL;
    }
    
    memset(new_item->value, 0, sizeof(char) * (len + 1));
    memcpy(new_item->value, value, sizeof(char) * len);
    new_item->len = len;
    
    if(list == NULL){
        return new_item;
    }
    else{    // 末尾に追加
        stringlist_t* last = list;
        while(last->next != NULL){
            last = last->next;
        }
        last->next = new_item;
        return list;
    }
}

void stringlist_free(stringlist_t* first)
{
    stringlist_t* list = first;
    while(list != NULL){
        free(list->value);
        list->value = NULL;
        
        stringlist_t* tmp = list;
        list = list->next;
        free(tmp);
        tmp = NULL;
    }
}

char* stringlist_concat(stringlist_t* first)
{
    int len = 0;
    stringlist_t* list = first;
    while(list != NULL){
        len += list->len;
        list = list->next;
    }
    
    if(len == 0){
        return NULL;
    }
    
    char* result = (char*) malloc(sizeof(char) * (len + 1));
    if(result == NULL){
        return NULL;
    }

    memset(result, 0, sizeof(char) * (len + 1));
  
    list = first;
    int pos = 0;
    while(list != NULL){
        memcpy(result + pos, list->value, sizeof(char) * list->len);
        pos += list->len;
        list = list->next;
    }
    return result;
}

char* stringlist_concat_with(stringlist_t* first, char* s)
{
    stringlist_t* list = NULL;
    stringlist_t* cursor = first;
    while(cursor != NULL){
        list = stringlist_add(list, cursor->value, cursor->len);
        if(cursor->next != NULL){
            list = stringlist_add(list, s, strlen(s));
        }
        cursor = cursor->next;
    }
    char* result = stringlist_concat(list);
    stringlist_free(list);
    return result;
}


void stringlist_concat_free(char* str)
{
    free(str);
}
