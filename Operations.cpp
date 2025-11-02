#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Operations.h"
#include "MyStack.h"

int MyPopOperation(MyStack* st, StackElem* result)
{
    printf("POP: ");
    if (MyPop(st, result) == 0)
    {
        printf("popped value %.3lf\n", *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: cannot pop from stack\n");
        return -1;
    }
}

int MyMUL(MyStack* st, StackElem* result)
{
    printf("MyMUL: ");
    StackElem a, b;

    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        *result = a * b;
        MyPush(st, *result);
        printf("got values %.3lf and %.3lf, sent result %.3lf\n", a / 1000.0, b / 1000.0, *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: not enough elements in stack\n");
        return -1;
    }
}

int MySUB(MyStack* st, StackElem* result)
{
    printf("MySUB: ");
    StackElem a, b;

    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        *result = b - a;
        MyPush(st, *result);
        printf("got values %.3lf and %.3lf, sent result %.3lf\n", a / 1000.0, b / 1000.0, *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: not enough elements in stack\n");
        return -1;
    }
}

int MyDIV(MyStack* st, StackElem* result)
{
    printf("MyDIV: ");
    StackElem a, b;

    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0 && CompareWithZero(a) != 0)
    {
        *result = b / a;
        MyPush(st, *result);
        printf("got values %.3lf and %.3lf, sent result %.3lf\n", a / 1000.0, b / 1000.0, *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: division error (not enough elements or division by zero)\n");
        return -1;
    }
}

int MySQRT(MyStack* st, StackElem* result)
{
    printf("MySQRT: ");
    StackElem a;

    if (MyPop(st, &a) == 0 && a >= 0)
    {
        *result = (StackElem)sqrt(a);
        MyPush(st, *result);
        printf("got value %.3lf, sent result %.3lf\n", a / 1000.0, *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: cannot calculate square root (not enough elements or negative value)\n");
        return -1;
    }
}

int MyOUT(MyStack* st, StackElem* result)
{
    printf("OUT: ");
    if (MyStackTop(st, result) == 0)
    {
        printf("output value %.3lf\n", *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: cannot get top element for OUT\n");
        return -1;
    }
}

int MyTOP(MyStack* st, StackElem* result)
{
    printf("MyTOP: ");
    if (MyStackTop(st, result) == 0)
    {
        printf("got value %.3f\n", *result / 1000.0);
        return 0;
    }
    else
    {
        printf("Error: cannot get top element\n");
        return -1;
    }
}

int MyIN(MyStack* st)
{
    printf("IN: ");
    StackElem value;
    printf("Enter number: ");
    if (scanf("%lf", &value) == 1)
    {
        MyPush(st, value * 1000); //умножаем на 1000 при вводе
        printf("read value %.3f\n", value);
        return 0;
    }
    else
    {
        printf("Error: invalid input for IN\n");
        return -1;
    }
}

int MyPOPR(MyStack* st, StackElem* registers, int reg_index)
{
    printf("POPR: ");
    StackElem value;
    if (MyPop(st, &value) == 0 && reg_index >= 0 && reg_index < 16)
    {
        registers[reg_index] = value;
        printf("stored value %.3f in register R%d\n", value / 1000.0, reg_index);
        return 0;
    }
    else
    {
        printf("Error: invalid POPR operation\n");
        return -1;
    }
}

int MyPUSHR(MyStack* st, StackElem* registers, int reg_index)
{
    printf("PUSHR: ");
    if (reg_index >= 0 && reg_index < 16)
    {
        MyPush(st, registers[reg_index]);
        printf("pushed value %.3f from register R%d\n", registers[reg_index] / 1000.0, reg_index);
        return 0;
    }
    else
    {
        printf("Error: invalid register index for PUSHR\n");
        return -1;
    }
}

int MyJB(MyStack* st, int* ip, int jump_address)
{
    printf("JB: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (CompareWithZero(a - b) == 1) // a>b
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f > %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f <= %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJBE(MyStack* st, int* ip, int jump_address)
{
    printf("JBE: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (a >= b) //a >= b
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f >= %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f < %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJA(MyStack* st, int* ip, int jump_address)
{
    printf("JA: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (a < b)
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f < %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f >= %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJAE(MyStack* st, int* ip, int jump_address)
{
    printf("JAE: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (a <= b)
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f <= %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f > %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJE(MyStack* st, int* ip, int jump_address)
{
    printf("JE: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (CompareWithZero(a - b) == 0) //a == b
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f == %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f != %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJNE(MyStack* st, int* ip, int jump_address)
{
    printf("JNE: ");
    StackElem a, b;
    if (MyPop(st, &a) == 0 && MyPop(st, &b) == 0)
    {
        if (CompareWithZero(a - b) != 0) //a != b
        {
            *ip = jump_address;
            printf("jump to address %d (condition: %.3f != %.3f)\n", jump_address, a / 1000.0, b / 1000.0);
        }
        else
        {
            printf("no jump (condition: %.3f == %.3f)\n", a / 1000.0, b / 1000.0);
        }
        MyPush(st, b);
        MyPush(st, a);
        return 0;
    }
    else
    {
        printf("Error: not enough elements for comparison\n");
        return -1;
    }
}

int MyJNP(int* ip, int jump_address)
{
    printf("JNP: unconditional jump to address %d\n", jump_address);
    *ip = jump_address;
    return 0;
}

int MyCALL(MyStack* retAdr, int* ip, int call_address)
{
     if (retAdr == NULL || ip == NULL)
     {
            return -1; // код ошибки
     }

    int return_address = *ip + 2;
    if (MyPush((MyStack*) retAdr, (StackElem) return_address) == STACK_SUCCESS)
    {
        *ip = call_address;
        return 0;
    }
    return -1;
}

int MyRET(MyStack* retAdr, int* ip)
{
     if (retAdr == NULL || ip == NULL)
     {
            return -1; // код ошибки
     }

     StackElem return_adress = 0;
     if (MyPop((MyStack*) retAdr, &return_adress) == 0)
     {
         *ip = (int)return_adress;
         return 0;
     }
     return -1;
}

//заглушки для нереализованных функций
/*int MyPUSHM(MyStack* st, StackElem* op_memory, int address)
{

    return 0;
}

int MyPOPM(MyStack* st, StackElem* op_memory, int address)
{
    return 0;
}      */
