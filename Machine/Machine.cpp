#include "Machine.h"

int main()
{
    char* mach = nullptr;
    ReadBinary (&mach, "../Codes/mach.avv");
    StartProcessor (mach);
    
    return 0;
}

void StartProcessor (char* mach)
{
    assert (mach);
    char* mach_free = mach;

    if (CheckSignature (mach))
    {
        printf ("Bad signature of file. Try to use another version of processor.");
        exit (1);
    }
    mach += LEN_SIGNATURE;

    Processor pr = {};
    ProcessorConstructor (&pr);


    size_t num_bytes = *((size_t*) mach);
    mach += sizeof (num_bytes);

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
                free (mach_free);
                exit (1);
        }
    }

    free (mach_free);
    ProcessorDestructor (&pr);
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