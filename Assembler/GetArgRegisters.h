#define REGISTER(name, num) if (strcmp(name, put_arg_s) == 0) {                     \
                                            crc->mach[crc->bytes - 1] |= MODE_2;    \
                                            crc->mach[crc->bytes] = num;            \
                                            crc->bytes++;                           \
                                            command += shift; }

#include "../DefineFiles/Registers.h"

#undef REGISTER