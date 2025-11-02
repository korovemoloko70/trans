#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "translate.h"

int* labels = NULL;
int labels_size = 0;

//получение кода регистра по его имени
int GetRegisterCode(const char* reg, const int line_num, const char* in_file)
{

    if (strcmp(reg, "AX") == 0) return REG_AX;
    else if (strcmp(reg, "BX") == 0) return REG_BX;
    else if (strcmp(reg, "CX") == 0) return REG_CX;
    else if (strcmp(reg, "DX") == 0) return REG_DX;
    else if (strcmp(reg, "EX") == 0) return REG_EX;
    else if (strcmp(reg, "FX") == 0) return REG_FX;
    else if (strcmp(reg, "GX") == 0) return REG_GX;
    else if (strcmp(reg, "HX") == 0) return REG_HX;
    else if (strcmp(reg, "IX") == 0) return REG_IX;
    else if (strcmp(reg, "JX") == 0) return REG_JX;
    else if (strcmp(reg, "KX") == 0) return REG_KX;
    else if (strcmp(reg, "LX") == 0) return REG_LX;
    else if (strcmp(reg, "MX") == 0) return REG_MX;
    else if (strcmp(reg, "NX") == 0) return REG_NX;
    else if (strcmp(reg, "OX") == 0) return REG_OX;
    else if (strcmp(reg, "PX") == 0) return REG_PX;
    else if (strcmp(reg, "QX") == 0) return REG_QX;
    else
    {
        printf("\nIM in GetRegisterCod\n\n");
        printf("File: %s, Line: %d - unknown register: '%s'\n", in_file, line_num, reg);
        return REG_ERR;
    }
}

int ProcessPush(const char* after_command, const int line_num, const char* in_file, FILE* out)
{
    int argument = 0;
    if (*after_command == '\0' || sscanf(after_command, "%d", &argument) != 1)
    {
        printf("File: %s, Line: %d - syntax error: 'PUSH'\n", in_file, line_num);
        return SINTACSIS_ERR;
    }
    fprintf(out, "%d %d\n", OP_PUSH, argument);
    return SUCCESS;
}

