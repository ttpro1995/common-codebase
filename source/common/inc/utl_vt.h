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

#ifndef __UTL_VT_H
#define __UTL_VT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>


/* Define key ascii value */
#define VT_KEY_ESC         (27)
#define VT_KEY_CR          (13)  /* CR */
#define VT_KEY_SPACE       (32)
#define VT_KEY_LF          (10)  /* LF */
#define VT_KEY_TAB         (9)
#define VT_KEY_BS          (8)

/* Super Terminal not support yet,
 * but supported by "Tera Term" and "SecureCRT" */
#define VT_KEY_DEL		   (0x7F)

#define VT_KEY_CTRL_D      (0x04)
#define VT_KEY_CTRL_L      (0x0C)
#define VT_KEY_CTRL_C      (0x03)
#define VT_KEY_CTRL_T      (0x14)
#define VT_KEY_CTRL_R      (0x12)
#define VT_KEY_CTRL_S      (0x13)
#define VT_KEY_CTRL_U      (0x15)
#define VT_KEY_CTRL_Z      (0x1A)

#define VT_OTHER_KEY_BASE  (200)

/* the keys below are not in the ascii table, so we should map them */
#define VT_KEY_LEFT        (VT_OTHER_KEY_BASE + 0)
#define VT_KEY_RIGHT       (VT_OTHER_KEY_BASE + 1)
#define VT_KEY_UP          (VT_OTHER_KEY_BASE + 2)
#define VT_KEY_DOWN        (VT_OTHER_KEY_BASE + 3)
#define VT_KEY_F1          (VT_OTHER_KEY_BASE + 4)
#define VT_KEY_F2          (VT_OTHER_KEY_BASE + 5)
#define VT_KEY_F3          (VT_OTHER_KEY_BASE + 6)
#define VT_KEY_F4          (VT_OTHER_KEY_BASE + 7)
#define VT_KEY_HOME	       (VT_OTHER_KEY_BASE + 8)
#define VT_KEY_END		   (VT_OTHER_KEY_BASE + 9)
#define VT_KEY_INSERT	   (VT_OTHER_KEY_BASE + 10)

#define VT_KEY_BRACKET     '['
#define VT_KEY_NULL        '\0'


/* VT100 Text Attributes */
/* The table of attributes that are used with the SGR command */
typedef enum
{
    VT_TEXT_NORMAL             = 0,      /* NORNAL (Attributes off)          */
    VT_TEXT_HIGH_INTENSITY     = 1,      /* Bold or high intensity           */
    VT_TEXY_LOW_INTENSITY      = 2,      /* Low intensity                    */
    VT_TEXT_UNDERLINE          = 4,      /* Underline                        */
    VT_TEXT_BLINK              = 5,      /* Blinking                         */
    VT_TEXT_INVERSE            = 7,      /* INVERSE (Negative/reverse image) */
    VT_TEXT_CONCEALED          = 8,      /* Concealed                        */
    VT_TEXT_ASCII              = 10,     /* Graphics off                     */
    VT_TEXT_GRAPHICS           = 12,     /* Graphics on                      */

    VT_TEXT_END_HIGH_INTENSITY = 21,     /* End high intensity               */
    VT_TEXT_END_LOW_INTENSITY  = 22,     /* End low intensity                */
    VT_TEXT_END_UNDERLINE      = 24,     /* End underline                    */
    VT_TEXT_END_BLINK          = 25,     /* End blinking                     */
    VT_TEXT_END_INVERSE        = 27,     /* End inverse                      */
    VT_TEXT_END_CONCEALED      = 28,     /* End concealed                    */

    VT_TEXT_BLACK_FOREGROUND   = 30,     /* Black foreground                 */
    VT_TEXT_RED_FOREGROUND     = 31,     /* Red foreground                   */
    VT_TEXT_GREEN_FOREGROUND   = 32,     /* Green foreground                 */
    VT_TEXT_YELLOW_FOREGROUND  = 33,     /* Yellow foreground                */
    VT_TEXT_BLUE_FOREGROUND    = 34,     /* Blue foreground                  */
    VT_TEXT_MAGENTA_FOREGROUND = 35,     /* Magenta foreground               */
    VT_TEXT_CYAN_FOREGROUND    = 36,     /* Cyan foreground                  */
    VT_TEXT_WHITE_FOREGROUND   = 37,     /* White foreground                 */
    VT_TEXT_DEFAULT_FOREGROUND = 39,     /* Default foreground               */

    VT_TEXT_BLACK_BACKGROUND   = 40,     /* Black background                 */
    VT_TEXT_RED_BACKGROUND     = 41,     /* Red background                   */
    VT_TEXT_GREEN_BACKGROUND   = 42,     /* Green background                 */
    VT_TEXT_YELLOW_BACKGROUND  = 43,     /* Yellow background                */
    VT_TEXT_BLUE_BACKGROUND    = 44,     /* Blue background                  */
    VT_TEXT_MAGENTA_BACKGROUND = 45,     /* Magenta background               */
    VT_TEXT_CYAN_BACKGROUND    = 46,     /* Cyan background                  */
    VT_TEXT_WHITE_BACKGROUND   = 47,     /* White background                 */
    VT_TEXT_DEFAULT_BACKGROUND = 49      /* Default background               */
} UTL_VT_TextAttrib;

