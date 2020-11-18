#pragma once
#include "../Libraries/Stack/Stack.h"
#include "../Libraries/Text/Text.h"
#include "../Libraries/BinaryText/BinaryText.h"
#include "../DefineFiles/Modes.h"

const size_t STACK_SIZE = 100;
const size_t RAM_SIZE   = 1e6;

struct VRAM
{
    size_t size_x = 0;
    size_t size_y = 0;
    size_t place_in_RAM = 0;
    int VRAM_on = 0;
};

struct Processor
{
    Stack stk  = {};
    Stack call = {};
    double rx[4] = {};
    double* RAM  = nullptr;
    VRAM vr = {};
};

void GoProcessor (char* mach);

size_t StartProcessor (Processor* pr, char* *mach);

void FinishProcessor (Processor* pr, char* mach);

void ProcessorConstructor (Processor* pr);

void ProcessorDestructor  (Processor* pr);

void ExitError (Processor* pr, char* mach, size_t byte);

void UpdateScreen (Processor* pr);
