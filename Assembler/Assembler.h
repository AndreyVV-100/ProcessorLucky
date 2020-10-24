#pragma once

#include "Text.h"
#include <math.h>

/*
enum commands
{
	#define DEV_CMD(name, num) CMD_##name = num,
	#include "Commands.h"
	#undef DEV_CMD
};
*/

void CreateMachineCode (Text* code, const char* out_name);

int ProcessLine (char* command, char* file_out, size_t* bytes);