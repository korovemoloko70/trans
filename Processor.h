#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "MyStack.h"


enum ProcessorStatus
{
    PROC_SUCCESS = 0,
    NULL_PROC_POINTER,    //процессор по нулевому указателю
    ASM_MEMORY_ALLOCATION_ERROR, //ошибка выделени€ пам€ти
    ERROR_IP,
    INVALID_ASM_POINTER //неверный указатель на массив Asm
};

//объ€вление внешнего массива
extern StackElem* op_memory;

struct Processor
{
    MyStack stack;  //стек, на котором производ€тс€ вычислени€
    StackElem* Asm; //указатель на массив с кодом ассемблера
    int ip;  //счетчик выполненных команд из массива(указатель на команды)
    StackElem R[16];  //регистры
    MyStack retAdr;  //стек адресов возврата дл€ CALL
    size_t asm_size;  //размер массива Asm
    size_t asm_capacity;  //емкость массива Asm
};

int ProcessorInit(Processor* cpu);
void ProcessorDestroy(Processor* cpu);
void ProcessorDump(Processor* cpu);


#endif