int ProcessPopr(const char* after_command, const int line_num, const char* in_file, FILE* out)
{
    char* reg = (char*) calloc(50, sizeof(char));
    if (reg == NULL)
    {
        printf("Error: Memory allocation failed for register popr\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    if (*after_command == '\0' || sscanf(after_command, "%49s", reg) != 1)
    {
        printf("File: %s, Line: %d - syntax error: 'POPR'\n", in_file, line_num);
        return SINTACSIS_ERR;
    }

    int reg_code = GetRegisterCode(reg, line_num, in_file);
    if (reg_code == -1) return REG_ERR;

    fprintf(out, "%d %d\n", OP_POPR, reg_code);
    return SUCCESS;
}

int ProcessPushr(const char* after_command, const int line_num, const char* in_file, FILE* out)
{
    char* reg = (char*) calloc(50, sizeof(char));
    if (reg == NULL)
    {
        printf("Error: Memory allocation failed for regisror pushr\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    if (*after_command == '\0' || sscanf(after_command, "%49s", reg) != 1)
    {
        printf("File: %s, Line: %d - syntax error: 'PUSHR'\n", in_file, line_num);
        return SINTACSIS_ERR;
    }

    int reg_code = GetRegisterCode(reg, line_num, in_file);
    if (reg_code != 0) return REG_ERR;

    fprintf(out, "%d %d\n", OP_PUSHR, reg_code);
    return SUCCESS;
}

//вытаскиваем номер строки для перехода по метке
int ProcessJumpCommand(const char* after_command, const int line_num, const char* in_file, FILE* out, int op_code)
{
    int argument = 0;
    if (*after_command == '\0' || after_command[0] != ':' || sscanf(after_command + 1, "%d", &argument) != 1)
    {
        printf("File: %s, Line: %d - syntax error\n", in_file, line_num);
        return SINTACSIS_ERR;
    }
    if (argument >= labels_size || labels[argument] == 0)
    {
        printf("File: %s, Line: %d - неизвестная метка: '%s'\n", in_file, line_num, after_command);
        return SINTACSIS_ERR;
    //НАДО РАСШИРЕНИЕ?
    }
    argument = labels[argument];
    fprintf(out, "%d %d\n", op_code, argument);
    return SUCCESS;
}

int ProcessPushm(const char* after_command, const int line_num, const char* in_file, FILE* out)
{
    char* word = (char*) calloc(50, sizeof(char));
    if (word == NULL)
    {
        printf("Error: Memory allocation failed for pushm\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    if (*after_command == '\0' || sscanf(after_command, "[%49[^]]]", word) != 1)
    {
        printf("File: %s, Line: %d - syntax error: 'PUSHM'\n", in_file, line_num);
        return SINTACSIS_ERR;
    }

    int reg_code = GetRegisterCode(word, line_num, in_file);
    if (reg_code == -1) return REG_ERR;

    fprintf(out, "%d %d\n", OP_PUSHM, reg_code);
    return SUCCESS;
}

int ProcessPopm(const char* after_command, const int line_num, const char* in_file, FILE* out)
{
    char* word = (char*) calloc(50, sizeof(char));
    if (word == NULL)
    {
        printf("Error: Memory allocation failed for popm\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    if (*after_command == '\0' || sscanf(after_command, "[%49[^]]]", word) != 1)
    {
        printf("File: %s, Line: %d - syntax error: 'POPM'\n", in_file, line_num);
        return SINTACSIS_ERR;
    }

    int reg_code = GetRegisterCode(word, line_num, in_file);
    if (reg_code == -1) return REG_ERR;

    fprintf(out, "%d %d\n", OP_POPM, reg_code);
    return SUCCESS;
}

int InputProgram(const char* in_file, const char* out_file)
{
    if (in_file == NULL || out_file == NULL)
    {
        printf("Error: NULL pointer in InputProgram\n");
        return NULL_POINTER;
    }

    FILE* file = NULL;
    long int file_size = OpenAndGetFileSize(&file, in_file);
    if (file_size < 0)
    {
        return ERR_SIZE;
    }

    char* buffer = (char*)calloc(file_size + 1, sizeof(char));
    if (buffer == NULL)
    {
        fclose(file);
        return MEMORY_ALLOCATION_ERROR;
    }

    if (ReadFileIntoBuffer(file, file_size, buffer) != SUCCESS)
    {
        free(buffer);

         //printf("L159\n");

        fclose(file);
        return READ_FILE_ERR;
    }
    fclose(file);

    int command_number = 0;
    if (NumberOfComandsAndLablesFromBuffer(buffer, in_file, &command_number) != SUCCESS)
    {
        free(buffer);
        return ERR_COMANDS;
    }

     //printf("L172\n");//запись количества команд в выходной файл

    FILE* out = fopen(out_file, "w+");
    printf("\n---FILE WAS OPRENED---\n");

    if (out == NULL)
    {
        printf("Error: Cannot open/create output file %s\n", out_file);
        free(buffer);
        return FILE_ERROR;
    }
    fprintf(out, "%d\n", command_number + 1);

    if (TranslateFromBuffer(buffer, in_file, out) != SUCCESS)
    {
        free(buffer);
        return TRNSL_ERR;
    }

     //printf("L222\n");

     free(buffer);
    return SUCCESS;
}

long int OpenAndGetFileSize(FILE** file, const char* filename)
{
    if (file == NULL || filename == NULL)
    {
        printf("Error: NULL pointer in OpenAndGetFileSize\n");
        return NULL_POINTER;
    }

    *file = fopen(filename, "r");
    if (*file == NULL)
    {
        printf("Error: Cannot open input file %s\n", filename);
        return FILE_ERROR;
    }

    if (fseek(*file, 0, SEEK_END)) return FILE_ERROR;
    long fileLength = ftell(*file);
    if (fseek(*file, 0, SEEK_SET)) return FILE_ERROR;

    return fileLength;
}

int ReadFileIntoBuffer(FILE* file, long int file_size, char* buffer)
{
    if (file == NULL || buffer == NULL)
    {
        printf("Error: NULL pointer in ReadFileIntoBuffer\n");
        return NULL_POINTER;
    }

    size_t total_read = fread(buffer, sizeof (char), file_size, file);

    buffer[file_size] = '\0';

    return SUCCESS;

}

int NumberOfComandsAndLablesFromBuffer(char* buffer, const char* in_file, int* command_number)
{
    if (buffer == NULL || in_file == NULL || command_number == NULL)
    {
        printf("Error: NULL pointer in NumberOfComandsAndLablesFromBuffer\n");
        return NULL_POINTER;
    }

    *command_number = 0;
    int line_num = 1;
    char* current_pos = buffer;  //указ где мы в буфере

    while (*current_pos != '\0')
    {

        //printf("L281\n");

        char* line = (char*) calloc(20, sizeof(char));
        if (line == NULL)
        {
            printf("Error: Memory allocation failed for line\n");
            return MEMORY_ALLOCATION_ERROR;
        }
         //копируем строку до комментария или конца строки

        char* comment_pos = strchr(current_pos, ';');
        char* newline_pos = strchr(current_pos, '\n');
        size_t copy_len = 0;

        if (comment_pos != NULL && (newline_pos == NULL || comment_pos < newline_pos))
        {
            copy_len = comment_pos - current_pos;
        }
        else if (newline_pos != NULL)
        {
            copy_len = newline_pos - current_pos;
        }
        else
        {
            copy_len = strlen(current_pos);
        }                           //опр сколько копировать

        if (copy_len > 0 && copy_len < 20)
        {
            strncpy(line, current_pos, copy_len);
            line[copy_len] = '\0';
        }                                  //строка line для работы (без комментариев)



        char* line_ptr = line;
        while (*line_ptr == ' ') line_ptr++;//пропускаем пробелы в начале строки

        //меткa. Заполняем лейблз
        if (*line_ptr == ':')
        {

            if (IfLabel(line_ptr, line_num, in_file, command_number) != SUCCESS)
            {
                printf("L314\n");
                free(line);
                return SINTACSIS_ERR;
            }
        }

        //остались не пустые и не только метка => команда
        else if (*line_ptr != '\0')
        {
            (*command_number)++;

            printf("334\n");

        }
        free(line);

        //переходим к следующей строке

        if (newline_pos != NULL) //те выше нашелся символ перевода строки
        {
            printf("DEBAG: SUCCESS DONE %d LINE FROM TXT\n", line_num);
            current_pos = newline_pos + 1;//след строка
            line_num++;
        }
        else
        {
            break;
        }
    }

    return SUCCESS;
}



int ExtendLabels(const int label_index)
{
    int new_size = label_index + 5;
    int* new_labels = (int*)realloc(labels, new_size * sizeof(int));
    if (new_labels == NULL)
    {
        printf("Error: Memory allocation failed for labels expansion\n");
        return MEMORY_ALLOCATION_ERROR;
    }

    for (int i = labels_size; i < new_size; i++)//новые ячейки нулями, так как реалок при довыделении не заполняет, а при первом аргументе = null работает как малок    {
    {
        new_labels[i] = 0;
    }
    labels = new_labels;
    labels_size = new_size;

    return SUCCESS;
}


int IfLabel(const char* line_ptr, const int line_num, const char* in_file, int* command_number)//заполняем массив переходов
{
    const char* num_ptr = line_ptr + 1;
    while (*num_ptr == ' ') num_ptr++;//пропускаем пробелы

    if (*num_ptr >= '0' && *num_ptr <= '9')
    {
        int label_index = atoi(num_ptr);

        if (label_index >= labels_size) //расширение
        {
            if (ExtendLabels(label_index) != SUCCESS)
            {
                return MEMORY_ALLOCATION_ERROR;
            }
        }

        //printf("L400\n");

        if (label_index < 0 || label_index >= labels_size) {
            printf("ERROR: Label index %d out of bounds [0, %d]\n", label_index, labels_size - 1);
            return SINTACSIS_ERR;
        }

        if (labels == NULL)
        {
            printf("ERROR: labels is NULL!\n");
            return MEMORY_ALLOCATION_ERROR;
        }


        if (labels[label_index] != 0)//проверяем на дублирование
        {

            printf("File: %s, Line: %d - syntax error(dubl): '%s'\n", in_file, line_num, line_ptr);
            return SINTACSIS_ERR;
        }

        //printf("L410\n");

        labels[label_index] = *command_number + 1;//номер след команды

        printf("DEBAG: MARKED %d IN %d\n", labels[label_index], label_index);

    }

    return SUCCESS;
}

int TranslateFromBuffer(char* buffer, const char* in_file, FILE* out) //CommandDetermine
{
    if (buffer == NULL || in_file == NULL || out == NULL)
    {
        printf("Error: NULL pointer in TranslateFromBuffer\n");
        return NULL_POINTER;
    }

    int line_num = 1;  // again
    char* current_pos = buffer;

    //printf("L444\n");

    while (*current_pos != '\0')
    {
        char* line = (char*) calloc(20, sizeof(char));
        if (line == NULL)
        {
            printf("Error: Memory allocation failed for line\n");
            return MEMORY_ALLOCATION_ERROR;
        }
        char* comment_pos = strchr(current_pos, ';');
        char* newline_pos = strchr(current_pos, '\n');
        size_t copy_len = 0;

        if (comment_pos != NULL && (newline_pos == NULL || comment_pos < newline_pos))
        {
            copy_len = comment_pos - current_pos;
        }
        else if (newline_pos != NULL)
        {
            copy_len = newline_pos - current_pos;
        }
        else
        {
            copy_len = strlen(current_pos);
        }

        printf("DEBUG: Copy length: %zu, comment_pos: %p, newline_pos: %p\n",
               copy_len, comment_pos, newline_pos);

        if (copy_len > 0)
        {
            strncpy(line, current_pos, copy_len);
            line[copy_len] = '\0';
        }

        printf("DEBUG: Line after copy: '");
        for (size_t i = 0; i < copy_len; i++)
        {
            if (line[i] == '\0') printf("\\0");
            else printf("%c", line[i]);
        }
        printf("'\n");

        char* line_ptr = line;    //пропуск пробелов
        while (*line_ptr == ' ') line_ptr++;//end of again

        printf("DEBUG: After skipping spaces: line_ptr='");
        if (*line_ptr == '\0') printf("\\0");
        else printf("%c", *line_ptr);
        printf("'\n");

        printf("L497 DEBUG: After skipping spaces: line_ptr+1 = '%c'\n", *(line_ptr+1));

        //скип метки и пустые строки
        if (*line_ptr != ':' && *line_ptr != '\0')
        {
            char* command = (char*) calloc(20, sizeof(char));
            if (command == NULL)
            {
                printf("Error: Memory allocation failed for command in line\n");
                return MEMORY_ALLOCATION_ERROR;
            }

            int scanned = sscanf(line_ptr, "%19s", command);//считываем только команду до пробела или конца строки

            printf("DEBUG len of command: %d \n", strlen(command));

            command[strlen(command)] = '\0';

            if (scanned == 1)//существует
            {

                char* after_command = line_ptr + strlen(command);
                while (*after_command == ' ') after_command++; //теперь указатель на аргумент функции или на конец строки
            //
           printf("L501\n");
           printf("DEBUG ptr after_command: %c\n", *after_command);
            //
                if (strcmp(command, "PUSH") == 0)
                {
                    printf("L506\n");
                    if (ProcessPush(after_command, line_num, in_file, out) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "POP") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_POP);
                }
                else if (strcmp(command, "MUL") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_MUL);
                }
                else if (strcmp(command, "SUB") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_SUB);
                }
                else if (strcmp(command, "DIV") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_DIV);
                }
                else if (strcmp(command, "SQRT") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_SQRT);
                }
                else if (strcmp(command, "OUT") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_OUT);
                }
                else if (strcmp(command, "TOP") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_TOP);
                }
                else if (strcmp(command, "IN") == 0)
                {
                    if (*after_command != '\0') //проверка, что аргументов нет там, где их не должно быть. after_command указывает на то, что после пробелов после команды
                    {
                        printf("File: %s, Line: %d - syntax error: unnecessary arguments\n", in_file, line_num);
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                    fprintf(out, "%d\n", OP_IN);
                }
                else if (strcmp(command, "POPR") == 0)
                {
                    if (ProcessPopr(after_command, line_num, in_file, out) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "PUSHR") == 0)
                {
                    if (ProcessPushr(after_command, line_num, in_file, out) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                //jump   :d
                else if (strcmp(command, "JB") == 0)
                {

                printf("613");

                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JB) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JBE") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JBE) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JA") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JA) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JAE") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JAE) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JE") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JE) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JNE") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JNE) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "JNP") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_JNP) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                //CALL :d
                else if (strcmp(command, "CALL") == 0)
                {
                    if (ProcessJumpCommand(after_command, line_num, in_file, out, OP_CALL) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "RET") == 0)// Зацикленный CALL, это его конец
                {
                    fprintf(out, "%d\n", OP_RET);
                    fclose(out);
                    return SUCCESS;
                }
                //PUSHM POPM - [d]
                else if (strcmp(command, "PUSHM") == 0)
                {
                    if (ProcessPushm(after_command, line_num, in_file, out) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }
                else if (strcmp(command, "POPM") == 0)
                {
                    if (ProcessPopm(after_command, line_num, in_file, out) != SUCCESS)
                    {
                        fclose(out);
                        return SINTACSIS_ERR;
                    }
                }

                else if (strcmp(command, "HLT") == 0)
                {
                    fprintf(out, "%d\n", OP_HLT);
                    fclose(out);
                    return SUCCESS;
                }

                else //не команда
                {
                    printf("Error in file: %s, line: %d - syntax error: found command '%s'\n", in_file, line_num, command);
                    fclose(out);
                    return SINTACSIS_ERR;
                }
            }
        }

        //след строка
        if (newline_pos != NULL)     //again
        {
            current_pos = newline_pos + 1;
            line_num++;
        }
        else
        {
            break;
        }                           //end again
    }

    fclose(out);
    return SUCCESS;
}
