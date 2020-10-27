#pragma once

#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"
#include <math.h>

struct CreatorCode
{
    char* mach   = nullptr;
    char* report = nullptr;
    size_t bytes = 0;
    size_t size_report = 0;
    size_t tags[10] = {};
};

const size_t J_FIRST = 15;
const size_t J_LAST  = 22;

void CreateMachineCode (Text* code, const char* out_name);

int ProcessLine (char* command, CreatorCode* crc);

void PrintDouble (char* buffer, double num);

void CrcConstructor (CreatorCode* crc, size_t code_size);

void CrcDestructor (CreatorCode* crc);

void CrcOutput (CreatorCode* crc, const char* out_name);

int GetArg (CreatorCode* crc, const char* command);

void PrintInt (char* buffer, size_t num);

int Isnum (char c);
