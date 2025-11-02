#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Spu.h"
#include "Processor.h"

int main()
{
    Processor cpu;

    if (SpuCalculator(&cpu, "KALKULATOR_ASM.txt") != 0)
    {
        printf("\nError during program execution\n");
        ProcessorDestroy(&cpu);
        return -1;
    }

    ProcessorDestroy(&cpu);

    printf("\nProgram executed successfully\n");
    return 0;
}
