#include "avl.h"
#include "list.h"
#include <stdio.h>
Treenode* TreenodeCreate(void *key,uint32 keysize,void *value,uint32 valuesize)
{
    Treenode *node;
    node=malloc(sizeof(struct Treenode));
    if(node)
    {
        node->balance=0;
        node->left=nullptr;
        node->right=nullptr;
        node->parent=nullptr;
        node->key=nullptr;
        node->value=nullptr;
        if(key&&keysize&&value&&valuesize)
        {
            node->key=malloc(keysize);
            node->value=malloc(valuesize);
            if(node->key&&node->value)
            {
                memcpy(node->key,key,keysize);
                memcpy(node->value,value,valuesize);
            }
            else
            {
                free(node->key);
                free(node->value);
                free(node);
                return nullptr;
            }
        }
    }
    return node;
}

Tree TreeCreate(GenericCompare compare)
{
    Tree tree;
    tree=malloc(sizeof(struct Tree));
    if(tree);
    {
        tree->root=TreenodeCreate(nullptr,0,nullptr,0);
        if(tree->root)
        {
            tree->count=0;
            tree->compare=compare;
        }
        else
        {
            free(tree);
            return nullptr;
        }
    }
    return tree;
}

Treenode* TreeInsertSimply(Tree tree,void *key,uint32 keysize,void *value,uint32 valuesize)
{
    int pos=0;
    CMP cmpresult;
    Treenode *tmp,*node,*parent;
    if(tree->count==0)
    {
        tmp=tree->root;
        node=TreenodeCreate(key,keysize,value,valuesize);
        if(node)
        {
            tree->count++;
            tmp->left=node;
            node->parent=tmp;
            return node;
        }
    }
    else
    {
        tmp=tree->root->left;
        while(tmp)
        {
            parent=tmp;
            cmpresult=tree->compare(key,tmp->key);
            if(cmpresult==SMALLER)
            {
                pos=1;
                tmp=tmp->left;
            }
            else if(cmpresult==BIGGER)
            {
                pos=2;
                tmp=tmp->right;
            }
            else
                return nullptr;
        }
        node=TreenodeCreate(key,keysize,value,valuesize);
        if(node)
        {
            tree->count++;
            node->parent=parent;
            if(pos==1)
                parent->left=node;
            else
                parent->right=node;
            return node;
        }
    }
    return nullptr;
}

Treenode* TreeBalanceDeal(Tree tree,Treenode *node)
{
    while(node->parent!=tree->root)
    {
        if(node->parent->left==node)
            node->parent->balance--;
        else
            node->parent->balance++;
        if(node->parent->balance==0)
            return nullptr;
        if(abs(node->parent->balance)>1)
            return node->parent;
        node=node->parent;
    }
    return nullptr;
}

void TreeLeftRotate(Treenode *node)
{
    Treenode *child,*parent;
    child=node->right;
    parent=node->parent;
    if(parent->left==node)
        parent->left=child;
    else
        parent->right=child;
    child->parent=parent;
    node->right=child->left;
    if(child->left)
        child->left->parent=node;
    child->left=node;
    node->parent=child;
    node->balance=0;
    child->balance=0;
}

void TreeRightRotate(Treenode *node)
{
    Treenode *child,*parent;
    child=node->left;
    parent=node->parent;
    if(parent->left==node)
        parent->left=child;
    else
        parent->right=child;
    child->parent=parent;
    node->left=child->right;
    if(child->right)
        child->right->parent=node;
    child->right=node;
    node->parent=child;
    node->balance=0;
    child->balance=0;
}

void TreeLeftRightRotate(Treenode *node)
{
    char balance;
    Treenode *child,*grand;
    child=node->left;
    grand=child->right;
    balance=grand->balance;
    TreeLeftRotate(child);
    TreeRightRotate(node);
    if(balance==-1)
    {
        node->balance=1;
        child->balance=0;
    }
    else if(balance==0)
    {
        node->balance=0;
        child->balance=0;
    }
    else
    {
        node->balance=0;
        child->balance=-1;
    }
    grand->balance=0;
}

void TreeRightLeftRotate(Treenode *node)
{
    char balance;
    Treenode *child,*grand;
    child=node->right;
    grand=child->left;
    balance=grand->balance;
    TreeRightRotate(child);
    TreeLeftRotate(node);
    if(balance==-1)
    {
        node->balance=0;
        child->balance=1;
    }
    else if(balance==0)
    {
        node->balance=0;
        child->balance=0;
    }
    else
    {
        node->balance=-1;
        child->balance=0;
    }
    grand->balance=0;
}

void TreeLeftRightDeleteRotate(Treenode *node)
{
    Treenode *child,*grand;
    child=node->left;
    grand=child->right;
    TreeLeftRotate(child);
    TreeRightRotate(node);
    node->balance=0;
    child->balance=-1;
    grand->balance=-1;
}

void TreeRightLeftDeleteRotate(Treenode *node)
{
    Treenode *child,*grand;
    child=node->right;
    grand=child->left;
    TreeRightRotate(child);
    TreeLeftRotate(node);
    node->balance=0;
    child->balance=1;
    grand->balance=1;
}

RType TreeRotateType(Treenode *node)
{
    if(node->balance<0)
    {
        if(node->left->balance<0)
            return RR;
        else if(node->left->balance>0)
            return LR;
        else
            return LRD;
    }
    else
    {
        if(node->right->balance>0)
            return LL;
        else if(node->right->balance<0)
            return RL;
        else
            return RLD;
    }
}

