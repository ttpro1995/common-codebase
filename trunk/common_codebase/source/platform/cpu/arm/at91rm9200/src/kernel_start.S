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

#include <inc/boot_define.h>
#include <inc/reg.h>

.globl  _start


.code 32
_start:
    b       _reset
    ldr     pc, _vector_undef   /* undefined vector      */
    ldr     pc, _vector_swi     /* swi vector            */
    ldr     pc, _vector_pabt    /* prefetch abort vector */
    ldr     pc, _vector_dabt    /* data abort vector     */
    b       .                   /* reserved vector       */
/* The irq and fiq exception will directly
 * read the corresponding exception deal API from AIC.
 */
#if 0
    ldr     pc, _vector_irq     /* irq vector            */
    ldr     pc, _vector_fiq     /* fiq vector            */
#else
    ldr     pc, [pc, #-0xF20]   /* irq vector            */
    ldr     pc, [pc, #-0xF20]   /* fiq vector            */
#endif

    /* reserved for image signature header, don't try to remove it */
    .space   64, 0x00

_text_base:
    .word   __KERNEL_TEXT_BASE__

_start_stack:
    .word   (start_stack + BOOT_STACK_SIZE - 1)

_armboot_start:
    .word   _start

_bss_start:
    .word   __bss_start

_bss_end:
    .word   _end


/*
 * system start from here
 */
_reset:
#if BOOT_MSG_PRINT
    adr     r0, _stage_1
    bl      _uart_putsp
#endif
    /* Enter SVC32 mode, and disable all interrupt */
    msr     cpsr_c, #(SVC_MODE|NO_INT)

/*
 * setup stack
 */
    bl      _stack_setup

#if BOOT_MSG_PRINT
    adr     r0, _stage_2
    bl      _uart_putsp
#endif

/*
 * clear bss area
 */
_clear_bss:
    ldr     r0, _bss_start      /* find start of bss segment        */
    ldr     r1, _bss_end        /* stop here                        */
    ldr     r2, =0x0            /* clear                            */

_clbss_loop:
    str     r2, [r0]            /* clear loop...                    */
    add     r0, r0, #4
    cmp     r0, r1
    ble     _clbss_loop

#if BOOT_MSG_PRINT
    adr     r0, _stage_3
    bl      _uart_putsp
#endif

/*
 * enable icache
 */
    bl      _enable_icache

#if BOOT_MSG_PRINT
    adr     r0, _stage_4
    bl      _uart_putsp
#endif

    /* enter system mode, with no interrupt */
    msr     cpsr_c, #(SYS_MODE|NO_INT)

/*
 * Jump to C program
 */
_jump_c:
    ldr     r0, =startC
    mov     lr, pc
    bx      r0

    /* dead loop here */
    b       .


	/* don't try to remove this.
	 * this is reserved for compiler to save the pool data.
	 */
    .pool


/******************************************************************************
 * enable icache
*******************************************************************************/
_enable_icache:
    mrc     p15, 0, r0, c1, c0, 0
    orr     r0, r0, #(1<<2)
    mcr     p15, 0, r0, c1, c0, 0

    /* return */
    mov     pc, lr


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
    mov     ip, lr

    ldr     r0, _start_stack

    /* supervisor mode */
    msr     cpsr_c, #(SVC_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0
    sub     r0, r0, #(SVC_STACK_SIZE)

    /* IRQ mode */
    msr     cpsr_c, #(IRQ_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0
    sub     r0, r0, #(IRQ_STACK_SIZE)

    /* FIQ mode */
    msr     cpsr_c, #(FIQ_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0
    sub     r0, r0, #(FIQ_STACK_SIZE)

    /* abort mode */
    msr     cpsr_c, #(ABORT_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0
    sub     r0, r0, #(ABORT_STACK_SIZE)

    /* undefined instruction mode */
    msr     cpsr_c, #(UDF_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0
    sub     r0, r0, #(UDF_STACK_SIZE)

    /* system mode */
    msr     cpsr_c, #(SYS_MODE|NO_INT)
    bic     r0, r0, #3
    mov     sp, r0

    /* return to supervisor mode, to continue the rest init */
    msr     cpsr_c, #(SVC_MODE|NO_INT)

    /* return */
    mov     pc, ip


/******************************************************************************
 * exception handlers
******************************************************************************/
/* undefined exception */
_vector_undef:
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_undef
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^

/* swi exception */
_vector_swi:
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_swi
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^

/* prefetch abort exception */
_vector_pabt:
    subs    lr, lr, #4
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_pabt
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^

/* data abort exception */
_vector_dabt:
    subs    lr, lr, #8
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_dabt
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^

/* irq exception */
_vector_irq:
    subs    lr, lr, #4
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_irq
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^

/* fiq exception */
_vector_fiq:
    subs    lr, lr, #4
    stmfd   sp!, {r0-r12,lr}
    ldr     r0, =vector_fiq
    mov     lr, pc
    bx      r0
    ldmfd   sp!, {r0-r12,pc}^


#if BOOT_MSG_PRINT
/******************************************************************************
 * Debug Uart putsp
 ******************************************************************************/
/* Input :
 *     R0 -- the string start address to be print.
 *
 * Registers used :  R8-R10
 */
.global _uart_putsp
_uart_putsp:

_uart_putsp_lp1:
	/* load 1 byte */
	ldrb	r8, [r0], #1
	cmp		r8, #0
	beq		_uart_putsp_exit

/* wait for Tx is empty  */
_uart_putsp_lp2:
    ldr     r9,  =(DBGU_SR)             /* DBGU_SR address       */
    ldr     r10, [r9]                   /* get the DBGU status   */
    tst     r10, #(1<<1)                /* check Tx empty status */
    beq     _uart_putsp_lp2

    /* write data */
    ldr     r9,  =(DBGU_THR)            /* DBGU_THR address      */
    str     r8, [r9]

	/* send next character */
    b       _uart_putsp_lp1

_uart_putsp_exit:
    /* return */
    mov     pc, lr


	.align  4
_stage_1:
	.ascii  "\n\renter kernel image ...\x0"

	.align  4
_stage_2:
	.ascii  "\n\rinit stack ok ...\x0"

	.align  4
_stage_3:
	.ascii  "\n\rclear bss ok ...\x0"

	.align  4
_stage_4:
	.ascii  "\n\rcache enable, and entering startC ...\x0"

#endif

	/* don't try to remove this.
	 * this is reserved for compiler to save the pool data.
	 */
    .pool

