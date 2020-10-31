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
        crc.bytes = 0;
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

    Checktags_num (&crc);
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

    char get_cmd[10] = "";
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
    
    crc->mach   = (char*) calloc (code_size,     sizeof (char));
    crc->report = (char*) calloc (code_size * 5, sizeof (char));

    crc->tags_num = (TagNum*) calloc (TAGS_NUM, sizeof (TagNum));
    for (size_t i_tg = 0; i_tg < TAGS_NUM; i_tg++)
    {
        crc->tags_num[i_tg].tg   = SIZE_T_MAX;
        crc->tags_num[i_tg].used = 0;
    }

    assert (crc->mach);
    assert (crc->report);

    return;
}

void CrcDestructor (CreatorCode* crc)
{
    assert (crc);
    
    free (crc->mach);
    free (crc->report);
    free (crc->tags_num);

    crc->mach   = nullptr;
    crc->report = nullptr;
    crc->tags_num   = nullptr;
    crc->bytes  = 0;

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
    {
        crc->mach[crc->bytes - 1] |= MODE_1;
        PrintInt (crc->mach + crc->bytes, put_arg_i);
        crc->bytes += sizeof (put_arg_i);
        return 0;
    }

    char* colon = strchr (command, ':');

    if (colon != NULL)
    {
        *colon = ' ';
        if (sscanf (command, " %u", &put_arg_i) && put_arg_i < TAGS_NUM)
        {
            crc->mach[crc->bytes - 1] |= MODE_2;
            crc->tags_num[put_arg_i].used = 1;
            PrintInt (crc->mach + crc->bytes, crc->tags_num[put_arg_i].tg);
            crc->bytes += sizeof (size_t);
            *colon = ':';
            return 0;
        }
    }
    return 1;
}

void Checktags_num (CreatorCode* crc)
{
    assert (crc);

    for (size_t i_tg = 0; i_tg < TAGS_NUM; i_tg++)
    {
        if (crc->tags_num[i_tg].used && crc->tags_num[i_tg].tg == SIZE_T_MAX)
        {
            printf ("tags_num error on tag %u: it was used, but wasn`t declared.", i_tg);
            CrcDestructor (crc);
            exit (1);            
        }
    }
}