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
 *    ARM Specific code, only for AT91RM9200.
 *
 * File Name:
 *    os_cpu_a.S
 * History:
 *    2007.11.21  Panda Xiong (yaxi1984@gmail.com)
*******************************************************************************/

#include <inc/boot_define.h>
#include <inc/reg.h>


.global OSTCBCur
.global OSTCBHighRdy
.global OSPrioCur
.global OSPrioHighRdy
.global OSRunning
.global OSIntNesting
.global OSTimeTick
.global OSIntEnter
.global OSIntExit

.global OSTickISR
.global ARMDisableInt
.global ARMEnableInt
.global OSIntCtxSw
.global OS_TASK_SW
.global OSStartHighRdy


.code 32

/*******************************************************************************
 * Disable Interrupt
 ******************************************************************************/
ARMDisableInt:
    mrs     r12, cpsr                 /* get current CPU mode               */
    orr     r12, r12, #(NO_INT)       /* set the interrupt disable mode bit */
    msr     cpsr_c, r12
    bx      lr

/*******************************************************************************
 * Enable Interrupt
 ******************************************************************************/
ARMEnableInt:
    mrs     r12, cpsr                 /* get current CPU mode               */
    bic     r12, r12, #(NO_INT)       /* clear the interrupt disable bit    */
    msr     cpsr_c, r12
    bx      lr

/*******************************************************************************
 * OS interrupt task switch routine
 *
 * Interrupt Exit if higher priority task ready to run
 * New Task Context switch
 ******************************************************************************/
OSIntCtxSw:
    add     sp, sp, #24                  /* adjust sp to compensate for the call to OSIntExit */
    msr     cpsr_c, #(IRQ_MODE | NO_INT) /* switch to IRQ mode and disable INT's interrupt    */
                                         /* the following 2 lines keep the stack              */
    ldmfd   sp!, {r12, lr}               /* restore the stack                                 */
    ldmfd   sp!, {r12}

    ldr     r12, =AIC_EOICR
    str     r12, [r12]                   /* write any value to signal end of interrupt        */

    ldr     r12, =(SYS_MODE)
    msr     cpsr_c, r12                  /* switch back to system mode with INTs enabled      */

    b   OSCtxSw                          /* perform the context switch                        */


/*******************************************************************************
 * task switch routine
 *
 * Call from OSSched()
 * New Task Context switch
 ******************************************************************************/
OS_TASK_SW:
    stmfd   sp!, {r0-r12, lr}       /* store current context                */
    b   OSCtxSw                     /* perform the context switch           */

/******************************************************************************/
OSCtxSw:
    mrs     r4, cpsr                /* load current psr to r4               */
    stmfd   sp!, {r4}               /* save current PSR to stack            */
    mrs     r4, spsr                /* load saved psr to r4                 */
    stmfd   sp!, {r4}               /* saved saved psr to stack             */

    /* OSPrioCur = OSPrioHighRdy */
    ldr     r4, =OSPrioCur          /* load the current priority pointer    */
    ldr     r5, =OSPrioHighRdy      /* load address of highest prio task    */
    ldrb    r6, [r5]                /* load value of highest prio task      */
    strb    r6, [r4]                /* store value of highest in current    */

    /* Get current task TCB address */
    ldr     r4, =OSTCBCur           /* load pointer                         */
    ldr     r5, [r4]                /* load current tcb sp                  */
    str     sp, [r5]                /* store sp in preempted tasks's TCB    */

    /* Get highest priority task TCB address */
    ldr     r6, =OSTCBHighRdy
    ldr     r6, [r6]                /* get highrdy's sp     */
    ldr     sp, [r6]                /* set sp = highrdy[sp] */

    /* OSTCBCur = OSTCBHighRdy */
    str     r6, [r4]                /* set new current task TCB address     */
    ldmfd   sp!, {r4}               /* load saved SPSR from stack           */
    msr     spsr_c, r4              /* spsr = saved spsr                    */
    ldmfd   sp!, {r4}               /* load saved cpsr from stack           */
    msr     cpsr_c, r4              /* cpsr = saved cpsr                    */
    ldmfd   sp!, {r0-r12, lr}       /* restore r0-r12 and lr interworking   */
    bx      lr                      /* interworking branch on lr            */

/******************************************************************************/
OSStartHighRdy:
    /* Set OSRunning == TRUE */
    ldr     r4, =OSRunning          /* point r4 to OSRunning                */
    mov     r5, #1                  /* move 1 into r5                       */
    str     r5, [r4]                /* store r5 in [r4]                     */

    /* Get current task TCB address */
    ldr     r4, =OSTCBCur           /* point to current tcb                 */

    /* Get highest priority task TCB address */
    ldr     r5, =OSTCBHighRdy       /* point to highest tcb                 */
    ldr     r5, [r5]                /* get stack pointer                    */
    ldr     sp, [r5]                /* sp = highrdy[sp]                     */
    str     r5, [r4]                /* set current tcb = highrdy tcb        */

    ldmfd   sp!, {r4}               /* get spsr                             */
    ldmfd   sp!, {r4}               /* get cpsr new state from stack top    */
    msr     cpsr_c, r4              /* set cpsr = saved CPSR                */
    ldmfd   sp!, {r0-r12, lr}       /* restore registers r0-r12 and lr      */
    bx      lr                      /* branch on new task                   */

/******************************************************************************/
OSTickISR:
    /* Adjust and save LR_irq in IRQ stack */
    sub     lr, lr, #4
    stmfd   sp!, {lr}

    /* Save SPSR and r0 in IRQ stack */
    mrs     lr, spsr
    stmfd   sp!, {r12, lr}

    /* Write in the IVR to support Protect Mode
     * No effect in Normal Mode
     * De-assert the NIRQ and clear the source in Protect Mode
     */
    ldr     lr, =AIC_BASE_REG
    str     lr, [lr, #(AIC_IVR - AIC_BASE_REG)]

    /* read the interrupt status reg to clear it */
    ldr     r12, =TC_SR(TC0_BASE_REG) /* load tc0 status register address */
    ldr     r12, [r12]                /* read from status register        */

    /* Enable Interrupt and Switch in SYS Mode */
    ldr     r12, =(SYS_MODE)
    msr     cpsr_c, r12

    /* Save scratch/used registers and LR in SYS Stack */
    stmfd   sp!, {r0-r12, lr}

    ldr     r12, =OSIntEnter
    mov     lr, pc
    bx      r12                     /* Branch to OsIntEnter */
    ldr     r12, =OSTimeTick
    mov     lr, pc
    bx      r12                     /* Branch to OsTimeTick */
    ldr     r12, =OSIntExit
    mov     lr, pc
    bx      r12                     /* Branch to OSIntExit  */

    /* Interrupt Exit if no higher priority task ready to run.
     * restore interrupted task
     * Restore scratch/used registers and LR from System Stack
     */
    ldmfd   sp!, {r0-r12, lr}

    /* Disable Interrupt and switch back in IRQ mode */
    msr     cpsr_c, #(IRQ_MODE | NO_INT)

    /* Mark the End of Interrupt on the AIC */
    ldr     r12, =AIC_EOICR
    str     r12, [r12]

    /* Restore SPSR_irq and r0 from IRQ stack */
    ldmfd   sp!, {r12, lr}
    msr     spsr_c, lr

    /* Restore adjusted  LR_irq from IRQ stack directly in the PC */
    ldmfd   sp!, {pc}^

