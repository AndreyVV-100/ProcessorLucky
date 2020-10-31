#pragma once

#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"
#include "../DefineFiles/Modes.h"
#include <math.h>

const size_t J_FIRST = 15;
const size_t J_LAST = 22;
const size_t SIZE_T_MAX = -1;
const size_t TAGS_NUM = 10000;

struct TagNum
{
    size_t tg = SIZE_T_MAX;
    size_t used = 0;
};

struct CreatorCode
{
    char* mach   = nullptr;
    char* report = nullptr;
    size_t bytes = 0;
    size_t size_report = 0;
    TagNum* tags_num = nullptr;
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

void Checktags_num (CreatorCode* crc);
