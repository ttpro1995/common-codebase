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
 *   startup.S
 * DESCRIPTION:
 *   System start entry for ARM7TDMI CPU.
 * HISTORY:
 *   2010.6.2        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#include "arm.h"
#include "boot_define.h"


.global  __start

.text
.code 32
__start:

    /* note: the instruction "ldr  pc, __vector_reset" can not be used here.
     *       if do, and the .text area real run address is not the
     *       cpu boot address, this instruction will load an incorrect
     *       instruction from .text run address(mostly is RAM),
     *       this will make the cpu crash.
     */
    b       _handler_reset      /* reset vector          */

    ldr     pc, _vector_undef   /* undefined vector      */
    ldr     pc, _vector_swi     /* swi vector            */
    ldr     pc, _vector_pabt    /* prefetch abort vector */
    ldr     pc, _vector_dabt    /* data abort vector     */
    b       .                   /* reserved vector       */
    ldr     pc, _vector_irq     /* irq vector            */
    ldr     pc, _vector_fiq     /* fiq vector            */


/* these are used to make sure the cpu can get the
 * corresponding vector handler from the real .text address.
 */
_vector_undef:   .word __start
_vector_swi:     .word __start
_vector_pabt:    .word __start
_vector_dabt:    .word __start
_vector_irq:     .word OSTickISR
_vector_fiq:     .word __start


/* stack definition */
_start_stack:    .word   (start_stack + BOOT_STACK_SIZE)


/* define the .text/.data/.bss section base & end address */
_armboot_start:  .word   __start        /* CPU actural boot address   */
_text_start:     .word   __text_start   /* .text section base address */
_text_end:       .word   __text_end     /* .text section end address  */
_data_start:     .word   __data_start   /* .data section base address */
_data_end:       .word   __data_end     /* .data section end address  */
_bss_start:      .word   __bss_start    /* .bss section base address  */
_bss_end:        .word   __bss_end      /* .bss section end address   */



/******************************************************************************
 * system start from here
 *****************************************************************************/
_handler_reset:

/*
 * Enter SVC32 mode, and disable all interrupt
 */
    msr     cpsr_c, #(SVC_MODE|NO_INT)


/*
 * setup stack
 */
    ldr     r0, _start_stack

    /* PABT/DABT/UDF share the same stack */
    msr     cpsr_c, #(ABORT_MODE|NO_INT)    /* abort mode */
    bic     r0, r0, #7
    mov     sp, r0
    msr     cpsr_c, #(UDF_MODE|NO_INT)      /* undefined instruction mode */
    mov     sp, r0
    sub     r0, r0, #(EXCEPTION_STACK_SIZE)

    /* FIQ mode */
    msr     cpsr_c, #(FIQ_MODE|NO_INT)
    bic     r0, r0, #7
    mov     sp, r0
    sub     r0, r0, #(FIQ_STACK_SIZE)

    /* IRQ mode */
    msr     cpsr_c, #(IRQ_MODE|NO_INT)
    bic     r0, r0, #7
    mov     sp, r0
    sub     r0, r0, #(IRQ_STACK_SIZE)

    /* SVC/SWI/USR share the same stack */
    msr     cpsr_c, #(SYS_MODE|NO_INT)  /* system mode */
    bic     r0, r0, #7
    mov     sp, r0
    msr     cpsr_c, #(SVC_MODE|NO_INT)  /* supervisor mode */
    mov     sp, r0
    sub     r0, r0, #(SVC_STACK_SIZE)

    /* here, make sure the CPU has returned to SVC mode,
     * to continue the rest init.
     */


/*
 * copy .text section from flash to RAM
 */
_copy_text:
    adr     r0, __start         /* r0 <- .text flash base address   */
    ldr     r1, _text_start     /* r1 <- target .text base address  */
    ldr     r3, _text_end
    sub     r2, r3, r1
    add     r2, r0, r2          /* r2 <- .text source end address   */

    cmp     r0, r1              /* test if we run from flash or RAM */
    beq     _copy_data          /* if run from RAM, skip copying    */

_copy_text_loop:
    ldmia   r0!, {r3-r10}       /* copy from source address [r0]    */
    stmia   r1!, {r3-r10}       /* copy to   target address [r1]    */
    cmp     r0, r2              /* until source end address [r2]    */
    blo     _copy_text_loop


/*
 * copy .data section from flash to RAM
 */
_copy_data:
    mov     r0, r2              /* r0 <- .data source start address */
    ldr     r1, _data_start     /* r1 <- target .data base address  */
    ldr     r3, _bss_start
    sub     r2, r3, r1
    add     r2, r0, r2          /* r2 <- .data source end address   */

_copy_data_loop:
    ldmia   r0!, {r3-r10}       /* copy from source address [r0]    */
    stmia   r1!, {r3-r10}       /* copy to   target address [r1]    */
    cmp     r0, r2              /* until source end address [r2]    */
    blo     _copy_data_loop


/*
 * clear bss area
 */
_clear_bss:
    ldr     r1, _bss_start      /* find start of bss segment        */
    ldr     r2, _bss_end        /* stop here                        */
    mov     r0, #0x0            /* clear                            */

_clbss_loop:
    cmp     r1, r2
    strlo   r0, [r1], #4        /* clear loop...                    */
    blo     _clbss_loop


/*
 * Jump to C program
 */
_jump_c:
    ldr     r0, =main
    bx      r0

    /* dead loop here */
    b       .

