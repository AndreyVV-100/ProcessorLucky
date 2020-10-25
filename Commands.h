DEV_CMD_ARG ("push", 1,
    {
        if ((mach[byte_now] & 0b11100000) == 0b00100000)
        {
            DO_PUSH (*((double*) (mach + byte_now + 1)));
            ass
            byte_now += sizeof (double);
        }
        else if ((mach[byte_now] & 0b11100000) == 0b01000000 &&
                 mach[byte_now + 1] >= 1 &&
                 mach[byte_now + 1] <= 4)
        {
            DO_PUSH (pr.rx[mach[byte_now + 1] - 1]);
            ass
            byte_now += sizeof (char);
        }
        else
        {
            printf ("Unknown command1.");
            free (mach_free);
            ProcessorDestructor (&pr);
            exit (1);
        }
    })

DEV_CMD_ARG ("pop", 2,
    {
        if ((mach[byte_now] & 0b11100000) == 0b00100000)
        {
            DO_POP;
            ass
        }
        else if ((mach[byte_now] & 0b11100000) == 0b01000000 &&
                 mach[byte_now + 1] >= 1 &&
                 mach[byte_now + 1] <= 4)
        {
            pr.rx[mach[byte_now + 1] - 1] = DO_POP;
            ass
            byte_now += sizeof (char);
        }
        else
        {
            printf ("Unknown command.%d", byte_now);
            free (mach_free);
            ProcessorDestructor (&pr);
            exit (1);
        }
    })

DEV_CMD ("add", 3,
    {
        DO_PUSH (DO_POP + DO_POP);
        ass
    })

DEV_CMD("mul",  4,
    {
        DO_PUSH (DO_POP * DO_POP);
        ass
    })

DEV_CMD("sub",  5, 
    {
        DO_PUSH (-DO_POP + DO_POP);
        ass
    })

DEV_CMD("div",  6, 
    {
        double num = DO_POP;
        ass
        if (num == 0)
        {
            printf ("Division by zero!");
            ProcessorDestructor (&pr);
            exit (1);
        }
        DO_PUSH (DO_POP / num);
        ass
    })

DEV_CMD("sin",  7, 
    {
        DO_PUSH (sin (DO_POP));
        ass
    })

DEV_CMD("cos",  8, 
    {
        DO_PUSH (cos (DO_POP));
        ass
    })

DEV_CMD("sqrt", 9, 
    {
        double num = DO_POP;
        ass
        if (num < 0)
        {
            printf ("Bad argument of sqrt!");
            ProcessorDestructor (&pr);
            exit (1);
        }
        DO_PUSH (sqrt (num));
        ass
    })

DEV_CMD("neg",  10, 
    {
        DO_PUSH (-DO_POP);
        ass
    })

DEV_CMD("out",  11, 
    {
        printf ("Output: %lf\n", DO_POP);
        ass
    })

DEV_CMD("in",   12, 
{    
    printf ("Input: ");
    double num = NAN;
    if (!scanf ("%lf", &num))
    {
        printf ("Bad input! Ending processing...");
        ProcessorDestructor (&pr);
        exit (1);
    }
    DO_PUSH (num);
    ass
})

DEV_CMD("dumb", 13, 
    {
        StackLog (&(pr.stk));
    })

DEV_CMD("hlt",  14, 
    {
        printf ("Halt end exit...");
        ProcessorDestructor (&pr);
        exit (0);
    })
