#define DEV_CMD(name, num, cmd) if (strcmp(get_cmd, name) == 0) {                   \
                                        crc->mach[crc->bytes] = num;                \
                                        crc->bytes++;                               \
                                        } else

#define DEV_CMD_ARG(name, num, cmd) if (strcmp(get_cmd, name) == 0) {           \
                                            crc->mach[crc->bytes] = num;        \
                                            crc->bytes++;                       \
                                            if (GetArg (crc, command + shift))  \
                                                return 1;                       \
                                        } else

#include "../DefineFiles/Commands.h"
#undef DEV_CMD
#undef DEV_CMD_ARG

/*else*/
{
    char* colon = strchr (command, ':');
    //do tag
    if (colon != NULL)
    {
        if (crc->run_num == 1)
            return 0;
        
        *colon = '\0';
        size_t put_arg_i = SIZE_T_MAX;

        if (sscanf (command, " %u", &put_arg_i) && put_arg_i < TAGS_NUM)
        {
            if (put_arg_i >= TAGS_NUM)
                return 1;
            
            crc->tags_num[put_arg_i] = crc->bytes;
            *colon = ':';
            return 0;
        }

        if (crc->all_str_tags == TAGS_STR)
            return 1;

        crc->tags_str[crc->all_str_tags].str = (char*)calloc (STR_TAG_LEN, sizeof (char));
        assert (crc->tags_str[crc->all_str_tags].str);
        
        crc->tags_str[crc->all_str_tags].byte = crc->bytes;
        strcpy (crc->tags_str[crc->all_str_tags].str, command);

        crc->all_str_tags++;
        *colon = ':';

        return 0;
    }

    return 1;
}
