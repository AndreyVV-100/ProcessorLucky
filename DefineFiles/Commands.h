DEV_CMD_ARG ("push", 1,
    {
        if ((mach[byte_now] & MODE_MASK) == MODE_3 || !(mach[byte_now] & MODE_MASK))
            ExitError (&pr, mach_free, byte_now);

        double result = 0;
        size_t save_byte = byte_now;

        CHECK_ARGS

        if ((mach[save_byte] & MODE_3) == MODE_3)
        {
            if (result < 0)
                ExitError (&pr, mach_free, save_byte);
            DO_PUSH (pr.RAM[(int) result]);
        }
        else
            DO_PUSH (result);

        ass
    })

DEV_CMD_ARG ("pop", 2,
    {
        double result = 0;
        size_t save_byte = byte_now;

        if (!(mach[save_byte] & MODE_MASK))
            DO_POP;

        else if ((mach[save_byte] & MODE_MASK) == MODE_2)
        {
            if (mach[byte_now + 1] > 3)
                ExitError (&pr, mach_free, save_byte);

            pr.rx[mach[byte_now + 1]] = DO_POP;
            byte_now += sizeof (char);
        }

        else if ((mach[save_byte] & MODE_3) == MODE_3)
        {
            CHECK_ARGS
            
            if (result < 0)
                ExitError (&pr, mach_free, save_byte);

            pr.RAM[(int) result] = DO_POP;
        }
        else
            ExitError (&pr, mach_free, save_byte);
        ass
    })

DEV_CMD ("add", 3,
    {
        DO_PUSH (DO_POP + DO_POP);
        ass
    })

DEV_CMD ("mul",  4,
    {
        DO_PUSH (DO_POP * DO_POP);
        ass
    })

DEV_CMD ("sub",  5, 
    {
        DO_PUSH (-DO_POP + DO_POP);
        ass
    })

DEV_CMD ("div",  6, 
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

DEV_CMD ("sin",  7, 
    {
        DO_PUSH (sin (DO_POP));
        ass
    })

DEV_CMD ("cos",  8, 
    {
        DO_PUSH (cos (DO_POP));
        ass
    })

DEV_CMD ("sqrt", 9, 
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

DEV_CMD ("neg",  10, 
    {
        DO_PUSH (-DO_POP);
        ass
    })

DEV_CMD ("out",  11, 
    {
        printf ("Output: %lf\n", DO_POP);
        ass
    })

DEV_CMD ("in",   12, 
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

DEV_CMD ("dumb", 13, 
    {
        StackLog (&(pr.stk));
    })

DEV_CMD ("hlt",  14, 
    {
        printf ("Halt end exit...");
        ProcessorDestructor (&pr);
        exit (0);
    })

DEV_CMD_ARG ("jmp", 15, 
    {
        byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
    })

DEV_CMD_ARG ("ja", 16, 
    {
        if (DO_POP < DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("jae", 17, 
    {
        if (DO_POP <= DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("jb", 18, 
    {
        if (DO_POP > DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("jbe", 19, 
    {
        if (DO_POP >= DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else 
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("je", 20, 
    {
        if (DO_POP == DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("jne", 21, 
    {
        if (DO_POP != DO_POP)
            byte_now = *((size_t*) (mach + byte_now + 1)) - 1;
        else
            byte_now += sizeof (size_t);
        ass
    })

DEV_CMD_ARG ("call", 22,
    {
        StackPush (&pr.call, byte_now);
        byte_now = *((size_t*)(mach + byte_now + 1)) - 1;
    })

DEV_CMD ("ret", 23,
    {
        byte_now = StackPop(&pr.call);
        byte_now += sizeof (size_t);
    })

DEV_CMD_ARG ("scr", 24,
    {
        pr.vr.VRAM_on = 1;
        pr.vr.place_in_RAM = *((double*)(mach + byte_now + 1));
        byte_now += sizeof (double);
        pr.vr.size_x = *((double*)(mach + byte_now + 1));
        byte_now += sizeof (double);
        pr.vr.size_y = *((double*)(mach + byte_now + 1));
        byte_now += sizeof (double);
    })

DEV_CMD ("upd", 25,
    {
        if (pr.vr.VRAM_on)
            UpdateScreen (&pr);
    })

DEV_CMD ("rnd", 26,
    {
        DO_PUSH (round (DO_POP));
        ass
    })

DEV_CMD ("pow", 27,
    {
        double pw = DO_POP;
        DO_PUSH (pow (DO_POP, pw));
        ass
    })