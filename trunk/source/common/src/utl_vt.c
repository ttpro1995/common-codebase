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
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#include <h/basetype.h>
#include <inc/boot_define.h>
#include <inc/utl_vt.h>
#include <inc/uart.h>
#include <inc/utl_tran.h>


static UTL_VT_T  utl_vt_func;


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
GT_BOOL UTL_VT_SetUartMode(IN const UTL_VT_UART_MODE mode)
{
    if (utl_vt_func.valid != GT_TRUE)
    {
        return GT_FALSE;
    }

    if (utl_vt_func.set_mode != NULL)
    {
        return (*utl_vt_func.set_mode)(mode);
    }
    else
    {
	    return GT_FALSE;
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
GT_I32 UTL_VT_GetChar(IN GT_UI32 timeout)
{
	GT_I32	get_char;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return -1;
    }

    if (utl_vt_func.get_char != NULL)
    {
    	while (timeout-- > 0)
    	{
        	if ((get_char = (*utl_vt_func.get_char)()) != -1)
        	{
        		return get_char;
        	}
        }
    }

    return -1;
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
GT_BOOL UTL_VT_PutChar(IN GT_UI8 data)
{
    if (utl_vt_func.valid != GT_TRUE)
    {
        return GT_FALSE;
    }

    if (utl_vt_func.put_char != NULL)
    {
	    return (*utl_vt_func.put_char)(data);
    }
    else
    {
        return GT_FALSE;
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
void UTL_VT_PutString(IN const GT_UI8 *p_str, IN GT_UI32 length)
{
    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    if (p_str == NULL)
    {
        return;
    }

    if (utl_vt_func.put_char != NULL)
    {
        while ((length > 0) && (*p_str != '\0'))
        {
            while ((*utl_vt_func.put_char)(*p_str) != GT_TRUE)
            {}

            length--;
            p_str++;
        }
    }

    return;
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
    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    UTL_VT_EraseScreen(2);
    UTL_VT_SetCursorToXY(0, 0);

    return;
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
void UTL_VT_ClearRxBuffer(void)
{
    /* clear Rx buffer */
    while (UTL_VT_GetChar(UTL_VT_GET_CHAR_TIMEOUT) != -1)
    {}

    return;
}


/******************************************************************************
Function:
    UTL_VT_MoveLeft
Description:
    Move Cursor backward by the specified number of columns without changing lines.
    If the cursor is already in the leftmost column, VT ignores this sequence.
Input:
    columns : control cursor left-move n columns.
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_MoveLeft(IN const GT_UI8 columns)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    if (columns == 0)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)columns, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'D';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    cursorRight
Description:
    Move Cursor forward by the specified number of columns without changing lines.
    If the cursor is already in the rightmost column, VT ignores this sequence.
Input:
    columns : control cursor right-move n columns.
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_MoveRight(IN const GT_UI8 columns)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    if (columns == 0)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)columns, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'C';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/*******************************************************************************
Function:
    UTL_VT_MoveUp
Description:
    Move Cursor up by the specified number of columns without changing lines.
    If the cursor is already in the top column, VT ignores this sequence.
Input:
    columns : control cursor up-move n columns.
Output:
    None
Return:
    None
HISTORY:

*******************************************************************************/
void UTL_VT_MoveUp(IN const GT_UI8 columns)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    if (columns == 0)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)columns, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'A';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_MoveDown
Description:
    Move Cursor up by the specified number of columns without changing lines.
    If the cursor is already in the top column, VT ignores this sequence.
Input:
    columns : control cursor up-move n columns.
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_MoveDown(IN const GT_UI8 columns)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    if (columns == 0)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)columns, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'B';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_EraseLine
Description:
    Erase Text on the VT100 Line.
Input:
    erase_mode =  0 : Erase from the cursor to the end of the line.
                  1 : Erase from the beginning of the screen to the cursor.
                  2 : Clears the line but do not moves the cursor position.
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_EraseLine(IN const GT_UI8 erase_mode)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)erase_mode, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'K';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_EraseScreen
Description:
    Erase Text on the VT100 Line.
Input:
    erase_mode =  0 : Erase from the cursor to the end of the line.
                  1 : Erase from the beginning of the screen to the cursor.
                  2 : Clear screen, but do not moves the cursor position.
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_EraseScreen(IN const GT_UI8 erase_mode)
{
    GT_UI8  escapeStr[8];
    GT_UI8 *tempPtr;
    GT_UI8  ascii[4];
    GT_UI8  loop;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    tempPtr = escapeStr;
    *tempPtr++ = VT_KEY_ESC;
    *tempPtr++ = VT_KEY_BRACKET;
    UTL_TRAN_UI32ToAscii((GT_UI32)erase_mode, 10, ascii);
    for (loop = 0; ascii[loop] != VT_KEY_NULL; loop++)
    {
        *tempPtr++ = ascii[loop];
    }
    *tempPtr++ = 'J';
    *tempPtr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_SetTextAttrib
Description:
    Set Text attributes.
Input:
    attribute = '0'- NORNAL (Attributes off)
                '1'- HIGHT INTENSITY (Bold or increased intensity)
                '4'- Underscore
                '5'- BLINK
                '7'- INVERSE (Negative/reverse image)
                All other parameter values are ignored.
    Here is the table of attributes that are used with the SGR command:
      Parameter Attribute type
            0   Reset
            1   Bold
            2   Low intensity
            4   Underline
            5   Blinking
            6   Blinking
            7   Inverse
            8   Concealed
            10  Graphics off
            12  Graphics on
            21  End bold
            22  End low intensity
            24  End underline
            25  End blinking
            26  End blinking
            27  End inverse
            28  End concealed
            30  Black foreground
            31  Red foreground
            32  Green foreground
            33  Yellow foreground
            34  Blue foreground
            35  Magenta foreground
            36  Cyan foreground
            37  White foreground
            39  Default foreground
            40  Black background
            41  Red background
            42  Green background
            43  Yellow background
            44  Blue background
            45  Magenta background
            46  Cyan background
            47  White background
            49  Default background
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_SetTextAttrib(IN const UTL_VT_TextAttrib attrib)
{
    GT_UI8   escapeStr[6];
    GT_UI8  *temp_ptr;
    GT_UI8   str[5];
    GT_UI8   i;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    temp_ptr = escapeStr;
    *temp_ptr++ = VT_KEY_ESC;
    *temp_ptr++ = VT_KEY_BRACKET;

    UTL_TRAN_UI32ToAscii((GT_UI32)attrib, 10, str);
    for (i = 0; str[i] != VT_KEY_NULL; i++)
    {
        *temp_ptr++ = str[i];
    }
    *temp_ptr++ = 'm';
    *temp_ptr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_SaveCursorPosition
Description:
    Save Cursor Position
Input:
    None
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_SaveCursorPosition(void)
{
    GT_UI8   escapeStr[6];
    GT_UI8  *temp_ptr;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    temp_ptr = escapeStr;
    *temp_ptr++ = VT_KEY_ESC;
    *temp_ptr++ = '7';
    *temp_ptr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_RestoreCursorPosition
Description:
    Restore Cursor Position
Input:
    None
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_RestoreCursorPosition(void)
{
    GT_UI8 escapeStr[6];
    GT_UI8 *temp_ptr;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    temp_ptr    = escapeStr;
    *temp_ptr++ = VT_KEY_ESC;
    *temp_ptr++ = '8';
    *temp_ptr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_CharacterSet
Description:
    Set VT100 character set.
Input:
    set:  'A'- United Kingdom Set
          'B'- ASCII character set.
          '0'- Special Graphics.
          '1'- Alternate Character ROM Standard Character Set
          '2'- Alternate Character ROM Special Graphics
Output:
    None
Return:
    None
Note:
    The United Kingdom and ASCII sets conform to the "ISO international
    register of character sets to be used with escape sequences".
    The other sets are private character sets.
    Special graphics means that the graphic characters for the codes
    1378 to 1768 are replaced with other characters.
    The specified character set will be used until another SCS is received.
HISTORY:

******************************************************************************/
void UTL_VT_CharacterSet(IN const GT_UI8 set)
{
    GT_UI8 escapeStr[6];
    GT_UI8 *temp_ptr;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    temp_ptr    = escapeStr;
    *temp_ptr++ = VT_KEY_ESC;
    *temp_ptr++ = '(';
    *temp_ptr++ = set;
    *temp_ptr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_SetCursorToXY
Description:
    Moves the cursor to the specified position.
    If you do not specify a position, the cursor moves to the home
       position ---- the upper-left corner of the screen (line 0, column 0).
Input:
    column = Y
    row    = X
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
void UTL_VT_SetCursorToXY(IN const GT_UI8 row, IN const GT_UI8 column)
{
    GT_UI8  escapeStr[12];
    GT_UI8 *temp_ptr;
    GT_UI8  str[5];
    GT_UI8  i;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return;
    }

    /* Reserve 12 bytes space for Escape sequence */
    temp_ptr = escapeStr;
    *temp_ptr++ = VT_KEY_ESC;
    *temp_ptr++ = VT_KEY_BRACKET;

    UTL_TRAN_UI32ToAscii(row, 10, str);
    for (i = 0; str[i] != VT_KEY_NULL; i++)
    {
        *temp_ptr++ = str[i];
    }

    *temp_ptr++ = ';';

    UTL_TRAN_UI32ToAscii(column, 10, str);
    for (i = 0; str[i] != VT_KEY_NULL; i++)
        {*temp_ptr++ = str[i];}

    *temp_ptr++ = 'H';
    *temp_ptr   = VT_KEY_NULL;

    UTL_VT_PutString(escapeStr, sizeof(escapeStr));
}


/******************************************************************************
Function:
    UTL_VT_GetCursorPosition
Description:
    Get the current cursor position, stored in row & column.
    If get success, return GT_TRUE, else return GT_FALSE.
    Return format: row = 18(the 18th row) , column = 20(the 20th column).
      Not return ascii.
Input:
    column = Y
    row    = X
Output:
    None
Return:
    None
HISTORY:

******************************************************************************/
GT_BOOL UTL_VT_GetCursorPosition(OUT GT_UI8 *row, OUT GT_UI8 *column)
{
    GT_UI8  escapeStr[5];
    GT_UI8  get_row[10], get_column[10];
    GT_UI8 *p_value;
    GT_I32  get_value;

    if (utl_vt_func.valid != GT_TRUE)
    {
        return GT_FALSE;
    }

    memset(escapeStr, 0x0, sizeof(escapeStr));
    p_value    = escapeStr;
    *p_value++ = VT_KEY_ESC;
    *p_value++ = VT_KEY_BRACKET;
    *p_value++ = '6';
    *p_value++ = 'n';
    *p_value++ = VT_KEY_NULL;

    /* Send get position command */
    UTL_VT_PutString(escapeStr, sizeof(escapeStr));

    /* Get 'ESC' response */
    while (UTL_VT_GetChar(UTL_VT_GET_CHAR_TIMEOUT) != VT_KEY_ESC)
    {}

    /* Get '[' response */
    while (UTL_VT_GetChar(UTL_VT_GET_CHAR_TIMEOUT) != VT_KEY_BRACKET)
    {}

    /* Get row number */
    p_value = get_row;
    while (1)
    {
        while ((get_value = UTL_VT_GetChar(UTL_VT_GET_CHAR_TIMEOUT)) == -1)
        {}

        if ((get_value >= 0x30) && (get_value <= 0x39))
        {
            *p_value++ = (GT_UI8)get_value;
        }
        else if (get_value == ';')
        {
            *p_value = VT_KEY_NULL;
            break;
        }
    }

    /* Get column number */
    p_value = get_column;
    while (1)
    {
        while ((get_value = UTL_VT_GetChar(UTL_VT_GET_CHAR_TIMEOUT)) == -1)
        {}

        if ((get_value >= 0x30) && (get_value <= 0x39))
        {
            *p_value++ = (GT_UI8)get_value;
        }
        else if (get_value == 'R')
        {
            *p_value = VT_KEY_NULL;
            break;
        }
    }

    *row    = (GT_UI8)UTL_TRAN_AsciiToI32((GT_I8 *)get_row, 10);
    *column = (GT_UI8)UTL_TRAN_AsciiToI32((GT_I8 *)get_column, 10);

    return GT_TRUE;
}


/******************************************************************************
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
******************************************************************************/
void UTL_VT_Init(void)
{
    memset(&utl_vt_func, 0x0, sizeof(utl_vt_func));

    if (Uart_RegisterFunc(&utl_vt_func) != GT_TRUE)
    {
        memset(&utl_vt_func, 0x0, sizeof(utl_vt_func));
        return;
    }

    if (utl_vt_func.valid != GT_TRUE)
    {
        memset(&utl_vt_func, 0x0, sizeof(utl_vt_func));
        return;
    }

    /* init baudrate */
    if (utl_vt_func.init != NULL)
    {
        (*utl_vt_func.init)(BOOT_BAUDRATE);
    }

    /* set uart mode */
    UTL_VT_SetUartMode(UTL_VT_UART_DEFAULT_MODE);

    /* clear Rx buffer */
    UTL_VT_ClearRxBuffer();

    /* set screen to normal display */
    UTL_VT_SetTextAttrib(VT_TEXT_NORMAL);

    return;
}

