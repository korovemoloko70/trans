#ifndef MYSTACK_H
#define MYSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef double StackElem;

#define E 0.0001

enum StackStatus
{
    STACK_SUCCESS = 0,
    STACK_ERROR,
    MEMORY_ALLOCATION_ERROR,
    NULL_STACK_POINTER,
    LEFT_ATTACK,
    RIGHT_ATTACK,
    STACK_EMPTY,
    STACK_OVERFLOW
};

struct MyStack
{
    StackElem* data;
    size_t capacity;         // вместимость стека на данный момент
    size_t read_size;        // текущий размер стека (клеток из капасити заполнено)
    size_t element_size;
};

int MyStackInit(MyStack* st, size_t initial_capacity);
int MyStackRealloc(MyStack* st, size_t new_capacity);
int MyPush(MyStack* st, StackElem const value);
int MyPop(MyStack* st, StackElem* value);
int MyStackTop(MyStack* st, StackElem* value);
void MyStackDestroy(MyStack* st);
void MyStackDump(MyStack* st);
int MyStackVeryFun(MyStack* st);
int CompareWithZero(const StackElem t);

#endif
