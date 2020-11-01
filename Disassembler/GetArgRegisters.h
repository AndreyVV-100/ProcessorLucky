switch (mach[*byte + 1])
{
    #define REGISTER(name, num) case num: {                                 \
                                    PrintStr (code, name "\n", shift_code); \
                                    (*byte)++;                              \
                                    break; }
    #include "../DefineFiles/Registers.h"
    #undef REGISTER
    default:
        return 1;
}