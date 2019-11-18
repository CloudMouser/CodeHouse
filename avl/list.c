#include "list.h"

Listnode* ListnodeCreate(void *data,uint32 size)
{
    Listnode *node;
    node=malloc(sizeof(struct Listnode));
    if(node)
    {
        node->data=nullptr;
        if(data&&size)
        {
            node->data=malloc(size);
            if(node->data)
                memcpy(node->data,data,size);
            else
            {
                free(node);
                return nullptr;
            }
        }
        node->pre=nullptr;
        node->next=nullptr;
    }
    return node;
}

List ListCreate(void)
{
    List list;
    list=malloc(sizeof(struct List));
    if(list)
    {
        list->count=0;
        list->head=ListnodeCreate(nullptr,0);
        list->tail=ListnodeCreate(nullptr,0);
        if(list->head&&list->tail)
        {
            list->head->pre=nullptr;
            list->head->next=list->tail;
            list->tail->next=nullptr;
            list->tail->pre=list->head;
        }
        else
        {
            free(list->head);
            free(list->tail);
            free(list);
            return nullptr;
        }
    }
    return list;
}

Listnode* ListPushBack(List list, void* data, uint32 size)
{
    Listnode *node;
    if(list&&data&&size)
    {
        node=ListnodeCreate(data,size);
        if(node)
        {
            list->count++;
            node->pre=list->tail->pre;
            node->next=list->tail;
            list->tail->pre->next=node;
            list->tail->pre=node;
        }
        return node;
    }
    return nullptr;
}

Listnode* ListPushFront(List list, void* data, uint32 size)
{
    Listnode *node;
    if(list&&data&&size)
    {
        node=ListnodeCreate(data,size);
        if(node)
        {
            list->count++;
            node->next=list->head->next;
            node->pre=list->head;
            list->head->next->pre=node;
            list->head->next=node;
        }
        return node;
    }
    return nullptr;
}

void ListPopBack(List list)
{
    Listnode *node;
    if(list&&list->count)
    {
        list->count--;
        node=list->tail->pre;
        node->next->pre=node->pre;
        node->pre->next=node->next;
        free(node->data);
        free(node);
    }
}

void ListPopFront(List list)
{
    Listnode *node;
    if(list&&list->count)
    {
        list->count--;
        node=list->head->next;
        node->next->pre=node->pre;
        node->pre->next=node->next;
        free(node->data);
        free(node);
    }
}

void* ListFront(List list)
{
    return ListAt(list,0);
}

void* ListBack(List list)
{
    return ListAt(list,list->count-1);
}

void* ListAt(List list, uint32 index)
{
    int i;
    Listnode *node;
    if(list&&list->count>index)
    {
        if(index<list->count/2)
        {
            node=list->head->next;
            for(i=0;i<index;i++)
                node=node->next;
        }
        else
        {
            node=list->tail->pre;
            index=list->count-index-1;
            for(i=0;i<index;i++)
                node=node->pre;
        }
        return node->data;
    }
    return nullptr;
}

void ListFlush(List list)
{
    Listnode *node,*next;
    if(list&&list->count)
    {
        list->count=0;
        node=list->head->next;
        while(node!=list->tail)
        {
            next=node->next;
            node->pre->next=node->next;
            node->next->pre=node->pre;
            free(node->data);
            free(node);
            node=next;
        }
    }
}

void ListDestroy(List *list)
{
    if(list&&*list)
    {
        ListFlush(*list);
        free((*list)->head);
        free((*list)->tail);
        free(*list);
        *list=nullptr;
    }
}
