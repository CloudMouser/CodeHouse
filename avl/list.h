#ifndef __LIST_H
#define __LIST_H
#include "define.h"
#include <stdlib.h>
#include <string.h>
typedef struct Listnode
{
    void *data;
    struct Listnode *pre;
    struct Listnode *next;
}Listnode;

typedef struct List
{
    uint32 count;
    Listnode *head;
    Listnode *tail;
}*List;
List ListCreate(void);
Listnode* ListPushBack(List list,void *data,uint32 size);
Listnode* ListPushFront(List list,void *data,uint32 size);
void ListPopBack(List list);
void ListPopFront(List list);
void* ListFront(List list);
void* ListBack(List list);
void* ListAt(List list,uint32 index);
void ListFlush(List list);
void ListDestroy(List *list);
#endif // __LIST_H
