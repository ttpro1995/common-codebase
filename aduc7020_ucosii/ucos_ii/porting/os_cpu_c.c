/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                        (c) Copyright 1992-1998, Jean J. Labrosse, Plantation, FL
*                                          All Rights Reserved
*                        (c) Copyright ARM Limited 1999.  All rights reserved.
*
*                                          ARM Specific code
*
*
* File : OS_CPU_C.C
*
* Modification : 21/08/00  EL
*********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"

OS_STK   IRQ_SP_BOTTOM;

/*
*********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created.  This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack.  It is assumed that 'ptos' points to
*                            a 'free' entry on the task stack.  If OS_STK_GROWTH is set to 1 then
*                            'ptos' will contain the HIGHEST valid address of the stack.  Similarly, if
*                            OS_STK_GROWTH is set to 0, the 'ptos' will contains the LOWEST valid address
*                            of the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              PSW to 0x0002 instead.  In this case, interrupts would be disabled upon task startup.  The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code.  You will need to modify OSTaskIdle() and OSTaskStat() so that they enable
*              interrupts.  Failure to do this will make your system crash!
*********************************************************************************************************
*/

OS_STK *OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk ;

    opt    = opt;             /* 'opt' is not used, prevent warning */
    stk    = ptos;            /* Load stack pointer                 */

    /* build a context for the new task */
    *--stk = (OS_STK)task;              /* pc, task entry, will be directly transferd to PC register */
    *--stk = 0;                         /* r12  */
    *--stk = 0;                         /* r11  */
    *--stk = 0;                         /* r10  */
    *--stk = 0;                         /* r9   */
    *--stk = 0;                         /* r8   */
    *--stk = 0;                         /* r7   */
    *--stk = 0;                         /* r6   */
    *--stk = 0;                         /* r5   */
    *--stk = 0;                         /* r4   */
    *--stk = 0;                         /* r3   */
    *--stk = 0;                         /* r2   */
    *--stk = 0;                         /* r1   */
    *--stk = (OS_STK)pdata;             /* r0, the first parameter, to follow APCS */
    *--stk = SVC_MODE;                  /* cpsr, can set to USR mode, to force all tasks are running at USR mode */
    *--stk = (OS_STK)task;              /* lr, return address of this task, can be initialized to other procedure to handle the stuffs after task exiting */

    return stk;
}


void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

void OSTaskSwHook (void)
{}

void OSTaskStatHook (void)
{}

void OSTimeTickHook (void)
{
    /* write any value to this register, to clear the Timer 2 interrupt */
    T2CLRI = 0;
}

void OSTCBInitHook(OS_TCB *ptcb)
{
    ptcb = ptcb;
}

void OSInitHookBegin(void)
{}

void OSInitHookEnd(void)
{}

void OSTaskIdleHook(void)
{}

