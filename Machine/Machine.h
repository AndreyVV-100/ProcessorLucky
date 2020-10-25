#pragma once
#include "../Libraries/Stack/Stack.h"
#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"

const size_t STACK_SIZE = 100;

struct Processor
{
    Stack stk = {};
};

void StartProcessor (char* mach);

void ProcessorConstructor (Processor* pr);

void ProcessorDestructor  (Processor* pr);