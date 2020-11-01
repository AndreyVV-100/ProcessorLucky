#include "Assembler.h"

int main()
{
    Text code = {};
    ConstructorText (&code, "../Codes/Factorial.txt");

    CreateMachineCode (&code, "../Codes/mach.avv");

    DestructorText (&code);

    return 0;
}

void CreateMachineCode (Text* code, const char* out_name)
{
    assert (code);
    assert (out_name);

    CreatorCode crc = {};

    CrcConstructor (&crc, code->size_text);

    for (size_t i_iter = 0; i_iter < 2; i_iter++)
    {
        crc.run_num   = i_iter;
        crc.all_bytes = crc.bytes;
        crc.bytes     = 0;

        for (size_t i_line = 0; i_line < code->n_empty_lines; i_line++)
        {
            if (ProcessLine (code->lines[i_line].point, &crc))
            {
                printf ("Assemblering error in line %d.", code->lines[i_line].position);
                CrcDestructor (&crc);
                return;
            }
        }
    }

    CrcOutput (&crc, out_name);
    CrcDestructor (&crc);

    printf ("Assemblering was successful!");
    return;
}

int ProcessLine (char* command, CreatorCode* crc)
{
    assert (command);
    assert (crc);
    
    char* check_comment = strchr (command, ';');
    if (check_comment != nullptr)
        *check_comment = '\0';

    char get_cmd[STR_TAG_LEN + 2] = "";
    int shift = -1;

    sscanf (command, " %s%n", get_cmd, &shift);
    if (*get_cmd == '\0')
        return 0;

    #include "ProcessLineCommands.h"

    return 0;
}

void PrintDouble (char* buffer, double num)
{
    assert (buffer);
    assert (!isnan (num));
    *(double*) buffer = num;
    return;
}

void PrintInt (char* buffer, size_t num)
{
    assert (buffer);
    *(size_t*) buffer = num;
    return;
}

void CrcConstructor (CreatorCode* crc, size_t code_size)
{
    assert (crc);
    assert (code_size);
    
    crc->bytes        =  0;
    crc->all_bytes    =  0;
    crc->run_num      = -1;
    crc->all_str_tags =  0;

    crc->mach   = (char*) calloc (code_size,     sizeof (char));
    assert (crc->mach);

    crc->report = (char*) calloc (code_size * 5, sizeof (char));
    assert (crc->report);

    crc->tags_num = (size_t*) calloc (TAGS_NUM, sizeof (size_t));
    assert (crc->tags_num);
    for (size_t i_tg = 0; i_tg < TAGS_NUM; i_tg++)
        crc->tags_num[i_tg] = SIZE_T_MAX;

    crc->tags_str = (TagStr*)  calloc (TAGS_STR, sizeof (TagStr));
    assert (crc->tags_str);
    for (size_t i_tg = 0; i_tg < TAGS_STR; i_tg++)
    {
        crc->tags_str[i_tg].str  = nullptr;
        crc->tags_str[i_tg].byte = SIZE_T_MAX;
    }

    assert (crc->mach);
    assert (crc->report);

    return;
}

void CrcDestructor (CreatorCode* crc)
{
    assert (crc);
    
    
    for (size_t i_tg = 0; i_tg < TAGS_STR; i_tg++)
    {
        free (crc->tags_str[i_tg].str);
        crc->tags_str[i_tg].str = nullptr;
        crc->tags_str[i_tg].byte = SIZE_T_MAX;
    }
        
    free (crc->mach);
    free (crc->report);
    free (crc->tags_num);
    free (crc->tags_str);

    crc->mach     = nullptr;
    crc->report   = nullptr;
    crc->tags_num = nullptr;
    crc->tags_str = nullptr;

    crc->bytes        =  0;
    crc->all_bytes    =  0;
    crc->run_num      = -1;
    crc->all_str_tags =  0;

    return;
}

void CrcOutput (CreatorCode* crc, const char* out_name)
{
    assert (crc);

    FILE* file = fopen (out_name, "wb");
    assert (file);

    fwrite (SIGNATURE,      sizeof (*SIGNATURE),    LEN_SIGNATURE,  file);
    fwrite (&(crc->bytes),  sizeof (crc->bytes),    1,              file);
    fwrite (crc->mach,      sizeof (*(crc->mach)),  crc->bytes,     file);

    fclose (file);
    file = fopen ("../Codes/AssemblerLog.txt", "w");
    assert (file);

    fwrite (crc->report, sizeof (*(crc->report)), crc->size_report, file);
    fclose (file);

    return;
}

