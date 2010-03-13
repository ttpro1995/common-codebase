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
 *    2007.03.27    Panda Xiong       Copied from u-boot, and modified
 *
******************************************************************************/

#include <inc/boot_define.h>
#include <inc/reg.h>


/*
 * lowlevel init, initial memory bank
 */
.global _lowlevel_init
_lowlevel_init:
    /* backup return address */
    mov     ip, lr

/*
 * post init main oscillator
 */
    bl      _post_clock_init

/*
 * init Flash
 * must be done before change the clocks -> Set correct Wait States
 */
    bl      _flash_init

/*
 * second init main oscillator
 */
    bl      _second_clock_init

#if BOOT_MSG_PRINT
/*
 * init uart
 */
    bl      _uart_init

    adr     r11, _uart_init_ok
    bl      _uart_putsp
#endif

/*
 * init SDRAM
 */
    bl      _sdram_init

#if BOOT_MSG_PRINT
    adr     r11, _sdram_init_ok
    bl      _uart_putsp
#endif

	/* return */
	mov	    pc, ip


/******************************************************************************
 * post and second init clock
 * init CPU frequency to about 180MHz(=PLLA=179.912MHz)
 ******************************************************************************/
/*********************************************
 * post init clock
 *********************************************/
 _post_clock_init:

/*
 * After reset, Slow Clock is normally selected,
 * But in case of a boot already started, re-selection of Slow Clock,
 * In two steps because of constraints of the Master Clock selection sequence,
 * Can be cleared if the project is used for a boot execution.
 */
#if 0 //def __START_FROM_SRAM__
/* set 0a :
 *
 * Write in the MCKR dirty value concerning the clock selection CSS,
 * then overwrite it in a second sequence.
 */
	ldr		r0, =(PMC_MCKR)
	ldr		r1, =0
	str		r1, [r0]
    /* Reading the PMC Status register to detect when the Master Clock is commuted */
    ldr     r0, =(PMC_SR)
_init_pmc_lp1:
    ldr     r1, [r0]
    tst     r1, #(1<<3)
    bne     _init_pmc_lp1

	/* Second sequence */
	ldr		r0, =(PMC_MCKR)
	ldr		r1, =0
	str		r1, [r0]
    /* Reading the PMC Status register to detect when the Master Clock is commuted */
    ldr     r0, =(PMC_SR)
_init_pmc_lp2:
    ldr     r1, [r0]
    tst     r1, #(1<<3)
    bne     _init_pmc_lp2

/* set 0b :
 *
 * After reset, PLLs are disabled,
 * But in case of a boot already started, PLLs are turned off,
 * Can be cleared if the project is used for a boot execution.
 */
 	/* turn off PLLA */
 	ldr		r0, =(CKGR_PLLAR)
	ldr		r1, =0
	str		r1, [r0]

	/* turn off PLLB */
 	ldr		r0, =(CKGR_PLLBR)
	ldr		r1, =0
	str		r1, [r0]

 #endif

    /* enable main oscillator, OSCOUNT = 0xFF */
    ldr     r0, =(CKGR_MOR)         /* CKGR_MOR address           */
    ldr 	r1, =0x0000FF01         /* enable main oscillator     */
    str     r1, [r0]

    /* return */
    mov     pc, lr


/*********************************************
 * second init clock, including PLLA and PLLB
 *********************************************/
_second_clock_init:

    /* wait Main Oscillator to be steady */
    ldr 	r0, =0x00000010
_clock_init_lp1:
    subs    r0, r0, #1
    bhi     _clock_init_lp1

    /* Waiting for the main clock frequency is ready */
    ldr     r0, =(CKGR_MCFR)
    ldr     r2, =(DELAY_MAIN_FREQ)
_clock_init_lp2:
    ldr     r1, [r0]
    tst     r1, #(1<<16)
    beq     _set_pll_a
    subs    r2, r2, #1
    bne     _clock_init_lp2

