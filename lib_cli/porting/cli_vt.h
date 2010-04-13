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

#ifndef __CLI_VT_H
#define __CLI_VT_H


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
SINT32 ATOI(IN const UINT8 *str);

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
void CLI_VT_DelayMs(IN UINT32   ms);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_Printf
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
#define CLI_VT_Printf(...)      printf(__VA_ARGS__)

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
void CLI_VT_ClearScreen(void);

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
SINT32 CLI_VT_ReadKey(void);

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
void CLI_VT_SetCursorToXY(IN UINT8 x, IN UINT8 y);

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
void CLI_VT_GetCursorPosition(OUT UINT8 *x, OUT UINT8 *y);

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
void CLI_VT_InverseScreenColor(void);

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
void CLI_VT_Init(void);


#endif /* __CLI_VT_H */

