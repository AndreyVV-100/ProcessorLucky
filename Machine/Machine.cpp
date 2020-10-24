#include "Machine.h"

//ToDo: One signature file for all projects.

int main()
{
    char* mach = nullptr;
    ReadBinary (&mach, "../Assembler/CompileFiles/mach.txt");
    StartProcessor (mach);
    
    return 0;
}

void ReadBinary (char** text, const char* file_name)
{
    FILE* file = fopen (file_name, "rb");
    assert (file);

    size_t file_size = CountSize (file);
    *text = (char*) calloc (file_size + 1, sizeof (**text));
    assert (*text);

    fread (*text, sizeof (*text), file_size, file);
    fclose (file);

    return;
}

void StartProcessor (const char* mach)
{
    assert (mach);
    if (CheckSignature (mach))
    {
        printf ("Bad signature of file. Try to use another version of processor.");
        exit (1);
    }

    Processor pr = {};
    ProcessorConstructor (&pr);

    size_t num_bytes = *((size_t*) (mach + LEN_SIGNATURE));
    mach += LEN_SIGNATURE + sizeof (num_bytes);

    for (size_t byte_now = 0; byte_now < num_bytes; byte_now++)
    {
        switch (mach[byte_now])
        {
            #define ass if (pr.stk.status_error != 0) {	                    \
			    printf ("\n" "Error in byte: %d. Exiting...", byte_now);	\
				exit (pr.stk.status_error);}
            #define DO_PUSH(number) StackPush (&(pr.stk), number)                  
            #define DO_POP  StackPop  (&(pr.stk))
            #define DEV_CMD(name, num, cmd)     case (num): cmd break;
            #define DEV_CMD_ARG(name, num, cmd) case (num): cmd break;

            #include "../Commands.h"

            #undef DEV_CMD
            #undef DEV_CMD_ARG
            #undef DO_PUSH
            #undef DO_POP

            default:
                printf ("Unknown command.");
                exit (1);
        }
    }
    ProcessorDestructor (&pr);
}

int CheckSignature (const char* text)
{
    assert (text);

    for (size_t i_symb = 0; i_symb < LEN_SIGNATURE; i_symb++)
        if (text[i_symb] != SIGNATURE[i_symb])
            return 1;

    return 0;
}

void ProcessorConstructor (Processor* pr)
{
    assert (pr);
    StackConstructor (&(pr->stk), STACK_SIZE);
    return;
}

void ProcessorDestructor (Processor* pr)
{
    assert (pr);
    StackDestructor (&(pr->stk));
    return;
}