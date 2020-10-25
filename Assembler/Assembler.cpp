#include "Assembler.h"

int main()
{
    Text code = {};
    ConstructorText (&code, "../Codes/asm.txt");

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

    for (size_t i_line = 0; i_line < code->n_empty_lines; i_line++)
    {
        if (ProcessLine (code->lines[i_line].point, &crc))
        {
            printf ("Assemblering error in line %d.", code->lines[i_line].position);
            CrcDestructor (&crc);
            return;
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

    char get_cmd[10] = "";
    int shift = -1;
    sscanf (command, " %s%n", get_cmd, &shift);
    if (*get_cmd == '\0')
        return 0;

    #define DEV_CMD(name, num, cmd) if (strcmp(get_cmd, name) == 0) {                   \
                                        sprintf (crc->mach + crc->bytes, "%c", num);    \
                                        crc->bytes++;                                   \
                                        } else

    #define DEV_CMD_ARG(name, num, cmd) if (strcmp(get_cmd, name) == 0) {               \
                                            sprintf (crc->mach + crc->bytes, "%c", num);\
                                            crc->bytes++;                               \
                                            if (GetArg (crc, command + shift))          \
                                                return 1;                               \
                                        } else

    #include "../Commands.h"
    #undef DEV_CMD
    #undef DEV_CMD_ARG
    /*else*/ return 1;

    return 0;
}

void PrintDouble (char* buffer, double num)
{
    assert (buffer);
    assert (!isnan (num));

    char* ch_num = (char*) &num;
    for (size_t i_byte = 0; i_byte < sizeof (num); i_byte++)
        sprintf (buffer + i_byte, "%c", ch_num[i_byte]);

    return;
}

void CrcConstructor (CreatorCode* crc, size_t code_size)
{
    assert (crc);
    assert (code_size);
    
    crc->mach   = (char*) calloc (code_size,     sizeof (char));
    crc->report = (char*) calloc (code_size * 5, sizeof (char));
    size_t bytes = 0;

    assert (crc->mach);
    assert (crc->report);

    return;
}

void CrcDestructor (CreatorCode* crc)
{
    assert (crc);
    
    free (crc->mach);
    free (crc->report);

    crc->mach   = nullptr;
    crc->report = nullptr;
    crc->bytes  = 0;

    return;
}

void CrcOutput (CreatorCode* crc, const char* out_name)
{
    assert (crc);

    FILE* file = fopen (out_name, "wb");
    assert (file);

    fprintf (file, "%s", SIGNATURE);
    fwrite (&(crc->bytes), sizeof (crc->bytes), 1, file);
    fwrite (crc->mach, sizeof (*(crc->mach)), crc->bytes, file);
    fclose (file);

    file = fopen ("../Codes/AssemblerLog.txt", "w");
    assert (file);
    fwrite (crc->report, sizeof (*(crc->report)), crc->size_report, file);
    fclose (file);

    return;
}

int GetArg (CreatorCode* crc, const char* command)
{
    assert (crc);
    assert (command);

    double put_arg_d = NAN;                           
    
    //sscanf (command, " %lf", &put_arg_d) < 1 - Double or not double?
    //*(crc->mach + crc->bytes - 1) == 2       - Pop or not pop?

    if (sscanf (command, " %lf", &put_arg_d) < 1 || *(crc->mach + crc->bytes - 1) == 2)
    {
        char put_arg_s[50] = "";
        if (sscanf (command, " %s", put_arg_s))
        {
            //ToDo: sprintf -> =
            #define STR_CMP(name, num) if (strcmp(name, put_arg_s) == 0) {               \
                                            crc->mach[crc->bytes - 1] |= 0b01000000;     \
                                            sprintf (crc->mach + crc->bytes, "%c", num); \
                                            crc->bytes++;                                \
                                            return 0; }
            #include "Registers.h"
            #undef STR_CMP
            
            //null pop
            if (*(crc->mach + crc->bytes - 1) == 2 && *put_arg_s == '\0')
            {
                *(crc->mach + crc->bytes - 1) |= 0b00100000;
                return 0;
            }
        }
        return 1;
    }

    *(crc->mach + crc->bytes - 1) |= 0b00100000;
    PrintDouble (crc->mach + crc->bytes, put_arg_d);  
    crc->bytes += sizeof (put_arg_d);
    return 0;
}
