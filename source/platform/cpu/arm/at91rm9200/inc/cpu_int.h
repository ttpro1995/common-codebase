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

#ifndef __CPU_INT_H
#define __CPU_INT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <inc/boot_define.h>


typedef void (*CPU_INT_VECTOR_HANDLER)(void);


#define CPU_INT_MAX_AIC_PRIORITY        (8)
#define CPU_INT_LOWEST_AIC_PRIORITY     (0)
#define CPU_INT_HIGHEST_AIC_PRIORITY    (CPU_INT_MAX_AIC_PRIORITY - 1)
#define CPU_INT_CHECK_PRIORITY(prio)     \
    ((((GT_UI32)(prio) >= 0)             \
        && ((GT_UI32)(prio) <= CPU_INT_MAX_AIC_PRIORITY)) ? GT_TRUE : GT_FALSE)


#define CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE    ((GT_UI32)0)
#define CPU_INT_SRC_TYPE_INT_EDGE_TRIGGERED     ((GT_UI32)1)
#define CPU_INT_SRCTYPE_EXT_HIGH_LEVEL          ((GT_UI32)2)
#define CPU_INT_SRCTYPE_EXT_POSITIVE_EDGE       ((GT_UI32)3)


/*
 * *****************************************************************************
 *               Peripheral ID Definitions for AT91RM9200
 *            (according to Table 8 of AT91RM9200 datasheet)
 * *****************************************************************************
 */
typedef enum
{
    CPU_INT_ID_FIQ       =  0, /* Advanced Interrupt Controller (FIQ)  */
    CPU_INT_ID_SYS       =  1, /* System Peripheral                    */
    CPU_INT_ID_PIOA      =  2, /* Parallel IO Controller A             */
    CPU_INT_ID_PIOB      =  3, /* Parallel IO Controller B             */
    CPU_INT_ID_PIOC      =  4, /* Parallel IO Controller C             */
    CPU_INT_ID_PIOD      =  5, /* Parallel IO Controller D             */
    CPU_INT_ID_USART0    =  6, /* USART 0                              */
    CPU_INT_ID_USART1    =  7, /* USART 1                              */
    CPU_INT_ID_USART2    =  8, /* USART 2                              */
    CPU_INT_ID_USART3    =  9, /* USART 3                              */
    CPU_INT_ID_MCI       = 10, /* Multimedia Card Interface            */
    CPU_INT_ID_UDP       = 11, /* USB Device Port                      */
    CPU_INT_ID_TWI       = 12, /* Two-Wire Interface                   */
    CPU_INT_ID_SPI       = 13, /* Serial Peripheral Interface          */
    CPU_INT_ID_SSC0      = 14, /* Serial Synchronous Controller 0      */
    CPU_INT_ID_SSC1      = 15, /* Serial Synchronous Controller 1      */
    CPU_INT_ID_SSC2      = 16, /* Serial Synchronous Controller 2      */
    CPU_INT_ID_TC0       = 17, /* Timer Counter 0                      */
    CPU_INT_ID_TC1       = 18, /* Timer Counter 1                      */
    CPU_INT_ID_TC2       = 19, /* Timer Counter 2                      */
    CPU_INT_ID_TC3       = 20, /* Timer Counter 3                      */
    CPU_INT_ID_TC4       = 21, /* Timer Counter 3                      */
    CPU_INT_ID_TC5       = 22, /* Timer Counter 5                      */
    CPU_INT_ID_UHP       = 23, /* USB Host port                        */
    CPU_INT_ID_EMAC      = 24, /* Ethernet MAC                         */
    CPU_INT_ID_IRQ0      = 25, /* Advanced Interrupt Controller (IRQ0) */
    CPU_INT_ID_IRQ1      = 26, /* Advanced Interrupt Controller (IRQ1) */
    CPU_INT_ID_IRQ2      = 27, /* Advanced Interrupt Controller (IRQ2) */
    CPU_INT_ID_IRQ3      = 28, /* Advanced Interrupt Controller (IRQ3) */
    CPU_INT_ID_IRQ4      = 29, /* Advanced Interrupt Controller (IRQ4) */
    CPU_INT_ID_IRQ5      = 30, /* Advanced Interrupt Controller (IRQ5) */
    CPU_INT_ID_IRQ6      = 31, /* Advanced Interrupt Controller (IRQ6) */

    /* start and end flag */
    CPU_INT_ID_START     = CPU_INT_ID_FIQ,
    CPU_INT_ID_END       = CPU_INT_ID_IRQ6,

    /* unknown id */
    CPU_INT_ID_UNKNOWN

} CPU_INT_ID;



