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

#include <inc/reg.h>
#include <inc/uart.h>
#include <inc/cpu_int.h>
#include <inc/drv_sio.h>


/* if the uart is initialized, this flag is set to GT_TRUE */
static GT_BOOL uart_init_flag;


/******************************************************************************
 * Put char to uart.
******************************************************************************/
static GT_BOOL uart_PutChar(IN const GT_UI32 data)
{
    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return GT_FALSE;
    }

    /* if send buffer not empty, send char fail */
    if (!((VP32(DBGU_SR)) & (1<<1)))
    {
    	return GT_FALSE;
    }

    /* send char */
    (VP32(DBGU_THR)) = (data & 0xFF);

    return GT_TRUE;
}


/******************************************************************************
 * get char from uart, if no charater received, return -1.
******************************************************************************/
static GT_I32 uart_GetChar(void)
{
    GT_UI32     data;

    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return -1;
    }

    if ((VP32(DBGU_IMR)) == 0x0)
    {
        data = (VP32(DBGU_SR));
    }
    else
    {
        data = (VP32(DBGU_SR)) & (VP32(DBGU_IMR));
    }

    if (data & (1<<0))
    {
        return ((VP32(DBGU_RHR)) & 0xFF);
    }
	else
	{
    	return -1;
    }
}


/******************************************************************************
 * set uart mode.
 * Return:
 *   GT_TRUE  : Set new mode success;
 *   GT_FALSE : Set new mode fail.
******************************************************************************/
static GT_BOOL uart_SetMode(IN const UTL_VT_UART_MODE mode)
{
    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return GT_FALSE;
    }

    if (mode == UTL_VT_UART_POLLING_MODE)
    {
        /* disable DBGU Rx interrupt */
        (VP32(DBGU_IDR)) = ((0x1 << 0) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7));

#if defined(__TARGET_KERNEL__)
    if (!CPU_INT_SetInterrupt(CPU_INT_SRC_DBGU_UART, GT_FALSE))
    {
        return GT_FALSE;
    }
#endif

    }
    else if (mode == UTL_VT_UART_INTERRUPT_MODE)
    {
        /* enable DBGU Rx interrupt */
        (VP32(DBGU_IER)) = ((0x1 << 0) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7));

#if defined(__TARGET_KERNEL__)
    if (!CPU_INT_SetInterrupt(CPU_INT_SRC_DBGU_UART, GT_TRUE))
    {
        return GT_FALSE;
    }
#endif

    }
    else
    {
        return GT_FALSE;
    }

    return GT_TRUE;
}


/******************************************************************************
 * Check the uart Tx buffer is empty or not.
 * Return:
 *      GT_TRUE  : Tx buffer is empty.
 *      GT_FALSE : Tx buffer is not empty.
******************************************************************************/
static GT_BOOL uart_CheckEmpty(void)
{
    /* check this uart is initialized or not */
    if (!uart_init_flag)
    {
        return -1;
    }

    return ((VP32(DBGU_SR)) & (1<<1)) ? GT_TRUE : GT_FALSE;
}


/******************************************************************************
 * Uart initial, no FIFO mode.
******************************************************************************/
static void uart_Init(IN const GT_UI32 baudrate)
{
    (VP32(PIO_ASR(PIOA_BASE_REG))) = (1<<31) | (1<<30);
    (VP32(PIO_BSR(PIOA_BASE_REG))) = 0;
    (VP32(PIO_PDR(PIOA_BASE_REG))) = (1<<31) | (1<<30);

    (VP32(DBGU_CR))    = (1<<2) | (1<<3) | (1<<5) | (1<<7);
    (VP32(DBGU_IDR))   = (~0);
    (VP32(DBGU_MR))    = (0x0<<14) | (0x4<<9);
    (VP32(DBGU_BRGR))  = (CPU_MASTER_CLOCK / (baudrate*16));
    (VP32(DBGU_CR))    = (1<<4) | (1<<6);   /* enable Tx and Rx */

#if defined(__TARGET_KERNEL__)
    if (!CPU_INT_HookInterrupt(CPU_INT_SRC_DBGU_UART,
                               CPU_INT_HIGHEST_AIC_PRIORITY,
                               CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                               DRV_SIO_UpdateBuffer))
    {
        return;
    }
#endif

    uart_init_flag = GT_TRUE;
    return;
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
    GT_TRUE  : uart initial success
    GT_FALSE : uart initial fail
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
GT_BOOL Uart_RegisterFunc(OUT UTL_VT_T *utl_vt_func)
{
    if (utl_vt_func == NULL)
    {
        return GT_FALSE;
    }

    uart_init_flag = GT_FALSE;

    utl_vt_func->init        = uart_Init;
    utl_vt_func->put_char    = uart_PutChar;
    utl_vt_func->get_char    = uart_GetChar;
    utl_vt_func->set_mode    = uart_SetMode;
    utl_vt_func->check_empty = uart_CheckEmpty;
    utl_vt_func->valid       = GT_TRUE;

    return GT_TRUE;
}

