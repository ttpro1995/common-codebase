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
 *   os_cpu.h
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.6.2        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __OS_CPU_H
#define __OS_CPU_H


#include "arm.h"
#include "typedef.h"


#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif


/* Note: the BOOLEAN must be defined to 8-bit;
 *       else, the set OSRunning=1 in OSStartHighRdy() will be incorrect!!!
 */
typedef UINT8   BOOLEAN;
typedef UINT8   INT8U;                    /* Unsigned  8 bit quantity                           */
typedef SINT8   INT8S;                    /* Signed    8 bit quantity                           */
typedef UINT16  INT16U;                   /* Unsigned 16 bit quantity                           */
typedef SINT16  INT16S;                   /* Signed   16 bit quantity                           */
typedef UINT32  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef SINT32  INT32S;                   /* Signed   32 bit quantity                           */
typedef float   FP32;                     /* Single precision floating point                    */
typedef double  FP64;                     /* Double precision floating point                    */

typedef UINT32  OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef UINT32  OS_CPU_SR;                /* Each stack entry is 32-bit wide                    */



/* stack stuff */
#define OS_STK_GROWTH    1      /* define the stack to grow from high to low */


/* globally enable/disable interrupt */
#define OS_ENTER_CRITICAL()                                             \
    do {                                                                \
        UINT32 val;                                                     \
                                                                        \
        __asm__ __volatile__ (                                          \
            "mrs     %0, cpsr ;"                                        \
            "orr     %0, %0, #(IRQ_BIT) ;"                              \
            "msr     cpsr_c, %0"                                        \
            : "=&r" (val)                                               \
        );                                                              \
    } while (0)
#define OS_EXIT_CRITICAL()                                              \
    do {                                                                \
        UINT32 val;                                                     \
                                                                        \
        __asm__ __volatile__ (                                          \
            "mrs     %0, cpsr ;"                                        \
            "bic     %0, %0, #(IRQ_BIT) ;"                              \
            "msr     cpsr_c, %0"                                        \
            : "=&r" (val)                                               \
        );                                                              \
    } while (0)


/* defined in os_cpu_a.S */
void OS_TASK_SW(void);      /* task switch routine */

/* standard task sleep API, count at millisecond */
#define OS_TASK_SleepMs(vMs)    OSTimeDly(((vMs)+OS_TICK_TIME-1) /OS_TICK_TIME)


#endif /* __OS_CPU_H */