typedef enum
{
    /* unknown source name, not valid for user */
    CPU_INT_SRC_UNKNOWN    = 0,

    /* System Peripherals */
    CPU_INT_SRC_SYS_GLOBAL, /* global system peripherals flag   */
    CPU_INT_SRC_MC,         /* Memory Controller                    */
    CPU_INT_SRC_DBGU_UART,  /* Debug UART                           */
    CPU_INT_SRC_ST_PIT,     /* Periodic Interval Timer              */
    CPU_INT_SRC_ST_RTT,     /* Real-Time Timer                      */
    CPU_INT_SRC_ST_WDT,     /* WatchDog Timer                       */
    CPU_INT_SRC_ST_ALARM,   /* Alarm                                */
    CPU_INT_SRC_RTC,        /* Real-Time Controller                 */
    CPU_INT_SRC_PMC,        /* Power Management Controller          */

    /* User Peripherals */
    CPU_INT_SRC_PIOA,       /* Parallel IO Controller A             */
    CPU_INT_SRC_PIOB,       /* Parallel IO Controller B             */
    CPU_INT_SRC_PIOC,       /* Parallel IO Controller C             */
    CPU_INT_SRC_PIOD,       /* Parallel IO Controller D             */
    CPU_INT_SRC_USART0,     /* USART 0                              */
    CPU_INT_SRC_USART1,     /* USART 1                              */
    CPU_INT_SRC_USART2,     /* USART 2                              */
    CPU_INT_SRC_USART3,     /* USART 3                              */
    CPU_INT_SRC_MCI,        /* Multimedia Card Interface            */
    CPU_INT_SRC_UDP,        /* USB Device Port                      */
    CPU_INT_SRC_TWI,        /* Two-Wire Interface                   */
    CPU_INT_SRC_SPI,        /* Serial Peripheral Interface          */
    CPU_INT_SRC_SSC0,       /* Serial Synchronous Controller 0      */
    CPU_INT_SRC_SSC1,       /* Serial Synchronous Controller 1      */
    CPU_INT_SRC_SSC2,       /* Serial Synchronous Controller 2      */
    CPU_INT_SRC_TC0,        /* Timer Counter 0                      */
    CPU_INT_SRC_TC1,        /* Timer Counter 1                      */
    CPU_INT_SRC_TC2,        /* Timer Counter 2                      */
    CPU_INT_SRC_TC3,        /* Timer Counter 3                      */
    CPU_INT_SRC_TC4,        /* Timer Counter 3                      */
    CPU_INT_SRC_TC5,        /* Timer Counter 5                      */
    CPU_INT_SRC_UHP,        /* USB Host port                        */
    CPU_INT_SRC_EMAC,       /* Ethernet MAC                         */

    /* start and end flag, not valid for user */
    CPU_INT_SRC_START    = CPU_INT_SRC_SYS_GLOBAL,
    CPU_INT_SRC_END      = CPU_INT_SRC_EMAC,

} CPU_INT_SRC_NAME;

#define CPU_INT_CHECK_SRC_NAME(src_name)        \
    (((src_name >= CPU_INT_SRC_START)           \
        && (src_name <= CPU_INT_SRC_END)) ? GT_TRUE : GT_FALSE)


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_HookInterrupt
 * DESCRIPTION:
 *      Hook ISR into AIC.
 * INPUT:
 *      src_name  : interrupt source peripherals name;
 *      prio      : interrupt priority;
 *      src_type  : interrupt trigger source mode;
 *      handler   : interrupt handler.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : hook success;
 *      GT_FALSE : hook fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_HookInterrupt
(
    IN const CPU_INT_SRC_NAME   src_name,
    IN const GT_UI32            prio,
    IN const GT_UI32            src_type,
    IN CPU_INT_VECTOR_HANDLER   handler
);

/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_SetInterrupt
 * DESCRIPTION:
 *      Enable/disable corresponding interrupt.
 * INPUT:
 *      int_num   : interrupt number;
 *      enable    : =TRUE: enable interrupt; =FALSE: disable interrupt.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : set interrupt success;
 *      GT_FALSE : set interrupt fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_SetInterrupt
(
    IN const CPU_INT_SRC_NAME   src_name,
    IN const GT_BOOL            enable
);

/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_SetGlobalState
 * DESCRIPTION:
 *      Enable/Disable CPU global interrupt.
 * INPUT:
 *      enable : =GT_TRUE, enable;
 *               =GT_FALSE, disable.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : success;
 *      GT_FALSE : fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_SetGlobalState(IN const GT_BOOL enable);

/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_Init
 * DESCRIPTION:
 *      Init AIC for interrupt.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : init success;
 *      GT_FALSE : init fail.
 * NOTES:
 *      Before we do this, the vector address in 0x18-0x1C must be:
 *      ldr pc, [pc, #-0xF20]  -- that is 0xE51FFF20.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __CPU_INT_H */

