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

/*else*/ if (get_cmd[1] == ':' && Isnum (get_cmd[0]))
            crc->tags[get_cmd[0] - '0'] = crc->bytes;

else return 1;