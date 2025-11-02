#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProcessorVerifay_for_otladka.h"
#include "Processor.h"
#include "MyStack.h"

int ProcessorVerify(Processor* cpu)
{
    if (cpu == NULL)
    {
        return NULL_PROC_POINTER;
    }

    if (cpu->Asm == NULL)
    {
        return INVALID_ASM_POINTER;
    }

    if (cpu->ip < 0 || cpu->ip > (int) cpu->asm_size)
    {
        return ERROR_IP;
    }

    return MyStackVeryFun(&cpu->stack);
}
