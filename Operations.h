#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "MyStack.h"
#include <stdio.h>

int MyPopOperation(MyStack* st, StackElem* result);
int MyMUL(MyStack* st, StackElem* result);
int MySUB(MyStack* st, StackElem* result);
int MyDIV(MyStack* st, StackElem* result);
int MySQRT(MyStack* st, StackElem* result);
int MyOUT(MyStack* st, StackElem* result);
int MyTOP(MyStack* st, StackElem* result);
int MyIN(MyStack* st);

int MyPOPR(MyStack* st, StackElem* registers, int reg_index);
int MyPUSHR(MyStack* st, StackElem* registers, int reg_index);

int MyJB(MyStack* st, int* ip, int jump_address);
int MyJBE(MyStack* st, int* ip, int jump_address);
int MyJA(MyStack* st, int* ip, int jump_address);
int MyJAE(MyStack* st, int* ip, int jump_address);
int MyJE(MyStack* st, int* ip, int jump_address);
int MyJNE(MyStack* st, int* ip, int jump_address);
int MyJNP(int* ip, int jump_address);

//прототипы
int MyCALL(MyStack* retAdr, int* ip, int call_address);
int MyRET(MyStack* retAdr, int* ip);

int MyPUSHM(MyStack* st, StackElem* op_memory, int address);
int MyPOPM(MyStack* st, StackElem* op_memory, int address);

#endif
