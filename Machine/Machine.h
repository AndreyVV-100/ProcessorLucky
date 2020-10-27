#pragma once
#include "../Libraries/Stack/Stack.h"
#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"

const size_t STACK_SIZE = 100;

struct Processor
{
    Stack stk  = {};
    Stack call = {};
    double rx[4] = {};
};

void GoProcessor (char* mach);

size_t StartProcessor (Processor* pr, char* *mach);

void FinishProcessor (Processor* pr, char* mach);

void ProcessorConstructor (Processor* pr);

void ProcessorDestructor  (Processor* pr);

void ExitError (Processor* pr, char* mach, size_t byte);
