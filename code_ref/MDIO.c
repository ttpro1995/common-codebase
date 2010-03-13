

/******************************************************************************
 * FUNCTION NAME:
 *      DelayUs
 * DESCRIPTION:
 *      Delay Us.
 * PARAMETERS:
 *      vUs : Us to be delayed.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
void DelayUs(U16 vUs)
{
    U32 vClockCount;

    /* calculate the clock count to be delayed */
    vClockCount = (vUs*49) >> (3 - (OSCICN&0x3) + 1);

    while (vClockCount--)
    {
        __no_operation();
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      DelayMs
 * DESCRIPTION:
 *      Delay Ms.
 * PARAMETERS:
 *      vMs : Ms to be delayed.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
void DelayMs(U16 vMs)
{
    U32 vClockCount;

    /* calculate the clock count to be delayed */
    vClockCount = (vMs*24500UL) >> (3 - (OSCICN&0x3));

    while (vClockCount--)
    {
        __no_operation();
    }
}


