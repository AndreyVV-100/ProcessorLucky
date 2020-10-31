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

/*else*/
{
    char* colon = strchr (command, ':');

    if (colon != NULL)
    {
        *colon = '\0';
        size_t put_arg_i = SIZE_T_MAX;

        if (sscanf (command, " %u", &put_arg_i) && put_arg_i < TAGS_NUM)
        {
            crc->tags_num[put_arg_i].tg = crc->bytes;
            *colon = ':';
            return 0;
        }
    }

    return 1;
}