int GetArg (CreatorCode* crc, char* command)
{
    assert (crc);
    assert (command);

    if (crc->mach[crc->bytes - 1] >= J_FIRST && crc->mach[crc->bytes - 1] <= J_LAST)
        return GoTag (crc, command);

    double put_arg_d = NAN;                           
    
    //sscanf (command, " %lf", &put_arg_d) < 1 - Double or not double?
    //crc->mach[crc->bytes - 1] == 2 - Pop or not pop?

    if (sscanf (command, " %lf", &put_arg_d) < 1 || crc->mach[crc->bytes - 1] == 2)
    {
        char put_arg_s[50] = "";
        if (sscanf (command, " %s", put_arg_s))
        {
            //registers include
            #include "GetArgRegisters.h"
            
            //null pop
            if (crc->mach[crc->bytes - 1] == 2 && *put_arg_s == '\0')
            {
                crc->mach[crc->bytes - 1] |= MODE_1;
                return 0;
            }
        }
        return 1;
    }

    crc->mach[crc->bytes - 1] |= MODE_1;
    PrintDouble (crc->mach + crc->bytes, put_arg_d);  
    crc->bytes += sizeof (put_arg_d);

    return 0;
}

int GoTag (CreatorCode* crc, char* command)
{
    size_t put_arg_i = SIZE_T_MAX;
    if (sscanf (command, " %u", &put_arg_i))
        return GoTagByte (crc, put_arg_i);

    char* colon = strchr (command, ':');

    if (colon != NULL)
    {
        *colon = ' ';
        if (sscanf (colon, " %u", &put_arg_i) && put_arg_i < TAGS_NUM)
            return GoTagNum (crc, put_arg_i, colon);

        return GoTagStr (crc, colon);
    }
    return 1;
}

int GoTagByte (CreatorCode* crc, size_t arg)
{
    assert (crc);
    
    if (crc->all_bytes <= arg && crc->run_num)
        return 1;

    crc->mach[crc->bytes - 1] |= MODE_1;
    PrintInt (crc->mach + crc->bytes, arg);
    crc->bytes += sizeof (arg);

    return 0;
}

int GoTagNum (CreatorCode* crc, size_t arg, char* colon)
{
    assert (crc);
    assert (colon);
    
    if (crc->tags_num[arg] == SIZE_T_MAX && crc->run_num == 1)
        return 1;

    crc->mach[crc->bytes - 1] |= MODE_2;
    PrintInt (crc->mach + crc->bytes, crc->tags_num[arg]);
    crc->bytes += sizeof (size_t);
    *colon = ':';
    
    return 0;
}

int GoTagStr (CreatorCode* crc, char* colon)
{
    assert (crc);
    assert (colon);
    
    if (crc->run_num == 0)
    {
        PrintInt (crc->mach + crc->bytes, SIZE_T_MAX);
        crc->bytes += sizeof (size_t);
        *colon = ':';
        return 0;
    }

    char* tag = colon + 1;
    if (*tag == '\0' || *tag == '\t' || *tag == ' ')
        return 1;

    while (*tag != '\0' && *tag != '\t' && *tag != ' ')
        tag++;
    *tag = '\0';

    size_t byte = GetStrTag (crc, colon + 1);
    if (byte == SIZE_T_MAX)
        return 1;

    PrintInt (crc->mach + crc->bytes, byte);
    crc->bytes += sizeof (size_t);
    return 0;
}

size_t GetStrTag (CreatorCode* crc, char* str)
{
    assert (crc);
    assert (str);

    size_t i_tag = 0;
    while (crc->tags_str[i_tag].str != nullptr &&
           strcmp (crc->tags_str[i_tag].str, str) &&
           i_tag < TAGS_STR)
        i_tag++;
    
    if (i_tag == TAGS_STR)
        return SIZE_T_MAX;

    return crc->tags_str[i_tag].byte;
}
