#include "intlist.h"

#include <stdlib.h>

int intlist_max(intlist_t *lst)
{
    int max = 0;
    intlist_t* cursor = lst;
    while(cursor != NULL){
        max = max > cursor->value ? max : cursor->value;
        cursor = cursor->next;
    }
    return max;
}

intlist_t* intlist_add(intlist_t *lst, int i)
{
    intlist_t *new = malloc(sizeof(intlist_t));
    new->value = i;
    new->next = NULL;

    if(lst == NULL){
        return new;
    }
    else{
        intlist_t* cursor = lst;
        while(cursor->next != NULL){
            cursor = cursor->next;
        }
        cursor->next = new;
        return lst;
    }
}

void intlist_free(intlist_t *lst)
{
    intlist_t* cursor = lst;
    while(cursor != NULL){
        intlist_t* tmp = cursor;
        free(cursor);
        cursor = tmp->next;
    }
}

intlist_t* intlist_del(intlist_t *lst, int i)
{
    if(lst->value == i){
        intlist_t* r;
        r = lst->next;
        free(lst);
        return r;
    }
    else{
        intlist_t* pre = lst;
        intlist_t* cursor = lst->next;
        while(cursor != NULL){
            if(cursor->value == i){
                intlist_t* next = cursor->next;
                free(cursor);
                pre->next = next;
                break;
            }
            cursor = cursor->next;
        }
        return lst;
    }
}
