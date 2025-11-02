#ifndef SPU_H
#define SPU_H
#include "Processor.h"

int SpuCalculator(Processor* cpu, const char* input_filename);
void PrintResult(const StackElem value);

#endif