RType TreeRotate(Treenode *node,RType rtype)
{
    switch(rtype)
    {
    case LL:
        TreeLeftRotate(node);
        break;
    case RL:
        TreeRightLeftRotate(node);
        break;
    case RR:
        TreeRightRotate(node);
        break;
    case LR:
        TreeLeftRightRotate(node);
        break;
    case RLD:
        TreeRightLeftDeleteRotate(node);
        break;
    case LRD:
        TreeLeftRightDeleteRotate(node);
        break;
    default:
        break;
    }
    return rtype;
}

Treenode* TreeInsert(Tree tree, void* key, uint32 keysize, void* value, uint32 valuesize)
{
    RType rtype;
    Treenode *node,*rotate;
    if(tree&&tree->compare&&key&&value&&keysize&&valuesize)
    {
        node=TreeInsertSimply(tree,key,keysize,value,valuesize);
        if(node)
        {
            rotate=TreeBalanceDeal(tree,node);
            if(rotate)
            {
                rtype=TreeRotateType(rotate);
                TreeRotate(rotate,rtype);
            }
        }
        return node;
    }
    return nullptr;
}

Treenode* TreeFind(Tree tree,void *key)
{
    CMP cmp;
    Treenode *node;
    if(tree&&tree->count)
    {
        node=tree->root->left;
        while(node)
        {
            cmp=tree->compare(key,node->key);
            if(cmp==BIGGER)
                node=node->right;
            else if(cmp==SMALLER)
                node=node->left;
            else
                return node;
        }
    }
    return nullptr;
}

Treenode* TreenodeToBeDeleted(Tree tree,void *key)
{
    void *tmpkey,*tmpvalue;
    Treenode *tmp,*node,*parent;
    node=TreeFind(tree,key);
    if(node&&node->left&&node->right)
    {
        tmp=node;
        node=node->left;
        while(node)
        {
            parent=node;
            node=node->right;
        }
        node=parent;
        tmpkey=tmp->key;
        tmpvalue=tmp->value;
        tmp->key=node->key;
        tmp->value=node->value;
        node->key=tmpkey;
        node->value=tmpvalue;
    }
    return node;
}

void TreeDeleteDeal(Tree tree,Treenode *node)
{
    Treenode *parent=node->parent;
    if(parent->left==node)
    {
        if(parent!=tree->root)
            parent->balance++;
        if(node->left)
        {
            parent->left=node->left;
            node->left->parent=parent;
        }
        else if(node->right)
        {
            parent->left=node->right;
            node->right->parent=parent;
        }
        else
            parent->left=nullptr;
    }
    else
    {
        if(parent!=tree->root)
            parent->balance--;
        if(node->left)
        {
            parent->right=node->left;
            node->left->parent=parent;
        }
        else if(node->right)
        {
            parent->right=node->right;
            node->right->parent=parent;
        }
        else
            parent->right=nullptr;
    }
}

void TreeDelete(Tree tree,void *key)
{
    RType rtype;
    char balance;
    Treenode *del,*parent,*grandparent;
    if(tree&&tree->count&&key&&tree->compare)
    {
        del=TreeFind(tree,key);
        if(del)
        {
            parent=del->parent;
            balance=parent->balance;
            TreeDeleteDeal(tree,del);
            free(del->key);
            free(del->value);
            free(del);
            tree->count--;
            while(parent!=tree->root)
            {
                grandparent=parent->parent;
                if(balance==0)
                    return;
                balance=grandparent->balance;
                if(abs(parent->balance)>1)
                {
                    rtype=TreeRotateType(parent);
                    if(rtype!=RLD&&rtype!=LRD)
                    {
                        if(grandparent!=tree->root)
                        {
                            if(grandparent->left==parent)
                                grandparent->balance++;
                            else
                                grandparent->balance--;
                        }
                    }
                    else
                    {
                        TreeRotate(parent,rtype);
                        return;
                    }
                    TreeRotate(parent,rtype);
                }
                else if(parent->balance==0)
                {
                    if(grandparent!=tree->root)
                    {
                        if(grandparent->left==parent)
                            grandparent->balance++;
                        else
                            grandparent->balance--;
                    }
                }
                parent=grandparent;
            }
        }
    }
}

void TreeFlush(Tree tree)
{
    List list;
    Treenode *node;
    if(tree&&tree->count)
    {
        tree->count=0;
        list=ListCreate();
        ListPushBack(list,&tree->root->left,sizeof(Treenode*));
        while(list->count)
        {
            node=*(Treenode**)ListFront(list);
            if(node->left)
                ListPushBack(list,&node->left,sizeof(Treenode*));
            if(node->right)
                ListPushBack(list,&node->right,sizeof(Treenode*));
            ListPopFront(list);
            free(node->key);
            free(node->value);
            free(node);
        }
        ListDestroy(&list);
    }
}

void TreeDestroy(Tree *tree)
{
    #ifdef DEBUG
    uint32 count;
    #endif // DEBUG
    if(tree&&*tree)
    {
        #ifdef DEBUG
        count=(*tree)->count;
        #endif // DEBUG
        TreeFlush(*tree);
        free((*tree)->root);
        *tree=nullptr;
        #ifdef DEBUG
        printf("%d nodes were deleted!",count);
        #endif // DEBUG
    }
}
