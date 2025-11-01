#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Translate.h"

int InputProgram(const char* in_file, const char* out_file);

int main()
{

    //DEBUG START
    const char* in_file = "C:/mipt/programing/stack/tranS/KALKULATOR_TEXT.txt";
    const char* out_file = "C:/mipt/programing/stack/tranS/KALKULATOR_ASM.txt";
    printf("=== DEBUG START ===\n");
    printf("Input file: %s\n", in_file);
    printf("Output file: %s\n", out_file);

    // Проверка существования входного файла
    FILE* test_in = fopen(in_file, "r");
    if (test_in == NULL) {
        printf("ERROR: Input file not found or cannot be read\n");
        return -1;
    }
    fclose(test_in);

    if (InputProgram(in_file, out_file) != SUCCESS)
    {
        printf("Error during translation\n");
        return -1;
    }
printf("Nice translation\n");
    // Проверка создался ли выходной файл
    FILE* test_out = fopen(out_file, "r");
    if (test_out == NULL) {
        printf("ERROR: Output file was not created\n");
        return -1;
    }
    printf("l34\n");
    fclose(test_out);

    printf("=== DEBUG END ===\n");
    return 0;
    //DEBUG END

/*
    const char* in_file = "KALKULATOR_TEXT.txt";
    const char* out_file = "KALKULATOR_ASM.txt";
    if (InputProgram(in_file, out_file) != 0)
    {
        printf("Error during translation\n");
        return -1;
    }
    printf("Nice translation\n");

    return 0;*/
}
