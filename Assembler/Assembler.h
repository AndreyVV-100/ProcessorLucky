#pragma once

#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"
#include "../DefineFiles/Modes.h"
#include <math.h>

const size_t SIZE_T_MAX = -1;
const size_t TAGS_NUM = 10000;
const size_t TAGS_STR = 10000;
const size_t STR_TAG_LEN = 50;

struct TagStr
{
    char* str   = nullptr;
    size_t byte = SIZE_T_MAX;
};

struct CreatorCode
{
    char* mach   = nullptr;
    char* report = nullptr;

    int run_num = -1;
    size_t bytes = 0;
    size_t all_bytes = 0;
    size_t size_report = 0;
    size_t all_str_tags = 0;

    size_t* tags_num = nullptr;
    TagStr* tags_str = nullptr;
};

void CreateMachineCode (Text* code, const char* out_name);

int ProcessLine (char* command, CreatorCode* crc);

void PrintDouble (char* buffer, double num);

void CrcConstructor (CreatorCode* crc, size_t code_size);

void CrcDestructor (CreatorCode* crc);

void CrcOutput (CreatorCode* crc, const char* out_name);

int GetArg (CreatorCode* crc, char* command);

void PrintInt (char* buffer, size_t num);

int GoTag (CreatorCode* crc, char* command);

int GoTagByte (CreatorCode* crc, size_t arg);

int GoTagNum (CreatorCode* crc, size_t arg, char* colon);

int GoTagStr (CreatorCode* crc, char* colon);

size_t GetStrTag (CreatorCode* crc, char* str);

int PoPuArg (CreatorCode* crc, char* command);
