/*******************************************************************************
 *                       uC/OS-II
 *                The Real-Time Kernel
 *
 *     (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
 *                 All Rights Reserved
 *     (c) Copyright ARM Limited 1999.  All rights reserved.
 *
 *
 * Description:
 *    ARM Specific code, only tested on AT91RM9200.
 *
 * File Name:
 *    os_cpu.h
 * History:
 *    2007.11.21  Panda Xiong (yaxi1984@gmail.com)
*******************************************************************************/

#ifndef __OS_CPU_H
#define __OS_CPU_H

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

#include <h/basetype.h>
#include <inc/boot_define.h>

/*******************************************************************************
*                 DATA TYPES
*             (Compiler Specific)
*******************************************************************************/
typedef GT_BOOL  BOOLEAN;
typedef GT_UI8   INT8U;                /* Unsigned  8 bit quantity           */
typedef GT_I8    INT8S;                /* Signed    8 bit quantity           */
typedef GT_UI16  INT16U;               /* Unsigned 16 bit quantity           */
typedef GT_I16   INT16S;               /* Signed   16 bit quantity           */
typedef GT_UI32  INT32U;               /* Unsigned 32 bit quantity           */
typedef GT_I32   INT32S;               /* Signed   32 bit quantity           */

typedef INT32U   OS_STK;               /* Each stack entry is 32-bit wide    */
typedef INT32U   OS_CPU_SR;            /* Define size of CPU status register */

#define BYTE           INT8S           /* Define data types for backward     */
#define UBYTE          INT8U           /* compatibility to uC/OS V1.xx.      */
#define WORD           INT16S          /* Not actually needed for uC/OS-II.  */
#define UWORD          INT16U
#define LONG           INT32S
#define ULONG          INT32U

/*******************************************************************************
*                  OS Critical Method on ARM
*
* Method #1:  Disable/Enable interrupts using simple instructions.
*             After critical section, interrupts will be enabled even if they
*              were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.
*             In other words, if interrupts were disabled before entering the
*              critical section, they will be disabled when leaving the critical
*              section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.
*             Generally speaking you would store the state of the interrupt
*              disable flag in the local variable 'cpu_sr' and then disable
*              interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions
*              that need to disable interrupts.
*             You would restore the interrupt disable state by copying back
*              'cpu_sr' into the CPU's status register.
*
*******************************************************************************/
#define  OS_CRITICAL_METHOD    1

#if (OS_CRITICAL_METHOD == 1)

extern void ARMDisableInt(void);   /* disable global interrupts      */
extern void ARMEnableInt(void);    /* enable global interrupts       */

/* Disable interrupts */
#define  OS_ENTER_CRITICAL()    ARMDisableInt()

/* Enable  interrupts */
#define  OS_EXIT_CRITICAL()     ARMEnableInt()

#elif (OS_CRITICAL_METHOD == 3)
/*
 * critical section macros use "method 3"
 * (save to local var "cpu_sr").
 */

extern int  ARMDisableInt(void);   /* disable global interrupts      */
extern void ARMEnableInt(void);    /* enable global interrupts       */

/* Disable interrupts */
#define  OS_ENTER_CRITICAL()    do { cpu_sr = ARMDisableInt(); } while(0)

/* Enable  interrupts */
#define  OS_EXIT_CRITICAL()     do { if(cpu_sr == 0) ARMEnableInt(); } while(0)

#else
 #error "Unknown OS Critical Method!"
#endif

extern void OS_TASK_SW(void);      /* task switch routine            */
extern void OSIntCtxSw(void);      /* interrupt task switch routine  */
extern void OSTickISR(void);       /* timer interrupt routine        */


/* Stack grows from HIGH to LOW memory on for large mode */
#define OS_STK_GROWTH          1


#endif /* __OS_CPU_H */

