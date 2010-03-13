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
#include <inc/reg.h>
#include <inc/cpu_int.h>
#include <inc/cpu_io.h>
#include <inc/cpu_watchdog.h>


static GT_BOOL cpu_InitClock(void)
{
    /* enable all peripherals clock */
    (VP32(PMC_PCER)) = ~0;

    return GT_TRUE;
}


static GT_BOOL cpu_InitST(void)
{
    /* Periodic Interval Timer, timeout : ~0.01s */
    (VP32(ST_PIMR)) = 0xCCD;

    /* Real-Time Timer, timeout : 0.25s */
    (VP32(ST_RTMR)) = 0x2000;

    /* enable peridical timer and Real-Time-Timer interrupt */
    (VP32(ST_IER))  = (1<<0) | (1<<2);

    return GT_TRUE;
}


static void cpu_int_ST_RTT_Handler(void)
{
    static GT_UI32  dbg_led_val = 0;

    dbg_led_val = (dbg_led_val == 0)? 1 : 0;
    CPU_IO_WRITE_DEBUG_LED(dbg_led_val);

    /* kick watchdog */
    CPU_WatchDog_Kick();

    return;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_Init
 * DESCRIPTION:
 *      Init CPU.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : init success;
 *      GT_FALSE : init fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_Init(void)
{
    /* init clock */
    cpu_InitClock();

    /* init interrupt */
    CPU_INT_Init();

    /* init System Timer */
    cpu_InitST();

    /* hook real-time Timer vector handler, and enable it */
    if (!CPU_INT_HookInterrupt(CPU_INT_SRC_ST_RTT,
                               CPU_INT_HIGHEST_AIC_PRIORITY,
                               CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                               cpu_int_ST_RTT_Handler)
        || !CPU_INT_SetInterrupt(CPU_INT_SRC_ST_RTT, GT_TRUE))
    {
        return GT_FALSE;
    }

    /* enable watchdog */
    CPU_WatchDog_Set(GT_TRUE);

    /* enable CPU global interrupt */
    CPU_INT_SetGlobalState(GT_TRUE);

    return GT_TRUE;
}

