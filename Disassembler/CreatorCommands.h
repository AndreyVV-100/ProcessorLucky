#define DEV_CMD(name, num, cmd) case (num): {                   \
                                    PrintStr (&code_now, name "\n"); \
                                    break;  }

#define DEV_CMD_ARG(name, num, cmd) case (num): {                               \
                                        PrintStr (&code_now, name " ");         \
                                        GetArg (mach, &i_byte, &code_now);      \
                                        break;  }

#include "../DefineFiles/Commands.h"

#undef DEV_CMD
#undef DEV_CMD_ARG