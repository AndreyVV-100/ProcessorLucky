#include "Disassembler.h"

int main()
{
    _set_printf_count_output (1);

    char* mach = nullptr;
    ReadBinary (&mach, "../Codes/mach.avv");
    CreateAssemblerCode (mach, "../Codes/disasm.txt");

    return 0;
}

void CreateAssemblerCode (char* mach, const char* file_name)
{
    char* mach_free = mach;
    char* code = nullptr;

    size_t num_bytes = StartCreate (&mach, &code, file_name);

    size_t shift_code = 0;

    for (size_t i_byte = 0; i_byte < num_bytes; i_byte++)
    {
        switch (mach[i_byte] & CMD_MASK)
        {
            #include "CreatorCommands.h"

            default:
                ExitError (code, mach_free, i_byte);
        }
    }

    FILE* file = fopen (file_name, "w");
    fwrite (code, sizeof (*code), shift_code, file);

    fclose (file);
    
    EndCreate (code, mach_free);

    printf ("Disassemblering was successful!");
    return;
}

size_t StartCreate (char** mach, char** code, const char* file_name)
{
    assert (mach);
    assert (*mach);
    assert (file_name);

    if (CheckSignature (*mach))
    {
        printf ("Bad signature of file. Try to use another version of disassembler.");
        exit (1);
    }
    *mach += LEN_SIGNATURE;

    size_t num_bytes = *((int*) *mach);
    *mach += sizeof (num_bytes);

    *code = (char*) calloc (num_bytes * 10, sizeof (*code));
    assert (*code);  

    return num_bytes;
}

void EndCreate (char* code, char* mach)
{
    free (code);
    free (mach);

    return;
}

void ExitError (char* code, char* mach, size_t byte)
{
    EndCreate (code, mach);
    printf ("Disassemblering error in byte %d.", byte);

    exit (1);
}

int GetArg (char* mach, size_t* byte, char** code, size_t* shift_code)
{
    assert (mach);
    assert (byte);
    assert (code);
    assert (*code);

    if ((mach[*byte] & CMD_MASK) >= J_FIRST && (mach[*byte] & CMD_MASK) <= J_LAST)
    {
        PrintInt (code, mach + *byte + 1, shift_code);
        *byte += sizeof (size_t);
        return 0;
    }

    switch (mach[*byte])
    {
        case POP_NULL:
            PrintStr (code, "\n", shift_code);
            return 0;
        case POP_RX:
        case PUSH_RX:
            #include "GetArgRegisters.h"
            return 0;
        case PUSH_NUM:
            PrintDouble (code, mach + *byte + 1, shift_code);
            *byte += sizeof (double);
            return 0;
        default:
            return 1;
    }
}

void PrintStr (char** code, const char* str, size_t* shift_code)
{
    assert (code);
    assert (*code);
    assert (str);

    size_t shift = 0;
    sprintf (*code + *shift_code, "%s%n", str, &shift);  
    *shift_code += shift;

    return;
}

void PrintDouble (char** code, const char* mach, size_t* shift_code)
{
    assert (code);
    assert (*code);
    assert (mach);

    size_t shift = 0;
    sprintf (*code + *shift_code, "%lf\n%n", *((double*) mach), &shift);
    *shift_code += shift;

    return;
}

void PrintInt (char** code, const char* mach, size_t* shift_code)
{
    assert (code);
    assert (*code);
    assert (mach);

    size_t shift = 0;
    sprintf (*code + *shift_code, "%u\n%n", *((size_t*) mach), &shift);
    *shift_code += shift;

    return;
}
