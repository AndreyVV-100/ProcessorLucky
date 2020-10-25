#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../Text/Text.h"
#include "BinaryText.h"

void ReadBinary (char** text, const char* file_name)
{
    FILE* file = fopen (file_name, "rb");
    assert (file);

    size_t file_size = CountSize (file);
    *text = (char*) calloc (file_size + 1, sizeof (**text));
    assert (*text);

    fread (*text, sizeof (*text), file_size, file);
    fclose (file);

    return;
}

int CheckSignature (const char* text)
{
    assert (text);

    for (size_t i_symb = 0; i_symb < LEN_SIGNATURE; i_symb++)
        if (text[i_symb] != SIGNATURE[i_symb])
            return 1;

    return 0;
}