#include "Machine.h"

int main()
{
    char* text = nullptr;
    ReadBinary (&text, "../Assembler/ComppileFiles/mach.txt");
    //StartProcessor (text);
    printf ("%s", text);
}

void ReadBinary (char** text, const char* file_name)
{
    FILE* file = fopen (file_name, "rb");
    assert (file);

    size_t file_size = CountSize (file);
    *text = (char*) calloc (file_size + 1, sizeof (**text));
    assert (*text);

    fread (text, sizeof (*text), file_size, file);
    fclose (file);

    return;
}

void StartProcessor (const char* text)
{

}