typedef enum
{
    UTL_VT_UART_UNKNOWN_MODE   = 0,
    UTL_VT_UART_POLLING_MODE   = 1,
    UTL_VT_UART_INTERRUPT_MODE = 2,

#if defined(__TARGET_LOADER__)
    UTL_VT_UART_DEFAULT_MODE   = UTL_VT_UART_POLLING_MODE
#elif defined(__TARGET_KERNEL__)
    UTL_VT_UART_DEFAULT_MODE   = UTL_VT_UART_INTERRUPT_MODE
#else
  #error "Unknown Target"
#endif

} UTL_VT_UART_MODE;

typedef struct
{
    GT_BOOL      valid;

    void         (*init)(IN const GT_UI32 baudrate);
    GT_BOOL      (*put_char)(IN const GT_UI32 data);
    GT_I32       (*get_char)(void);
    GT_BOOL      (*set_mode)(IN const UTL_VT_UART_MODE mode); /* set uart mode */
    GT_BOOL      (*check_empty)(void);
} UTL_VT_T;


#define UTL_VT_GET_CHAR_TIMEOUT     (1000)


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
GT_BOOL UTL_VT_SetUartMode(IN const UTL_VT_UART_MODE mode);

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
GT_I32 UTL_VT_GetChar(IN GT_UI32 timeout);

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
GT_BOOL UTL_VT_PutChar(IN GT_UI8 data);

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
void UTL_VT_PutString(IN const GT_UI8 *p_str, IN GT_UI32 length);

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
void UTL_VT_ClearScreen(void);


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
void UTL_VT_MoveLeft(IN const GT_UI8 columns);


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
void UTL_VT_MoveRight(IN const GT_UI8 columns);


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
void UTL_VT_MoveUp(IN const GT_UI8 columns);


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
void UTL_VT_MoveDown(IN const GT_UI8 columns);


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
void UTL_VT_EraseLine(IN const GT_UI8 erase_mode);


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
void UTL_VT_EraseScreen(IN const GT_UI8 erase_mode);


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
void UTL_VT_SetTextAttrib(IN const UTL_VT_TextAttrib attrib);


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
void UTL_VT_SaveCursorPosition(void);


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
void UTL_VT_RestoreCursorPosition(void);


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
void UTL_VT_CharacterSet(IN const GT_UI8 set);


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
void UTL_VT_SetCursorToXY(IN const GT_UI8 row, IN const GT_UI8 column);


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
GT_BOOL UTL_VT_GetCursorPosition(OUT GT_UI8 *row, OUT GT_UI8 *column);


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
void UTL_VT_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __UTL_VT_H */
