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

