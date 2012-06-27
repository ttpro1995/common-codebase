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
 *   os_cpu_a.S
 * DESCRIPTION:
 *   *) ref. http://hi.baidu.com/flashmuzi/blog/item/efef8abf8087c40319d81fa3.html
 *   *) add lr register saving, for correctly restore the lr register of each task.
 * HISTORY:
 *   2010.5.31        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#include "arm.h"

#define IRQ_SAVED_CONTEXT_SIZE  (15*4)  /* cpsr/r0-r12/lr, total 15*4 Bytes */


/* interrupt context switch, called from OSIntExit() */
.global OSIntCtxSw
OSIntCtxSw:
    /* the lower priority task's context has been pushed into IRQ's stack,
     *  and, the IRQ stack pointer is saved into SP_SAVE variable;
     * all we need to do, is to save this context from IRQ's stack into
     *  the lower priority task's stack.
     */

    /* restore IRQ stack pointer to bottom */
    ldr     r0, =IRQ_SP_BOTTOM
    ldr     r0, [r0]                /* load sp_irq                       */
    mov     sp, r0

    /* 1. switch back to previous cpu mode with IRQ disabled,
     *     this cpu mode is exactly the lower priority task's cpu mode.
     *
     * note: here, we must disable IRQ, to prevent further IRQ interrupt,
     *       because we are not ready to deal this IRQ interrupt yet.
     */
    mrs     r12, spsr               /* load spsr_irq value               */
    orr     r12, r12, #(IRQ_BIT)    /* disable IRQ interrupt             */
    msr     cpsr, r12               /* switch to previous cpu mode       */

    /* 2. copy context from IRQ's stack into the lower priority task's stack */
    sub     r1, sp, #(IRQ_SAVED_CONTEXT_SIZE)
_OSIntCtxSw_loop:
    ldr     r3, [r0, #-4]!
    str     r3, [sp, #-4]!
    cmp     sp, r1
    bne     _OSIntCtxSw_loop

    /* perform a normal context switch */
    b       OSCtxSw



/* normal context switch, called from OSSched() */
.global OS_TASK_SW
OS_TASK_SW:
    stmfd   sp!, {r0-r12, lr}       /* store r0-r12, and return address  */
    mrs     r5, cpsr
    stmfd   sp!, {r5}               /* save cpsr to stack                */

OSCtxSw:
    /* save lr register, two cases:
     *  1. save lr register in normal context switch;
     *  2. save lr register in interrupt context switch.
     */
    stmfd   sp!, {lr}

    /* save current task stack pointer */
    ldr     r5, =OSTCBCur           /* load the current task's TCB       */
    ldr     r5, [r5]                /* get TCB pointer                   */
    str     sp, [r5]                /* save current task stack pointer   */

    /* OsTCBCur = OsTCBHightRdy */
    ldr     r6, =OSTCBHighRdy       /* load the highest ready task's TCB */
    ldr     r6, [r6]                /* get TCB pointer                   */
    ldr     r5, =OSTCBCur           /* load the current task's TCB       */
    str     r6, [r5]                /* change current TCB to the highest */

    /* OSPrioCur = OSPrioHighRdy */
    ldr     r6, =OSPrioHighRdy      /* load address of highest prio task */
    ldrb    r6, [r6]                /* load value of highest prio task   */
    ldr     r5, =OSPrioCur          /* load the current priority pointer */
    strb    r6, [r5]                /* store value of highest in current */

    /* start real context switch */
    b       OSCtxSw2


/* start the highest ready task, called from OSStart() */
.global OSStartHighRdy
OSStartHighRdy:
    ldr     r5, =OSRunning
    mov     r6, #1
    strb    r6, [r5]                /* set os running flag               */

OSCtxSw2:
    /* load the highest priority task stack pointer */
    ldr     r5, =OSTCBHighRdy       /* point to highest TCB              */
    ldr     r5, [r5]                /* get stack pointer                 */
    ldr     sp, [r5]                /* save highest task stack pointer   */

    ldmfd   sp!, {lr}
    ldmfd   sp!, {r5}
    msr     spsr, r5                /* load initial cpsr value to spsr   */
    ldmfd   sp!, {r0-r12, pc}^      /* start the initial highest task    */
                                    /* note: use ^ to prevent interrupt  */


/* OS Tick ISR */
.global OSTickISR
OSTickISR:
    /* push the lower priority task's context into IRQ stack,
     *  it will be used in OSIntCtxSw(),
     *  to save into the lower priority task's stack.
     *
     * note: the lr register of previous cpu mode is still not saved yet,
     *        we do not saved it here, because in IRQ mode, the lr register
     *        of previous cpu mode and IRQ mode are two different physical
     *        registers, thus, we only need to save it while OSIntCtxSw()
     *        executing. if no need to switch context, the lr register of
     *        previous cpu mode is also no need to save.
     */
    sub     lr, lr, #4
    stmfd   sp!, {r0-r12, lr}
    mrs     r5, spsr
    stmfd   sp!, {r5}

    /* save IRQ stack pointer, for OSIntCtxSw() usage */
    ldr     r0, =IRQ_SP_BOTTOM
    add     r1, sp, #(IRQ_SAVED_CONTEXT_SIZE)
    str     r1, [r0]

    bl      OSIntEnter
    bl      OSTimeTick
    bl      OSIntExit

    /* no higher priority task is scheduled */
    ldmfd   sp!, {r5}
    msr     spsr, r5
    ldmfd   sp!, {r0-r12, pc}^  /* note: use ^ to prevent interrupt */


