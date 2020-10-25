#pragma once

const char   SIGNATURE[] = "AVV0.2";
const size_t LEN_SIGNATURE = 6;

void ReadBinary (char** text, const char* file_name);

int CheckSignature (const char* text);