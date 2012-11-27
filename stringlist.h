#ifndef STRINGLIST_H
#define STRINGLIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stringlist{
  struct stringlist* next;
  char* value;
  int len;
} stringlist_t;

stringlist_t* stringlist_add(stringlist_t* list, const char* value, const int len);
void stringlist_free(stringlist_t* list);
char* stringlist_concat(stringlist_t* list);
char* stringlist_concat_with(stringlist_t* list, char* s);
void stringlist_concat_free(char* str);

#ifdef __cplusplus
}
#endif

#endif
