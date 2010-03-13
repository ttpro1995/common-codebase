/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      HW_LCD.h
 * VERSION:
 *      1.00
 * DESCRIPTION:
 *      This package provides the driver of OCMJ5*10B LCD
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/

#ifndef __HW_LCD_H
#define __HW_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <h/bit.h>

#ifdef __CPU_AVR__
#include <avr/io.h>
#endif

/********** Hardware define ***************************************************/
#define LCD_DATA_PORT       (PORTB)
#define LCD_DATA_DDR        (DDRB)
#define LCD_CTRL_PORT       (PORTA)
#define LCD_CTRL_PIN        (PINA)
#define LCD_CTRL_DDR        (DDRA)

#define LCD_REQ_BIT         (5)
#define LCD_BUSY_BIT        (6)
#define LCD_RESET_BIT       (7)

/* the lcd's display size */
#define LCD_MAX_ROW         (10)
#define LCD_MAX_COLUMN      (20)

#define TAB_SIZE            (4)

/* REQ bit control */
#define LCD_SET_REQ()       SET_BIT(LCD_CTRL_PORT, LCD_REQ_BIT)
#define LCD_CLR_REQ()       CLR_BIT(LCD_CTRL_PORT, LCD_REQ_BIT)

/* Reset bit control */
#define LCD_RESET()         CLR_BIT(LCD_CTRL_PORT, LCD_RESET_BIT)
#define LCD_RELEASE_RESET() SET_BIT(LCD_CTRL_PORT, LCD_RESET_BIT)

/* check that the lcd is busy or not */
#define LCD_READ_BUSY()     TEST_BIT(LCD_CTRL_PIN, LCD_BUSY_BIT)

#define LCD_SEND_DATA(data) LCD_DATA_PORT = (GT_UI8)(data)



/******************************************************************************/
/*                         LCD Control API                                    */
/******************************************************************************/


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveCursorUp
 * DESCRIPTION:
 *      move cursor up n rows
 * INPUT:
 *      n : the rows that the cursor move up
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveCursorUp(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveCursorDown
 * DESCRIPTION:
 *      move cursor down n rows
 * INPUT:
 *      n : the rows that the cursor move down
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveCursorDown(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveCursorLeft
 * DESCRIPTION:
 *      move cursor left n columns
 * INPUT:
 *      n : the columns that the cursor move left
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveCursorLeft(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveCursoRight
 * DESCRIPTION:
 *      move cursor right n columns
 * INPUT:
 *      n : the columns that the cursor move right
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveCursorRight(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveScreenUp
 * DESCRIPTION:
 *      move screen up n dots
 * INPUT:
 *      n : the dots that the screen move up
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      The cursor's position will not change
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveScreenUp(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveScreenDown
 * DESCRIPTION:
 *      move screen down n dots
 * INPUT:
 *      n : the dots that the screen move down
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      The cursor's position will not change
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveScreenDown(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveScreenLeft
 * DESCRIPTION:
 *      move screen left n dots
 * INPUT:
 *      n : the dots that the screen move left
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      The cursor's position will not change
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveScreenLeft(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_MoveScreenRight
 * DESCRIPTION:
 *      move screen right n dots
 * INPUT:
 *      n : the dots that the screen move right
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      The cursor's position will not change
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_MoveScreenRight(IN const GT_UI8 n);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_ClrScreen
 * DESCRIPTION:
 *      Clear screen OCMJ5*10B LCD
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_ClrScreen(void);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_EraseLine
 * DESCRIPTION:
 *      Erase line of OCMJ5*10B LCD
 * INPUT:
 *      erase_mode =  0 : Erase from the cursor to the end of the line.
 *                    1 : Erase from the beginning of the line to the cursor.
 *                    2 : Clear the line but do not moves the cursor position.
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_EraseLine(IN const GT_UI8 erase_mode);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_EraseScreen
 * DESCRIPTION:
 *      Erase screen of OCMJ5*10B LCD
 * INPUT:
 *      erase_mode =  0 : Erase from the cursor to the end of the screen.
 *                    1 : Erase from the beginning of the screen to the cursor.
 *                    2 : Clear the screen but do not moves the cursor position.
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_EraseScreen(IN const GT_UI8 erase_mode);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_Init
 * DESCRIPTION:
 *      Initial OCMJ5*10B LCD
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_Printf(IN const GT_UI8 *ptr_in);


/*******************************************************************************
 * FUNCTION NAME:
 *      HW_LCD_Init
 * DESCRIPTION:
 *      Initial OCMJ5*10B LCD
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void HW_LCD_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __HW_LCD_H */

