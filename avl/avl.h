#ifndef __AVL_H
#define __AVL_H
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "define.h"
#include "compare.h"
#define DEBUG
typedef enum
{
    RR,
    LR,
    LL,
    RL,
    LRD,
    RLD,
    NO,
}RType;

typedef struct Treenode
{
    void *key;
    void *value;
    char balance;
    struct Treenode *left;
    struct Treenode *right;
    struct Treenode *parent;
}Treenode;

typedef struct Tree
{
    uint32 count;
    Treenode *root;
    GenericCompare compare;
}*Tree;

Tree TreeCreate(GenericCompare compare);
Treenode* TreeInsert(Tree tree, void *key, uint32 keysize, void *value, uint32 valuesize);
void TreeDelete(Tree tree,void *key);
void TreeFlush(Tree tree);
void TreeDestroy(Tree *tree);
#endif // __AVL_H
