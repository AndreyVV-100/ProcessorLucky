#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../Libraries/BinaryText/BinaryText.h"

#pragma warning (disable : 4996)

const char POP_NULL = 0b00100010;
const char POP_RX   = 0b01000010;
const char PUSH_NUM = 0b00100001;
const char PUSH_RX  = 0b01000001;

void CreateAssemblerCode (char* mach, const char* file_name);

size_t StartCreate (char** mach, char** code, const char* file_name);

void EndCreate (char* code, char* mach);

void ExitError (char* code, char* mach, size_t byte);

int GetArg (char* mach, size_t* byte, char** code);

void PrintStr (char** code, const char* str);

void PrintDouble (char** code, const char* mach);
