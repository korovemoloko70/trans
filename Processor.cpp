#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Processor.h"
#include "MyStack.h"

StackElem* op_memory = NULL;

int ProcessorInit(Processor* cpu)
{
    if (cpu == NULL)
    {
        return NULL_PROC_POINTER;
    }

    if (MyStackInit(&(cpu->stack), 10) != 0)
    {
        return -1;
    }

    cpu->asm_capacity = 0;
    cpu->Asm = NULL;

    for (int i = 0; i < 16; i++)
    {
        cpu->R[i] = 0;
    }

    if (MyStackInit(&(cpu->retAdr), 10) != 0) return -1;

    cpu->ip = 0;
    cpu->asm_size = 0;

    return PROC_SUCCESS;
}

void ProcessorDestroy(Processor* cpu)
{
    if (cpu != NULL)
    {

        if(cpu->Asm != NULL)
        {
            free(cpu->Asm);
            cpu->Asm = NULL;
        }

        for (int i = 0; i < 16; i++)
        {
            cpu->R[i] = 0;
        }

        MyStackDestroy(&cpu->stack);
        MyStackDestroy(&cpu->retAdr);
        cpu->asm_size = 0;
        cpu->asm_capacity = 0;
        cpu->ip = 0;
    }
}

void ProcessorDump(Processor* cpu)
{
    if (cpu == NULL)
    {
        printf("Processor: NULL pointer\n");
        return;
    }

    printf("---- Processor Dump ----\n");
    printf("Instruction counter (ip): %d\n", cpu->ip);
    printf("Asm array size: %lu, capacity: %lu\n", (unsigned long)cpu->asm_size, (unsigned long)cpu->asm_capacity);

    printf("Asm array:\n");
    for (size_t i = 0; i < cpu->asm_size; i++)
    {
        printf("Asm[%d] = %.3lf\n", i, cpu->Asm[i] / 1000.0);
    }

    printf("Registers R:\n");
    for (int i = 0; i < 16; i++)
    {
        printf("R[%d] = %.3lf\n", i, cpu->R[i] / 1000.0);
    }

    printf("Return addresses:\n");
    MyStackDump(&cpu->retAdr);

    MyStackDump(&cpu->stack);
    printf("---- End Dump ----\n");
}


