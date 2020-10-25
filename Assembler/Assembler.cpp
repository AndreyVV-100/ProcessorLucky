#include "Assembler.h"

int main()
{
    Text code = {};
    ConstructorText (&code, "../Codes/asm.txt");

    CreateMachineCode (&code, "../Codes/mach.txt");

    DestructorText (&code);

    return 0;
}

void CreateMachineCode (Text* code, const char* out_name)
{
    assert (code);
    assert (out_name);

    char* file_out = (char*) calloc (code->size_text, sizeof (*file_out));
    size_t bytes = 0;

    for (size_t i_line = 0; i_line < code->n_empty_lines; i_line++)
    {
        if (ProcessLine (code->lines[i_line].point, file_out, &bytes))
        {
            printf ("Assemblering error in line %d.", code->lines[i_line].position);
            exit (1);
        }
    }

    FILE* file = fopen (out_name, "wb");
    assert (file);

    fprintf (file, "AVV0.1");
    fwrite (&bytes, sizeof (bytes), 1, file);
    fwrite (file_out, sizeof (*file_out), bytes, file);
    fclose (file);

    free (file_out);
    printf ("Assemblering was successful!");
    return;
}

int ProcessLine (char* command, char* file_out, size_t* bytes)
{
    assert (command);
    assert (file_out);
    
    char* check_comment = strchr (command, ';');
    if (check_comment != nullptr)
        *check_comment = '\0';

    char get_cmd[10] = "";
    int shift = -1;
    sscanf (command, " %s%n", get_cmd, &shift);
    if (*get_cmd == '\0')
        return 0;

    #define DEV_CMD(name, num, cmd) if (strcmp(get_cmd, name) == 0) {            \
                                        sprintf (file_out + *bytes, "%c", num);  \
                                        (*bytes)++;                              \
                                        } else

    #define DEV_CMD_ARG(name, num, cmd) if (strcmp(get_cmd, name) == 0) {           \
                                        sprintf (file_out + *bytes, "%c", num);     \
                                        (*bytes)++;                                 \
                                        double put_arg = NAN;                       \
                        int check_arg = sscanf (command + shift, " %lf", &put_arg); \
                                        if (check_arg < 1)                          \
                                            return 1;                               \
                                        PrintDouble (file_out + *bytes, put_arg);   \
                                        (*bytes) += sizeof(put_arg);                \
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
