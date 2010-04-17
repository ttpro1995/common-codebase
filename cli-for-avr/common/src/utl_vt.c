/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    None
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#include <basetype.h>
#include <utl_vt.h>
#include <utl_string.h>

#define CLEAR_SCREEN_LEN     (9)

const UI8_T ansiclr[CLEAR_SCREEN_LEN] IN_ROM =
    {0x1b, 0x5b, 0x3b, 0x48, 0x1b, 0x5b, 0x32, 0x4a, 0};

/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
I8_T UTL_VT_ReadKey(void)
{
    static I8_T  lastkeys[2] = {-1, -1};
    I8_T    key, key1;
    UI16_T  i,   comb_key_timeout;

    comb_key_timeout = 12000;

    for (i=0; i<2; i++)
    {
        if (lastkeys[i] != -1)
        {
            key = lastkeys[i];
            lastkeys[i] = -1;
            return key;
        }
    }

    while ((key = UTL_VT_GetChar()) == -1)
    {}

    if (key != KEY_ESC)
    {
        return key;
    }

    /* wait '[' or 'O' */
    for (i=0; i<comb_key_timeout; i++)
    {
        key = UTL_VT_GetChar();
        if (key != -1)
            break;
    }

    if (key == -1)
        return KEY_ESC;

    switch (key)
    {
        case '[':
        case 'O':
            key1 = key;
            break;
        default:
            lastkeys[0] = key;
            return KEY_ESC;
    }

    /* wait 'A/B/C/D' or 'P/Q/R/S' */
    for (i=0; i<comb_key_timeout; i++)
    {
        key = UTL_VT_GetChar();
        if (key != -1)
            break;
    }

    if (key == -1)
        return KEY_ESC;

    switch (key1)
    {
        case '[':
            switch (key)
            {
                case 'A':
                    key = KEY_UP;
                    break;
                case 'B':
                    key = KEY_DOWN;
                    break;
                case 'C':
                    key = KEY_RIGHT;
                    break;
                case 'D':
                    key = KEY_LEFT;
                    break;
                default:
                    lastkeys[0] = '[';
                    lastkeys[1] = key;
                    key = KEY_ESC;
                    break;
            }
            break;

        case 'O':
            switch (key)
            {
                case 'P':
                    key = KEY_F1;
                    break;
                case 'Q':
                    key = KEY_F2;
                    break;
                case 'R':
                    key = KEY_F3;
                    break;
                case 'S':
                    key = KEY_F4;
                    break;
                default:
                    lastkeys[0] = 'O';
                    lastkeys[1] = key;
                    key = KEY_ESC;
                    break;
            }
            break;

        default:
            break;
    }

    return key;
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
void UTL_VT_PutAscii(const UI8_T *p_str, UI8_T size)
{
    if (POINTER_CHECK(p_str))
    {
        return;
    }

    while (size-- != 0)
    {
        UTL_VT_PutByte(*p_str++);
    }
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
void UTL_VT_ClearScreen(void)
{

#ifdef __CPU_AVR__

    UI8_T  tmp[CLEAR_SCREEN_LEN];

    MEMSET(tmp, 0x0, sizeof(tmp));
    P_MEMCPY(tmp, (ROM_VOID_T)ansiclr, CLEAR_SCREEN_LEN);
    UTL_VT_PutString(tmp);

#else

    UTL_VT_PutString(ansiclr);

#endif

}

