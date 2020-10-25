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
    assert (mach);
    assert (file_name);

    char* mach_free = mach;

    if (CheckSignature (mach))
    {
        printf ("Bad signature of file. Try to use another version of disassembler.");
        exit (1);
    }
    mach += LEN_SIGNATURE;

    size_t num_bytes = *((int*) mach);
    mach += sizeof (num_bytes);

    char* code = (char*) calloc (num_bytes * 10, sizeof (*code));
    assert (code);
    char* code_now = code;

    size_t shift = -1;

    for (size_t i_byte = 0; i_byte < num_bytes; i_byte++)
    {
        switch (mach[i_byte])
        {
            #define DEV_CMD(name, num, cmd) case (num): { sprintf (code_now, "%s\n%n",  \
                                                                   name, &shift);       \
                                                        code_now += shift;              \
                                                        break;  }

            #define DEV_CMD_ARG(name, num, cmd) case (num): { sprintf  (code_now,           \
                                                                        "%s %lf\n%n",       \
                                                    name, *((double*)(mach + i_byte + 1)),  \
                                                                        &shift);            \
                                                    code_now += shift;                      \
                                                    i_byte += sizeof (double);              \
                                                    break;  }

            #include "../Commands.h"

            #undef DEV_CMD
            #undef DEV_CMD_ARG

            default:
                printf ("Disassemblering error in byte %d.", i_byte);
                free (code);
                free (mach_free);
                exit (1);
        }
    }

    FILE* file = fopen (file_name, "w");
    fwrite (code, sizeof (*code), num_bytes * 10, file);

    fclose (file);
    free (code);
    free (mach_free);

    printf ("Disassemblering was successful!");
    return;
}
