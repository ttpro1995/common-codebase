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

#include <conio.h>
#include "typedef.h"
#include "cli_vt.h"
#include "vt_def.h"


#if 1

#define ISXDIGIT(x) ((((x)>='0') && ((x)<='9'))         \
                        || (((x)>='a') && ((x)<='f'))   \
                        || (((x)>='A') && ((x)<='F')))
#define TOUPPER(x)  ((((x)>='a') && ((x)<='z')) ? ((char)(((x)-'a')+'A')) : (x))


static const UINT8 TRAN_Htob[16] =
{
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};



static SINT32 hextoi(IN const UINT8 *str)
{
	SINT32 		i, len;
	UINT8		ch;
	UINT32 	    val = 0;

	len = strlen(str);
	for (i=0; i<len; i++)
	{
		val <<= 4;

		ch = str[i];
		if ((ch >='0') && (ch <='9'))
		{
			val |= (ch - '0');
		}
		else
		{
			if ((ch >='a') && (ch <='f'))
			{
				val |= (ch - 'a' + 10);
			}
			else if ((ch >='A') && (ch <='F'))
			{
				val |= ((ch - 'A') + 10);
			}
			else
			{
				break;
			}
		}
	}

	return val;
}

/******************************************************************************
FUNCTION NAME:
    ATOI
DESCRIPTION:
    Translate Ascii to int
INPUT:
    str   : string input
OUTPUT:
    NONE
RETURN:
    The result int 32
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
SINT32 ATOI(IN const UINT8 *str)
{
    SINT32  n = 0;

    if (str == NULL)
    {
        return 0;
    }

    if ((str[0]=='0')
            && ((str[1]=='x') || (str[1]=='X')))
    {
        return hextoi(str + 2);
    }
    else
    {
        while ((*str >= '0') && (*str <= '9'))
        {
            n = (n * 10) + *str - '0';
            str++;
        }

        return n;
    }
}

#endif


#if 1

#define CLI_VT_READKEY_TIMEOUT     (12000)


static SINT32 _getChar(void)
{
    if (kbhit())
    {
        return (SINT32)getch();
    }
    else
    {
        return -1;
    }
}


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
static SINT32 cli_vt_ReadKey_MSDOS(void)
{
    SINT32  i, key, key1;

	for (i=0; i<CLI_VT_READKEY_TIMEOUT; i++)
	{
		key = _getChar();
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
		key1 = _getChar();
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


#if 1

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
SINT32 CLI_VT_ReadKey(void)
{
    return cli_vt_ReadKey_MSDOS();
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
    system("cls");
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_SetCursorToXY
 * DESCRIPTION:
 *      Moves the cursor to the specified position.
 * PARAMETERS:
 *      x  : Column.
 *      y  : Row.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.4.15        Panda.Xiong         Create/Update
 *****************************************************************************/
void CLI_VT_SetCursorToXY(IN UINT8 x, IN UINT8 y)
{
    COORD c = {x, y};

    while (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c))
    {}
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_GetCursorPosition
 * DESCRIPTION:
 *      Get current cursor position.
 * PARAMETERS:
 *      x  : Column.
 *      y  : Row.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.4.15        Panda.Xiong         Create/Update
 *****************************************************************************/
void CLI_VT_GetCursorPosition(OUT UINT8 *x, OUT UINT8 *y)
{
    CONSOLE_SCREEN_BUFFER_INFO scrInfo;

    while (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scrInfo))
    {}

    *x = scrInfo.dwCursorPosition.X;
    *y = scrInfo.dwCursorPosition.Y;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_InverseScreenColor
 * DESCRIPTION:
 *      Inverse Screen Color.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.6.6        PANDA         Create/Update
 *****************************************************************************/
void CLI_VT_InverseScreenColor(void)
{
    static BOOL vState = TRUE;

#define WHITE_ON_BLACK  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define BLACK_ON_WHITE  (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)

    vState = !vState;

    if (vState)
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_ON_BLACK);
    }
    else
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLACK_ON_WHITE);
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_DelayMs
 * DESCRIPTION:
 *      Delay ms.
 * PARAMETERS:
 *      ms : Millisecond to delay.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.6.7        PANDA         Create/Update
 *****************************************************************************/
void CLI_VT_DelayMs(IN UINT32   ms)
{
    /* the Sleep() API only can be minimum to delay 30ms */
    Sleep((ms + 30 -1) / 30);
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_Init
 * DESCRIPTION:
 *      CLI VT Init.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.6.6        PANDA         Create/Update
 *****************************************************************************/
void CLI_VT_Init(void)
{
    COORD       buffersize = {80, 9999};  /* reset buffer size */
    SMALL_RECT  rc = {0, 0, 79, 35};      /* reset window size */

    /* set environment for CLI VT */
    SetConsoleTitle("CLI_VT");
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), buffersize);
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rc);
}

#endif

