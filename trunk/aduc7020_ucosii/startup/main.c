/******************************************************************************
 *
 * (C) Copyright 2007-2010
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *
 * FILE NAME:
 *   main.c
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.6.2        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#include "includes.h"


/* declare stack space.
 *
 * here, use __attribute__ to make sure the stack is been aligned to 8Byte.
 *
 * Add additional 8Byte, to make sure the stacking is FD(full-decrease) mode,
 *  to follow the APCS(ARM Procedure Call Standard).
 */
UINT8  start_stack[BOOT_STACK_SIZE + 8] __attribute__(( aligned(8) ));


static void _pre_init(void)
{
    /* Disable All IRQ & FIQ Interrupts */
    IRQCLR = ~0;
    FIQCLR = ~0;

    /* config P1.2/3/6/7 as GPIO output */
    GP1CON = 0x0;
    GP1DAT = (1 << (2+24)) | (1 << (3+24))| (1 << (6+24))| (1 << (7+24));

#if 1
    /* Timer 2 : disabled at first
     *
     * count down
     * periodic mode
     * binary format
     * prescale: 1
     * source clock: internal 32.768KHz clock
     */
    T2CON = 0x440;

    /* set Timer 2 reload value */
    T2LD = ((INTERNAL_OSC_CLOCK / 1000) * OS_TICK_TIME) / 1;

    /* Clear timer interrupt flag, to prevent generating an un-wanted interrupt */
    T2CLRI = 0;

    /* enable Timer 2 interrupt */
    IRQEN = WAKEUP_TIMER_BIT;
#endif
}


#define STACK_SIZE  64

/* Global Variable */
OS_STK Stack[4][STACK_SIZE];


static void Task(void *i)
{
    UINT8  vPinNo = *(UINT8 *)i;

    while (1)
    {
        /* reverse pin output */
        REVERSE_BIT(GP1DAT, (vPinNo + 16));
        OS_TASK_SleepMs(200 * vPinNo);
    }
}


int main(void)
{
    UINT8  id_table[4] = {2, 3, 6, 7};

    _pre_init();

    /* Init OS */
    OSInit();

    /* create the tasks in uC/OS and assign decreasing priority to them */
    OSTaskCreate(Task, &id_table[0], &Stack[0][STACK_SIZE - 1], 3);
    OSTaskCreate(Task, &id_table[1], &Stack[1][STACK_SIZE - 1], 4);
    OSTaskCreate(Task, &id_table[2], &Stack[2][STACK_SIZE - 1], 5);
    OSTaskCreate(Task, &id_table[3], &Stack[3][STACK_SIZE - 1], 6);

    /* Enable RTOS Timer */
    T2CON |= (1 << 7);

    /* start OS */
    OSStart();

    /* never reach here */
    return 0;
}

