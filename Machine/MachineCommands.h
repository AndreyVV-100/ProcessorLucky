#define CHECK_ARGS if ((mach[save_byte] & MODE_2) == MODE_2)    \
        {                                                       \
            if (mach[byte_now + 1] > 3)                         \
                ExitError (&pr, mach_free, save_byte);          \
                                                                \
            result += pr.rx[mach[++byte_now]];                  \
        }                                                       \
                                                                \
        if ((mach[save_byte] & MODE_1) == MODE_1)               \
        {                                                       \
            result += *((double*)(mach + byte_now + 1));        \
            byte_now += sizeof (double);                        \
        }

#define ass if (pr.stk.status_error != 0) {								\
			    printf ("\n" "Error in byte: %d. Exiting...", byte_now);\
				exit (pr.stk.status_error);}

#define DO_PUSH(number) StackPush (&(pr.stk), number)                  
#define DO_POP  StackPop  (&(pr.stk))

#define DEV_CMD(name, num, cmd)     case (num): cmd break;
#define DEV_CMD_ARG(name, num, cmd) case (num): cmd break;

#include "../DefineFiles/Commands.h"

#undef DEV_CMD
#undef DEV_CMD_ARG
#undef DO_PUSH
#undef DO_POP
#undef CHECK_ARGS

