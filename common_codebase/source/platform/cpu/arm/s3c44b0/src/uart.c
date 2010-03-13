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
#include <inc/utl_string.h>

#include <inc/reg.h>
#include <inc/uart.h>


/* if the uart is initialized, this flag is set to TRUE */
static GT_BOOL uart_init_flag;


/******************************************************************************
 * Uart initial, no FIFO mode.
******************************************************************************/
static void uart_Init(IN const GT_UI32 baudrate)
{
    volatile GT_UI8 i;

    rPCONC  |= 0x0f000000;
    rPUPC   |= 0x3000;
    rUFCON0  = 0x0;
    rUMCON0  = 0x0;
    rULCON0  = 0x3; /* normal mode, no parity, one stop bit, 8-bits */
    rUCON0   = 0x205;
    rUBRDIV0 = ((GT_UI32)((PLLCLK*10/16/baudrate + 5)/10) - 1);

    /* delay a short while, to wait for uart baudrate to be stable */
    for (i = 0; i < 100; i++)
    {}

    uart_init_flag = GT_TRUE;

    return;
}


/******************************************************************************
 * Put char to uart.
******************************************************************************/
static GT_BOOL uart_PutChar(IN const GT_UI8 data)
{
    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return GT_FALSE;
    }

    /* if send buffer not empty, send char fail */
    if (!(rUTRSTAT0 & 0x2))
    {
    	return GT_FALSE;
    }

    WrUTXH0(data); /* send char */

    return GT_TRUE;
}


/******************************************************************************
 * get char from uart, if no charater received, return -1.
******************************************************************************/
static GT_I32 uart_GetChar(void)
{
    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return -1;
    }

    if ((rUTRSTAT0 & 0x1) != 0)
    {
        return RdURXH0();
    }

    return -1;
}


/******************************************************************************
 * Check the uart Tx buffer is empty or not.
 * Return:
 *      TRUE  : Tx buffer is empty.
 *      FALSE : Tx buffer is not empty.
******************************************************************************/
static GT_BOOL uart_CheckEmpty(void)
{
    return (GT_BOOL)(rUTRSTAT0 & 0x2);
}


/******************************************************************************
FUNCTION NAME:
    Uart_Init
DESCRIPTION:
    Uart initial
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    TRUE  : uart initial success
    FALSE : uart initial fail
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_BOOL Uart_RegisterFunc(IN UTL_VT_T *utl_vt_func)
{
    if (utl_vt_func == NULL)
    {
        return GT_FALSE;
    }

    uart_init_flag = GT_FALSE;

    utl_vt_func->init        = uart_Init;
    utl_vt_func->put_char    = uart_PutChar;
    utl_vt_func->get_char    = uart_GetChar;
    utl_vt_func->check_empty = uart_CheckEmpty;
    utl_vt_func->valid       = GT_TRUE;

    return GT_TRUE;
}

