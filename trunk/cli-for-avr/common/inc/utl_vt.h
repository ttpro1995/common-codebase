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

#ifndef __UTL_VT_H__
#define __UTL_VT_H__

#include <basetype.h>
#include <uart.h>


/* Define key ascii value */
#define KEY_ESC		(27)
#define KEY_ENTER   (13)
#define KEY_SPACE   (32)
#define KEY_BS      (8)
#define KEY_TAB     (9)

#define KEY_CTRL_D  (0x04)
#define KEY_CTRL_L  (0x0C)

#define OTHER_KEY_BASE (200)

/* the keys below are not in the ascii table, so we should map them */
#define KEY_LEFT	(OTHER_KEY_BASE+0)
#define KEY_RIGHT	(OTHER_KEY_BASE+1)
#define KEY_UP		(OTHER_KEY_BASE+2)
#define KEY_DOWN	(OTHER_KEY_BASE+3)
#define KEY_F1		(OTHER_KEY_BASE+4)
#define KEY_F2		(OTHER_KEY_BASE+5)
#define KEY_F3		(OTHER_KEY_BASE+6)
#define KEY_F4		(OTHER_KEY_BASE+7)

#define KEY_NULL     '\0'


#define UTL_VT_GetChar          uartGetByte
#define UTL_VT_PutByte          uartSendByte
#define UTL_VT_PutString        uartSendStr


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
I8_T UTL_VT_ReadKey(void);

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
void UTL_VT_PutAscii(const UI8_T *p_str, UI8_T size);


#endif /* __UTL_VT_H__ */

