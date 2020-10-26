#define DEV_CMD(name, num, cmd) if (strcmp(get_cmd, name) == 0) {                   \
                                        crc->mach[crc->bytes] = num;                \
                                        crc->bytes++;                               \
                                        } else

#define DEV_CMD_ARG(name, num, cmd) if (strcmp(get_cmd, name) == 0) {               \
                                            crc->mach[crc->bytes] = num;            \
                                            crc->bytes++;                           \
                                            if (GetArg (crc, command + shift))      \
                                                return 1;                           \
                                        } else

#include "../DefineFiles/Commands.h"
#undef DEV_CMD
#undef DEV_CMD_ARG

/*else*/ return 1;