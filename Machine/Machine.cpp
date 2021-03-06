﻿#include "Machine.h"

int main()
{
    char* mach = nullptr;
    ReadBinary (&mach, "../Codes/mach.avv");
    GoProcessor (mach);
    
    return 0;
}

size_t StartProcessor (Processor* pr, char** mach)
{
    assert (mach);
    assert (*mach);

    if (CheckSignature (*mach))
    {
        printf ("Bad signature of file. Try to use another version of processor.");
        exit (1);
    }
    *mach += LEN_SIGNATURE;

    ProcessorConstructor (pr);

    size_t num_bytes = *((size_t*) *mach);
    *mach += sizeof (num_bytes);

    return num_bytes;
}

void GoProcessor (char* mach)
{
    char* mach_free = mach;
    Processor pr = {};
    
    size_t num_bytes = StartProcessor (&pr, &mach);

    for (size_t byte_now = 0; byte_now < num_bytes; byte_now++)
    {
        switch (mach[byte_now] & CMD_MASK)
        {
            #include "MachineCommands.h"
            default:
                ExitError (&pr, mach_free, byte_now);
        }
    }

    FinishProcessor (&pr, mach_free);
}

void FinishProcessor (Processor* pr, char* mach)
{
    assert (pr);

    ProcessorDestructor (pr);
    free (mach);

    return;
}

void ProcessorConstructor (Processor* pr)
{
    assert (pr);
    StackConstructor (&(pr->stk),  STACK_SIZE);
    StackConstructor (&(pr->call), STACK_SIZE);
    pr->RAM = (double*) calloc (RAM_SIZE, sizeof (*(pr->RAM)));

    for (size_t i_rx = 0; i_rx < 4; i_rx++)
        pr->rx[i_rx] = NAN;
    return;
}

void ProcessorDestructor (Processor* pr)
{
    assert (pr);
    StackDestructor (&(pr->stk));
    StackDestructor (&(pr->call));
    free (pr->RAM);
    pr->RAM = nullptr;

    for (size_t i_rx = 0; i_rx < 4; i_rx++)
        pr->rx[i_rx] = NAN;
    return;
}

void ExitError (Processor* pr, char* mach, size_t byte)
{
    FinishProcessor (pr, mach);
    printf ("Unknown command in byte %d. Exiting...", byte);

    exit (1);
}

void UpdateScreen (Processor* pr)
{
    assert (pr);

    system ("cls");
    for (size_t y = 0; y < pr->vr.size_y; y++)
    {
        for (size_t x = 0; x < pr->vr.size_x; x++)
        {
            int symb = pr->RAM[pr->vr.place_in_RAM + x + y * pr->vr.size_x];
            
            printf ("%c", symb);
            
            if (symb == 0)
                printf ("?");

            if (symb == '\n')
                printf ("!!!");
        }
        printf ("\n");
    }

    return;
}