_set_pll_a:
    ldr     r0, =(CKGR_PLLAR)
    ldr     r1, =0x20263E04     /* 179.712MHz for PLLA */
    str     r1, [r0]
    /* Wait for PLLA stabilization LOCKA bit in PMC_SR */
    ldr     r0, =(PMC_SR)
    ldr     r2, =(DELAY_PLL)
_clock_init_lp3:
    ldr     r1, [r0]
    tst     r1, #(1<<1)
    beq     _set_pll_b
    subs    r2, r2, #1
    bne     _clock_init_lp3

_set_pll_b:
#if 0
    ldr     r0, =(CKGR_PLLBR)
    ldr     r1, =0x10483E0E     /* 48,054857 MHz (divider by 2 for USB) */
    str     r1, [r0]
    /* Wait for PLLB stabilization LOCKB bit in PMC_SR */
    ldr     r0, =(PMC_SR)
    ldr     r2, =(DELAY_PLL)
_clock_init_lp4:
    ldr     r1, [r0]
    tst     r1, #(1<<2)
    beq     _update_mckr
    subs    r2, r2, #1
    bne     _clock_init_lp4

/* Selection of Master Clock MCK (and Processor Clock PCK) */
_update_mckr:
#endif
#if 0 //def __START_FROM_SRAM__
	/* Constraints of the Master Clock selection sequence.
	 * Write in the MCKR dirty value concerning the clock selection CSS,
	 * then overwrite it in a second sequence.
	 */
    ldr     r0, =(PMC_MCKR)
    ldr     r1, =0
    str     r1, [r0]
    /* Wait until the master clock is established */
    ldr     r0, =(PMC_SR)
    ldr     r2, =(DELAY_MAIN_FREQ)
_clock_init_lp5:
    ldr     r1, [r0]
    tst     r1, #(1<<3)      /* PMC_MCKRDY bit */
    beq     _second_sequence
    subs    r2, r2, #1
    bne     _clock_init_lp5
#endif

_second_sequence:
    ldr     r0, =(PMC_MCKR)
    ldr     r1, =0x00000202 /* PLLA=CPU=179.712MHz */
    str     r1, [r0]
    /* Wait until the master clock is established */
    ldr     r0, =(PMC_SR)
    ldr     r2, =(DELAY_MAIN_FREQ)
_clock_init_lp6:
    ldr     r1, [r0]
    tst     r1, #(1<<3)      /* PMC_MCKRDY bit */
    beq     _clock_exit
    subs    r2, r2, #1
    bne     _clock_init_lp6

_clock_exit:
    /* return */
    mov     pc, lr


/******************************************************************************
 * init SDRAM
 ******************************************************************************/
#define CLEAR_SDRAM_LOOP    ldr     r0, =SDRAM_BASE_ADDR;    \
                            ldr     r1, =0x0;                \
                            str     r1, [r0];

_sdram_init:
    ldr     r0, =(PIO_ASR(PIOC_BASE_REG))   /* PIOC_ASR address           */
    ldr     r1, =0xFFFF0000                 /* set PIOC[16:31] to D16-D31 */
    str     r1, [r0]

    ldr     r0, =(PIO_BSR(PIOC_BASE_REG))   /* PIOC_BSR address           */
    ldr     r1, =0x0
    str     r1, [r0]

    ldr     r0, =(PIO_PDR(PIOC_BASE_REG))   /* PIOC_PDR address           */
    ldr     r1, =0xFFFF0000                 /* disable PIO control [16:31]*/
    str     r1, [r0]

    ldr     r0, =(EBI_CSA)                  /* EBI_CSA register           */
    ldr     r1, =(1<<1)                     /* SDRAM is connected on CS1  */
    str     r1, [r0]

    ldr     r0, =(SDRAMC_CR)                /* SDRAMC_CR register         */
    ldr     r1, =0x2188c154
    str     r1, [r0]

    ldr     r0, =(SDRAMC_MR)                /* SDRAMC_MR register         */
    ldr     r1, =0x02                       /* all banks PreCharge        */
    str     r1, [r0]

    CLEAR_SDRAM_LOOP

    ldr     r0, =(SDRAMC_MR)                /* SDRAMC_MR register         */
    ldr     r1, =0x04                       /* Refresh                    */
    str     r1, [r0]

    /* loop */
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP
    CLEAR_SDRAM_LOOP

    ldr     r0, =(SDRAMC_MR)                /* SDRAMC_MR register         */
    ldr     r1, =0x03                       /* Load Mode Register         */
    str     r1, [r0]

    ldr     r0, =(SDRAM_BASE_ADDR + 0x200)  /* 0x80 * 4                   */
    ldr     r1, =0x0
    str     r1, [r0]

    ldr     r0, =(SDRAMC_TR)                /* SDRAMC_TR register         */
    ldr     r1, =0x2E0                      /* Refresh Time Count         */
    str     r1, [r0]

    CLEAR_SDRAM_LOOP

    ldr     r0, =(SDRAMC_MR)                /* SDRAMC_MR register         */
    ldr     r1, =0x00                       /* Normal Node                */
    str     r1, [r0]

    CLEAR_SDRAM_LOOP

    /* return */
    mov     pc, lr


