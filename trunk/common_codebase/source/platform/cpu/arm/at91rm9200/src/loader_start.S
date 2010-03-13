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

.globl  _start


.code 32
_start:
    b       _reset
#if 1
/* here, we jump to kernel text base address in SDRAM,
 *   to deal the CPU exception.
 * The irq and fiq exception will directly read exception deal API from AIC.
 *
 * There is no exception deal API in bootloader.
 */
    add     pc, pc,  #__KERNEL_TEXT_BASE__    /* undefined vector      */
    add     pc, pc,  #__KERNEL_TEXT_BASE__    /* swi vector            */
    add     pc, pc,  #__KERNEL_TEXT_BASE__    /* prefetch abort vector */
    add     pc, pc,  #__KERNEL_TEXT_BASE__    /* data abort vector     */
    add     pc, pc,  #__KERNEL_TEXT_BASE__    /* reserved vector       */
    ldr     pc, [pc, #-0xF20]                 /* irq vector            */
    ldr     pc, [pc, #-0xF20]                 /* fiq vector            */
#endif

    /* reserved for image signature header, don't try to remove it */
    .space   64, 0x00

_text_base:
    .word   __LOADER_TEXT_BASE__

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

    /* Enter SVC32 mode, and disable all interrupt */
    msr     cpsr_c, #(SVC_MODE|NO_INT)

/*
 * lowlevel init
 */
    bl      _lowlevel_init

	/* flush v4 I/D caches */
	mov	    r0, #0
	mcr	    p15, 0, r0, c7, c7, 0	/* flush v3/v4 cache               */
	mcr	    p15, 0, r0, c8, c7, 0	/* flush v4 TLB                    */

	/* disable MMU stuff and cache */
	mrc	    p15, 0, r0, c1, c0, 0
	bic	    r0, r0, #0x00002300	    /* clear bits 13, 9:8 (--V- --RS)  */
	bic	    r0, r0, #0x00000087	    /* clear bits 7, 2:0 (B--- -CAM)   */
	orr	    r0, r0, #0x00000002	    /* set bit 2 (A) Align             */
	orr	    r0, r0, #0x00001000	    /* set bit 12 (I) I-Cache          */
	orr     r0, r0, #0xC0000000     /* Asynchronous clock mode, not Fast Bus */
	mcr	    p15, 0, r0, c1, c0, 0

#if BOOT_MSG_PRINT
    adr     r11, _coprocessor_init_ok
    bl      _uart_putsp
#endif


/*
 * setup stack
 */
    bl      _stack_setup

#if BOOT_MSG_PRINT
    adr     r11, _stack_init_ok
    bl      _uart_putsp
#endif

/*
 * copy Bootloader to _text_base
 */
_copy:
    adr     r0, _start          /* r0 <- flash base address         */
    ldr     r1, _text_base      /* r1 <- the taget address          */
    cmp     r0, r1              /* test if we run from flash or RAM */
    beq     _clear_bss

    ldr     r2, _armboot_start
    ldr     r3, _bss_start
    sub     r2, r3, r2          /* r2 <- size of bootloader         */
    add     r2, r0, r2          /* r2 <- source end address         */

_copy_loop:
    ldmia   r0!, {r3-r10}   /* copy from source address [r0]    */
    stmia   r1!, {r3-r10}   /* copy to   target address [r1]    */
    cmp     r0, r2              /* until source end addreee [r2]    */
    ble     _copy_loop

#if BOOT_MSG_PRINT
    adr     r11, _image_copy_ok
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
    adr     r11, _bss_clear_ok
    bl      _uart_putsp
#endif

/*
 * enable icache
 */
    bl      _enable_icache

#if BOOT_MSG_PRINT
    adr     r11, _icache_init_ok
    bl      _uart_putsp
#endif

/*
 * Jump to C program
 */
_jump_c:
#if BOOT_MSG_PRINT
    adr     r11, _enter_c_procedure
    bl      _uart_putsp
#endif

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
 * debug message
******************************************************************************/

#if BOOT_MSG_PRINT

	.align 4
_coprocessor_init_ok:
	.ascii "\n\rCoprocessor init ok ...\x0"

	.align 4
_stack_init_ok:
	.ascii "\n\rStack init ok ...\x0"

	.align 4
_image_copy_ok:
	.ascii "\n\rImage copy ok ...\x0"

	.align 4
_bss_clear_ok:
	.ascii "\n\rBss clear ok ...\x0"

	.align 4
_icache_init_ok:
	.ascii "\n\rIcache init ok ...\x0"

	.align 4
_enter_c_procedure:
	.ascii "\n\rEnter C procedure ...\x0"

#endif

