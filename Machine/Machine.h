#pragma once
#include "../Libraries/Stack/Stack.h"
#include "../Libraries/Text/Text.h"

//ToDo: One signature file for all projects. 
const char   SIGNATURE[] = "AVV0.1";
const size_t LEN_SIGNATURE = 6;
const size_t STACK_SIZE = 100;

struct Processor
{
    Stack stk = {};
};

void ReadBinary (char** text, const char* file_name);

void StartProcessor (const char* text);

int CheckSignature (const char* text);

void ProcessorConstructor (Processor* pr);

void ProcessorDestructor  (Processor* pr);