/******************************************************************************
 * init Flash
 ******************************************************************************/
_flash_init:
    ldr     r0, =(EBI_CFGR)                 /* data bus D[15:0] pull up    */
    ldr     r1, =0x00
    str     r1, [r0]

    ldr     r0, =(SMC_CSR(0))               /* SMC CS                      */
    ldr     r1, =0x00003288                 /* flash is 16bit device       */
    str     r1, [r0]

    /* return */
    mov     pc, lr


#if BOOT_MSG_PRINT

/******************************************************************************
 * init Debug Uart
 ******************************************************************************/
.global _uart_init
_uart_init:
    /*
     * Init PIOA for DBGU
     */
    ldr     r1, =(PIO_ASR(PIOA_BASE_REG))   /* PIOA_ASR address      */
    ldr     r2, =((1<<31) | (1<<30))        /* disable Tx and Rx     */
    str     r2, [r1]

    ldr     r1, =(PIO_BSR(PIOA_BASE_REG))   /* PIOA_BSR address      */
    ldr     r2, =0x0
    str     r2, [r1]

    ldr     r1, =(PIO_PDR(PIOA_BASE_REG))   /* PIOA_PDR address      */
    ldr     r2, =((1<<31) | (1<<30))
    str     r2, [r1]

    /*
     * Init DBGU
     */
    ldr     r1, =(DBGU_CR)                  /* DBGU_CR address       */
    ldr     r2, =(0xAC)                     /* disable Rx and Tx     */
    str     r2, [r1]

    ldr     r1, =(DBGU_IDR)                 /* DBGU_IDR address      */
    ldr     r2, =(~0)                       /* disable all interrupt */
    str     r2, [r1]

    ldr     r1, =(DBGU_MR)                  /* DBGU_MR address       */
    ldr     r2, =(0x800)                    /* no parity,nornal mode */
    str     r2, [r1]

    ldr     r1, =(DBGU_BRGR)                /* DBGU_BRGR address     */
    ldr     r2, =(CPU_MASTER_CLOCK/(BOOT_BAUDRATE*16))
    str     r2, [r1]

    ldr     r1, =(DBGU_CR)                  /* DBGU_CR address       */
    ldr     r2, =((1<<4) | (1<<6))          /* Enable Tx and Rx      */
    str     r2, [r1]

    /* return */
    mov     pc, lr


/******************************************************************************
 * Debug Uart putsp
 ******************************************************************************/
/* Input :
 *     R11 -- the string start address to be print.
 *
 * Registers used :  R8-R10
 */
.global _uart_putsp
_uart_putsp:

_uart_putsp_lp1:
	/* load 1 byte */
	ldrb	r8, [r11], #1
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
_uart_init_ok:
	.ascii  "\n\rDBG_Uart init ok ...\x0"

	.align  4
_sdram_init_ok:
	.ascii  "\n\rSDRAM init ok ...\x0"

#endif

	/* don't try to remove this.
	 * this is reserved for compiler to save the pool data.
	 */
    .pool

