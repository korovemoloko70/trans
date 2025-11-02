#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyStack.h"

//подобраны так, чтобы занимать все 4 бита(не было нулей по краям)
#define canary1 (StackElem)17878103347812890625ULL
#define canary2 (StackElem)17878103347812890600ULL

int MyStackInit(MyStack* st, size_t initial_capacity)// Инициализация стека
{
    assert(st != NULL);

    if (initial_capacity == 0) initial_capacity = 10;  // емкость по умолчанию
    initial_capacity += 2;

    st->data = (StackElem*)calloc(initial_capacity, sizeof(StackElem));
    if (st->data == NULL) return MEMORY_ALLOCATION_ERROR;      // не хватило памяти

    st->data[0] = canary1;
    st->data[initial_capacity - 1] = canary2;
    st->capacity = initial_capacity;
    st->read_size = 1;
    st->element_size = sizeof(StackElem);

    if (MyStackVeryFun(st)) return ERROR;
    return STACK_SUCCESS;
}

int MyStackRealloc(MyStack* st, size_t new_capacity)// Увеличение емкости data стека
{
    if (MyStackVeryFun(st))  return ERROR;

    st->data[st->capacity - 1] = 0;
    StackElem* new_data = (StackElem*)realloc(st->data, new_capacity*sizeof(StackElem));
    if (new_data == NULL) return MEMORY_ALLOCATION_ERROR;// не хватило памяти


    st->data = new_data;
    st->capacity = new_capacity;
    st->data[new_capacity - 1] = canary2;

    if (MyStackVeryFun(st)) return ERROR;
    return STACK_SUCCESS;
}


int MyPush(MyStack* st, StackElem const value)// Добавление элемента в стек
{
    if (MyStackVeryFun(st)) return ERROR;

    if (st->read_size >= st->capacity-1)// увеличиваем емкость, если класть новое некуда
    {

        size_t new_capacity = (st->capacity) * 2;
        if (MyStackRealloc(st, new_capacity) != 0) return MEMORY_ALLOCATION_ERROR; // не удалось выделить память
    }

    st->data[st->read_size] = value;
    st->read_size++;
    if (MyStackVeryFun(st))  return ERROR;
    return STACK_SUCCESS;
}

int MyPop(MyStack* st, StackElem* value)// Извлечение элемента из стека
{
    if (MyStackVeryFun(st) || st->read_size == 1) return ERROR;
    assert(value != NULL);

    st->read_size--;
    *value = st->data[st->read_size];

    if (st->read_size < (st->capacity-1)/2)//уменьшаем емкость, чтобы не засорять память
    {

        size_t new_capacity = st->read_size + 1;//+1 а всяуий
        if (MyStackRealloc(st, new_capacity) != 0) return MEMORY_ALLOCATION_ERROR; // не удалось выделить память
    }

    if (MyStackVeryFun(st)) return ERROR;
    return STACK_SUCCESS;
}

int MyStackTop(MyStack* st, StackElem* value)//просмотр верхнего элемента
{
    if (MyStackVeryFun(st))  return ERROR;
    assert(value != NULL);

    *value = st->data[st->read_size-1];
    if (MyStackVeryFun(st))  return ERROR;
    return STACK_SUCCESS;
}

void MyStackDestroy(MyStack* st)// Уничтожение стека
{
    assert(st != NULL);

    st->capacity = 0;         // вместимость стека на данный момент
    st->read_size = 0;        // текущий размер стека (клеток из капасити заполнено)
    st->element_size = 0;
    if (st->data != NULL) free(st->data);
    st->data = NULL;
}


void MyStackDump(MyStack* st)// Печать всех полей стека
{
    assert(st != NULL);
    printf("Стек (считано элементов: %lu, емкость: %lu, размер элемента(байт): %lu):\n",
     (unsigned long)st->read_size-1, (unsigned long)st->capacity, (unsigned long)st->element_size);

    for (size_t i = 1; i < st->read_size; i++)
    {
        printf("st[%d] = %lf \n", (int) i, st->data[i]);
    }

    printf("\n");
}


int MyStackVeryFun(MyStack* st)
{
    if (st == NULL)
    {
        printf("NULL_STACK_POINTER\n");
        return NULL_STACK_POINTER;
    }

    if (CompareWithZero(st->data[0] - canary1)  != 0)
    {
        printf("LEFT_ATTACK\n");
        return LEFT_ATTACK;
    }

    if(CompareWithZero(st->data[st->capacity - 1] - canary2) != 0)
    {
        printf("RIGHT_ATTACK\n");
        return RIGHT_ATTACK;
    }

    if (st->read_size == 0)
    {
        printf("STACK_EMPTY\n");
        return STACK_EMPTY;
    }

    if (st->read_size >= st->capacity)
    {
        printf("STACK_OVERFLOW\n");
        return STACK_OVERFLOW;
    }

    return STACK_SUCCESS;
}

int CompareWithZero(const StackElem t)
{
   if (E >= t && t >= -E) return 0;
   if (E < t) return 1;
   return -1;
}
