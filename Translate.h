#ifndef TRANSLATE_H
#define TRANSLATE_H

enum OpCodes
{
    OP_PUSH = 1,
    OP_POP = 2,
    OP_MUL = 3,
    OP_SUB = 4,
    OP_DIV = 5,
    OP_SQRT = 6,
    OP_OUT = 7,
    OP_TOP = 8,
    OP_IN = 9,
    OP_POPR = 10,
    OP_PUSHR = 11,
    OP_JB = 12,
    OP_JBE = 13,
    OP_JA = 14,
    OP_JAE = 15,
    OP_JE = 16,
    OP_JNE = 17,
    OP_JNP = 18,
    OP_CALL = 19,
    OP_PUSHM = 20,
    OP_POPM = 21,
    OP_HLT = 0
};

enum Registers
{
    REG_AX = 0,
    REG_BX = 1,
    REG_CX = 2,
    REG_DX = 3,
    REG_EX = 4,
    REG_FX = 5,
    REG_GX = 6,
    REG_HX = 7,
    REG_IX = 8,
    REG_JX = 9,
    REG_KX = 10,
    REG_LX = 11,
    REG_MX = 12,
    REG_NX = 13,
    REG_OX = 14,
    REG_PX = 15,
    REG_QX = 16
};

enum TrsltStatus
{
    SUCCESS = 0,
    NULL_POINTER,
    READ_FILE_ERR,
    ERR_SIZE,
    ERR_COMANDS,
    FILE_ERROR,
    MEMORY_ALLOCATION_ERROR,
    NO_ARGUMENT,
    REG_ERR,
    SINTACSIS_ERR,
    TRNSL_ERR
};



long int OpenAndGetFileSize(FILE** file, const char* filename);
int ReadFileIntoBuffer(FILE* file, long int file_size, char* buffer);

int NumberOfComandsAndLablesFromBuffer(char* buffer, const char* in_file, int* command_number);
int TranslateFromBuffer(char* buffer, const char* in_file, FILE* out);

int ExtendLabels(const int label_index);
int IfLabel(const char* line_ptr, const int line_num, const char* in_file, int* command_number);

int ProcessPush(const char* after_command, const int line_num, const char* in_file, FILE* out);
int ProcessPopr(const char* after_command, const int line_num, const char* in_file, FILE* out);
int ProcessPushr(const char* after_command, const int line_num, const char* in_file, FILE* out);
int ProcessJumpCommand(const char* after_command, const int line_num, const char* in_file, FILE* out, int op_code);

int ProcessPushm(const char* after_command, const int line_num, const char* in_file, FILE* out);
int ProcessPopm(const char* after_command, const int line_num, const char* in_file, FILE* out);

int GetRegisterCode(const char* reg, const int line_num, const char* in_file);

#endif
