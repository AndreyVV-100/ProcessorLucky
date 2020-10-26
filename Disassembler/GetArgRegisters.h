switch (mach[*byte + 1])
{
    #define STR_CMP(name, num) case num: {                      \
                                    PrintStr (code, name "\n"); \
                                    (*byte)++;                  \
                                    break; }
    #include "../DefineFiles/Registers.h"
    #undef STR_CMP
    default:
        return 1;
}