#ifndef INTLIST_H
#define INTLIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct intlist {
    struct intlist* next;
    int value;
} intlist_t;

int intlist_max(intlist_t *lst);
intlist_t* intlist_add(intlist_t *lst, int i);
void intlist_free(intlist_t *lst);
intlist_t* intlist_del(intlist_t *lst, int i);


#ifdef __cplusplus
}
#endif

#endif
