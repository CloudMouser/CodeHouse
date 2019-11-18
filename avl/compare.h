#ifndef __COMPARE_H
#define __COMPARE_H
typedef enum
{
    EQUAL,
    BIGGER,
    SMALLER,
}CMP;
typedef CMP (*GenericCompare)(void *a,void *b);
#endif // __COMPARE_H
