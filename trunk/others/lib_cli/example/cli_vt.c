/******************************************************************************
 *
 * (C) Copyright 2007
 *    Panda Xiong, yaxi1984@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * History:
 *    2009.5.13    Panda Xiong       Create
 *
******************************************************************************/

#include <typedef.h>
#include <cli_vt.h>
#include <utl_prt.h>
#include <utl_vt.h>


/* CLI terminal type, only one can be selected  */
#define CLI_VT_TYPE_MSDOS
//#define CLI_VT_TYPE_VT100

#define CLI_VT_READKEY_TIMEOUT     (12000)


#if defined(CLI_VT_TYPE_VT100)
/******************************************************************************
 * FUNCTION NAME:
 *      cli_vt_ReadKey_VT100
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
static GT_S32 cli_vt_ReadKey_VT100(void)
{
    static GT_S32  lastkeys[2] = {-1, -1};
    GT_S32         i, key, key1;

    for (i = 0; i < COUNT_OF(lastkeys); i++)
    {
        if (lastkeys[i] != -1)
        {
            key = lastkeys[i];
            lastkeys[i] = -1;
            return key;
        }
    }

    key = CLI_VT_GetChar();
    if ((key == -1) || (key != VT_KEY_ESC))
    {
        return key;
    }

    /* wait '[' or 'O' */
	for (i=0; i<CLI_VT_READKEY_TIMEOUT; i++)
	{
		key = CLI_VT_GetChar();
		if (key != -1)
		{
			break;
	    }
	}

    if (key == -1)
    {
        return VT_KEY_ESC;
    }

    switch (key)
    {
        case '[':
        case 'O':
            key1 = key;
            break;
        default:
            lastkeys[0] = key;
            return VT_KEY_ESC;
    }

    /* wait 'A/B/C/D' or 'PQRS' */
	for (i=0; i<CLI_VT_READKEY_TIMEOUT; i++)
	{
		key = CLI_VT_GetChar();
		if (key != -1)
		{
			break;
		}
	}

    if (key == -1)
    {
        return VT_KEY_ESC;
    }

    switch (key1)
    {
        case  '[':
            switch (key)
            {
                case 'A':
                    key = VT_KEY_UP;
                    break;
                case 'B':
                    key = VT_KEY_DOWN;
                    break;
                case 'C':
                    key = VT_KEY_RIGHT;
                    break;
                case 'D':
                    key = VT_KEY_LEFT;
                    break;
				case 'H':
					key = VT_KEY_HOME;
					break;
				case 'K':
					key = VT_KEY_END;
					break;
				case '@':
					key = VT_KEY_INSERT;
					break;

                default:
                    lastkeys[0] = '[';
                    lastkeys[1] = key;
                    key = VT_KEY_ESC;
                    break;
            }
            break;

        case  'O':
            switch (key)
            {
                case 'P':
                    key = VT_KEY_F1;
                    break;
                case 'Q':
                    key = VT_KEY_F2;
                    break;
                case 'R':
                    key = VT_KEY_F3;
                    break;
                case 'S':
                    key = VT_KEY_F4;
                    break;

                default:
                    lastkeys[0] = 'O';
                    lastkeys[1] = key;
                    key = VT_KEY_ESC;
                    break;
            }
            break;

        default:
            break;
    }

    return key;
}
#endif

#if defined(CLI_VT_TYPE_MSDOS)
/******************************************************************************
 * FUNCTION NAME:
 *      cli_vt_ReadKey_MSDOS
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
static GT_S32 cli_vt_ReadKey_MSDOS(void)
{
    GT_S32  i, key, key1;

	for (i=0; i<CLI_VT_READKEY_TIMEOUT; i++)
	{
		key = CLI_VT_GetChar();
		if (key != -1)
		{
			break;
	    }
	}

    if (key == -1)
    {
        return -1;
    }
    else if ((key != 0x00) && (key != 0xE0))
    {
        return key;
    }

	for (i=0; i<CLI_VT_READKEY_TIMEOUT; i++)
	{
		key1 = CLI_VT_GetChar();
		if (key1 != -1)
		{
			break;
	    }
	}

    if (key1 == -1)
    {
        return key;
    }

    if (key == 0x00)
    {
        switch (key1)
        {
            case 0x3B:
                key = VT_KEY_F1;
                break;

            case 0x3C:
                key = VT_KEY_F2;
                break;

            case 0x3D:
                key = VT_KEY_F3;
                break;

            case 0x3E:
                key = VT_KEY_F4;
                break;

            default:
                break;
        }
    }
    else if (key == 0xE0)
    {
        switch (key1)
        {
            case 0x52:
                key = VT_KEY_INSERT;
                break;

            case 0x53:
                key = VT_KEY_DEL;
                break;

            case 0x47:
                key = VT_KEY_HOME;
                break;

            case 0x4F:
                key = VT_KEY_END;
                break;

            case 0x48:
                key = VT_KEY_UP;
                break;

            case 0x50:
                key = VT_KEY_DOWN;
                break;

            case 0x4B:
                key = VT_KEY_LEFT;
                break;

            case 0x4D:
                key = VT_KEY_RIGHT;
                break;

            default:
                break;
        }
    }

    return key;
}
#endif


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_ReadKey
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.12        panda.xiong         Create/Update
 *****************************************************************************/
GT_S32 CLI_VT_ReadKey(void)
{
  #if defined(CLI_VT_TYPE_MSDOS)

    return cli_vt_ReadKey_MSDOS();

  #elif defined(CLI_VT_TYPE_VT100)

    return cli_vt_ReadKey_VT100();

  #else
    #error "Unknown CLI Terminal Type!"
  #endif
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_ClearScreen
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
void CLI_VT_ClearScreen(void)
{
    UTL_VT_ClearScreen();
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_GetChar
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_S32 CLI_VT_Printf(IN const GT_S8 *fmt, ...)
{
    GT_U8        temp_buffer[UTL_PRINTF_MAX_BUFFER + 1];
    va_list      ptr;
    GT_S32       length;

    if (fmt == NULL)
    {
        return -1;
    }

    va_start(ptr, fmt);
    length = UTL_PRT_vsPrintf(temp_buffer, fmt, ptr);
    va_end(ptr);

    UTL_VT_PutString(temp_buffer, length);

    return length;
}

