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

.equ    WTCON,      0x01d30000
.equ    INTCON,     0x01e00000
.equ    INTMSK,     0x01e0000c

.equ    LOCKTIME,   0x01d8000c
.equ    PLLCON,     0x01d80000
.equ    CLKCON,     0x01d80004
.equ    BWSCON,     0x01c80000

#include <inc/boot_define.h>

.globl  _start
.globl  _text_base
.globl  _armboot_start
.globl  _bss_start
.globl  _bss_end

/*
 * section define
 * which is linked at first.
 */
.section .init

.code   32
_start:
    b   reset
    ldr pc, _vector_undef   /* undefined vector      */
    ldr pc, _vector_swi     /* swi vector            */
    ldr pc, _vector_pabt    /* prefetch abort vector */
    ldr pc, _vector_dabt    /* data abort vector     */
    b   .                   /* reserved vector       */
    ldr pc, _vector_irq     /* irq vector            */
    ldr pc, _vector_fiq     /* fiq vector            */

    .balignl 16, 0xdeadbeef

_text_base:
    .word   __TEXT_BASE__

_start_stack:
    .word   (start_stack + BOOT_STACK_SIZE - 1)

_armboot_start:
    .word   _start

_bss_start:
    .word   __bss_start

_bss_end:
    .word   __bss_end


/*
 * system start from here
 */
reset:
    /* Enter SVC32 mode, and disable all interrupt */
    msr cpsr_c, #(SVC_MODE|NO_INT)

    /*watch dog disable */
    ldr r0, =WTCON
    ldr r1, =0x0
    str r1, [r0]

    /* all interrupt disable */
    ldr r0, =INTMSK
    ldr r1, =0x07ffffff
    str r1, [r0]

    ldr r0, =INTCON
    ldr r1, =0x05
    str r1, [r0]

    /* set clock */
    ldr r0, =LOCKTIME
    ldr r1, =800    /* count = t_lock * Fin (t_lock=200us, Fin=10MHz) = 2000*/
    str r1, [r0]

    /* PLL setting */
    ldr r0, =PLLCON
    ldr r1, =((M_DIV << 12) + (P_DIV << 4) + S_DIV)
    str r1, [r0]

    /* All unit block CLK enable */
    ldr r0, =CLKCON
    ldr r1, =0x7ff8
    str r1, [r0]

/*
 * lowlevel init
 */
    bl  _lowlevel_init

/*
 * setup stack
 */
    bl _stack_setup

/*
 * copy
 * copy Bootloader to _text_base
 */
copy:
    adr r0, _start          /* r0 <- flash base address         */
    ldr r1, _text_base      /* r1 <- the taget address          */
    cmp r0, r1              /* test if we run from flash or RAM */
    beq clear_bss

    ldr r2, _armboot_start
    ldr r3, _bss_start
    sub r2, r3, r2          /* r2 <- size of bootloader         */
    add r2, r0, r2          /* r2 <- source end address         */

copy_loop:
    ldmia   r0!, {r3-r10}   /* copy from source address [r0]    */
    stmia   r1!, {r3-r10}   /* copy to   target address [r1]    */
    cmp r0, r2              /* until source end addreee [r2]    */
    ble copy_loop


/*
 * clear bss area
 */
clear_bss:
    ldr r0, _bss_start      /* find start of bss segment        */
    add r0, r0, #4          /* start at first byte of bss       */
    ldr r1, _bss_end        /* stop here                        */
    mov r2, #0x0            /* clear                            */

clbss_loop:
    str r2, [r0]            /* clear loop...                    */
    add r0, r0, #4
    cmp r0, r1
    ble clbss_loop


/*
 * Jump to C program
 */
_jump_c:
    ldr     r0, _start_c
    bx      r0

_start_c:
    .word   startC


/******************************************************************************
 * stack setup
 * only setup svc mode stack here
*******************************************************************************/
_stack_setup:
    /*
     * backup return address in svc mode.
     * and, make sure that the fiq mode can't be the last mode to be initialed.
     * or, the ip(R12) value will be incorrect.
     */
    mov ip, lr

    ldr r0, _start_stack

    /* supervisor mode */
    msr cpsr_c, #(SVC_MODE|NO_INT)
    mov sp, r0
    sub r0, r0, #(SVC_STACK_SIZE)

    /* IRQ mode */
    msr cpsr_c, #(IRQ_MODE|NO_INT)
    mov sp, r0
    sub r0, r0, #(IRQ_STACK_SIZE)

    /* FIQ mode */
    msr cpsr_c, #(FIQ_MODE|NO_INT)
    mov sp, r0
    sub r0, r0, #(FIQ_STACK_SIZE)

    /* abort mode */
    msr cpsr_c, #(ABORT_MODE|NO_INT)
    mov sp, r0
    sub r0, r0, #(ABORT_STACK_SIZE)

    /* undefined instruction mode */
    msr cpsr_c, #(UDF_MODE|NO_INT)
    mov sp, r0
    sub r0, r0, #(UDF_STACK_SIZE)

    /* system mode */
    msr cpsr_c, #(SWI_MODE|NO_INT)
    mov sp, r0

    /* return to supervisor mode, to continue the rest init */
    msr cpsr_c, #(SVC_MODE|NO_INT)

    /* return */
    mov pc, ip


/******************************************************************************
 * exception handlers
******************************************************************************/
/* undefined exception */
_vector_undef:
    stmfd sp!, {r0-r12,lr}
    bl vector_undef
    ldmfd sp!, {r0-r12,pc}^

/* swi exception */
_vector_swi:
    stmfd sp!, {r0-r12,lr}
    bl vector_swi
    ldmfd sp!, {r0-r12,pc}^

/* prefetch abort exception */
_vector_pabt:
    subs lr, lr, #4
    stmfd sp!, {r0-r12,lr}
    bl vector_pabt
    ldmfd sp!, {r0-r12,pc}^

/* data abort exception */
_vector_dabt:
    subs lr, lr, #8
    stmfd sp!, {r0-r12,lr}
    bl vector_dabt
    ldmfd sp!, {r0-r12,pc}^

/* irq exception */
_vector_irq:
    subs lr, lr, #4
    stmfd sp!, {r0-r12,lr}
    bl vector_irq
    ldmfd sp!, {r0-r12,pc}^

/* fiq exception */
_vector_fiq:
    subs lr, lr, #4
    stmfd sp!, {r0-r12,lr}
    bl vector_fiq
    ldmfd sp!, {r0-r12,pc}^

