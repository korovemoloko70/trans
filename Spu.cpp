#include <TXlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Spu.h"
#include "ProcessorVerifay_for_otladka.h"
#include "Operations.h"
#include "Processor.h"

void PrintResult(const StackElem value)
{
    printf("Result: %.3lf\n", value/1000.0);
}

int SpuCalculator(Processor* cpu, const char* input_filename)//основная
{
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        printf("Error: Cannot open input file %s\n", input_filename);
        return -1;
    }

    //читаем количество строк из первой строки файла
    int line_count;
    if (fscanf(input_file, "%d", &line_count) != 1)
    {
        printf("Error: Cannot read line count from file\n");
        fclose(input_file);
        return -1;
    }

    cpu->asm_capacity = line_count * 2;
    cpu->Asm = (StackElem*)calloc(cpu->asm_capacity, sizeof(StackElem));
    if (cpu->Asm == NULL)
    {
        printf("Error: Memory allocation failed for Asm array\n");
        fclose(input_file);
        return -1;
    }

    cpu->asm_size = 0;
    cpu->ip = 0;

    //читаем команды из файла
    int command;
    while (fscanf(input_file, "%d", &command) == 1 && cpu->asm_size < cpu->asm_capacity)
    {
        cpu->Asm[cpu->asm_size++] = command;
    }

    fclose(input_file);

    //верификация процессора перед выполнением
    if (ProcessorVerify(cpu) != 0)
    {
        printf("Error: Processor verification failed\n");
        return -1;
    }

    //выполнение команд
    while (cpu->ip < (int)cpu->asm_size)
    {
        int op_code = (int)cpu->Asm[cpu->ip];
        StackElem result = 0;
        int operation_result = 0;

        switch (op_code)
        {
            case 1: //PUSH с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    StackElem value = cpu->Asm[cpu->ip + 1] * 1000; //умножаем на 1000
                    operation_result = MyPush(&cpu->stack, value);
                    cpu->ip += 2;
                    printf("PUSH: pushed value %.3f\n", value / 1000.0);
                }
                else
                {
                    printf("Error: incomplete PUSH instruction\n");
                    return -1;
                }
                break;

            case 2: //POP
                operation_result = MyPopOperation(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 3: //MUL
                operation_result = MyMUL(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 4: //SUB
                operation_result = MySUB(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 5: //DIV
                operation_result = MyDIV(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 6: //SQRT
                operation_result = MySQRT(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 7: //OUT
                operation_result = MyOUT(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 8: //TOP
                operation_result = MyTOP(&cpu->stack, &result);
                cpu->ip++;
                break;

            case 9: //IN
                operation_result = MyIN(&cpu->stack);
                cpu->ip++;
                break;

            case 10: //POPR с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int reg_index = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyPOPR(&cpu->stack, cpu->R, reg_index);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete POPR instruction\n");
                    return -1;
                }
                break;

            case 11: //PUSHR с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int reg_index = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyPUSHR(&cpu->stack, cpu->R, reg_index);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete PUSHR instruction\n");
                    return -1;
                }
                break;

            case 12: //JB с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJB(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JB instruction\n");
                    return -1;
                }
                break;

            case 13: //JBE с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJBE(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JBE instruction\n");
                    return -1;
                }
                break;

            case 14: //JA с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJA(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JA instruction\n");
                    return -1;
                }
                break;

            case 15: //JAE с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJAE(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JAE instruction\n");
                    return -1;
                }
                break;

            case 16: //JE с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJE(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JE instruction\n");
                    return -1;
                }
                break;

            case 17: //JNE с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJNE(&cpu->stack, &cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JNE instruction\n");
                    return -1;
                }
                break;

            case 18: //JNP с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int jump_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyJNP(&cpu->ip, jump_address);
                    cpu->ip += 2;
                }
                else
                {
                    printf("Error: incomplete JNP instruction\n");
                    return -1;
                }
                break;

            case 19: //CALL с аргументом
                if (cpu->ip + 1 < (int)cpu->asm_size)
                {
                    int call_address = (int)cpu->Asm[cpu->ip + 1];
                    operation_result = MyCALL(&cpu->retAdr, &cpu->ip, call_address);
                }
                else
                {
                    printf("Error: incomplete CALL instruction\n");
                    return -1;
                }
                break;

            case 20: //RET for end of CALL
                operation_result = MyRET(&cpu->retAdr, &cpu->ip);//адрес указателя передаем
                break;

            case 0: //HLT
                printf("HLT: execution completed successfully\n");
                return 0;

            default:
                printf("Error: unknown operation code %d at position %d\n", op_code, cpu->ip);
                return -1;
        }

        //результат если возвращают
        if (operation_result == 0 && CompareWithZero(result) != 0)
        {
            PrintResult(result);
        }
    }

    return 0;
}
