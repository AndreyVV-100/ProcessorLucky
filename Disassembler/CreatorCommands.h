#define DEV_CMD(name, num, cmd) case (num): {                                 \
                                    PrintStr (&code, name "\n", &shift_code); \
                                    break;  }

#define DEV_CMD_ARG(name, num, cmd) case (num): {                                   \
                                        PrintStr (&code, name " ", &shift_code);    \
                                        GetArg (mach, &i_byte, &code, &shift_code); \
                                        break;  }

#include "../DefineFiles/Commands.h"

#undef DEV_CMD
#undef DEV_CMD_ARG