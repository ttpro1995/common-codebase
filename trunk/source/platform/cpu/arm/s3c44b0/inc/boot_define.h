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

#ifndef __BOOT_DEFINE_H
#define __BOOT_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/* 7 cpu modes of arm */
#define USR_MODE            (0b10000)
#define FIQ_MODE            (0b10001)
#define IRQ_MODE            (0b10010)
#define SVC_MODE            (0b10011)
#define ABORT_MODE          (0b10111)
#define UDF_MODE            (0b11011)
#define SWI_MODE            (0b11111)

/* global interrupt control */
#define DISABLE_IRQ         (1<<7)
#define DISABLE_FIQ         (1<<6)
#define NO_INT              (DISABLE_IRQ | DISABLE_FIQ)


/* define stack size in every mode */
#define USR_STACK_SIZE      (4096 * 100)
#define FIQ_STACK_SIZE      (2048)
#define IRQ_STACK_SIZE      (2048)
#define SVC_STACK_SIZE      (2048 * 100)
#define ABORT_STACK_SIZE    (2048)
#define UDF_STACK_SIZE      (2048)
#define SWI_STACK_SIZE      (2048)

/* define system initialize stack size, used by start.S */
#define BOOT_STACK_SIZE     (USR_STACK_SIZE + FIQ_STACK_SIZE             \
                            + IRQ_STACK_SIZE + SVC_STACK_SIZE            \
                            + ABORT_STACK_SIZE + UDF_STACK_SIZE          \
                            + SWI_STACK_SIZE)


/* define bootloader uart baudrate */
#define BOOT_BAUDRATE       (115200)


/*
 * define the PLL clock(Hz)
 */
#define PLLCLK              (75000000)

#if (PLLCLK == 40000000)    /* Fin=10MHz Fout=40MHz */
#define M_DIV   (0x48)
#define P_DIV   (0x3)
#define S_DIV   (0x2)
#elif (PLLCLK == 50000000)  /* Fin=10MHz Fout=50MHz */
#define M_DIV   (0x2a)
#define P_DIV   (0x3)
#define S_DIV   (0x1)
#elif (PLLCLK == 60000000)  /* Fin=10MHz Fout=60MHz */
#define M_DIV   (0x34)
#define P_DIV   (0x3)
#define S_DIV   (0x1)
#elif (PLLCLK == 66000000)  /* Fin=10MHz Fout=66MHz */
#define M_DIV   (0x3a)
#define P_DIV   (0x3)
#define S_DIV   (0x1)
#elif (PLLCLK == 75000000)  /* Fin=10MHz Fout=75MHz */
#define M_DIV   (0x43)
#define P_DIV   (0x3)
#define S_DIV   (0x1)
#endif


/* SDRAM define */
#define SDRAM_BASE_ADDR     (0x0C000000)
#define SDRAM_SIZE          (MB(16))
#define SDRAM_END_ADDR      (SDRAM_BASE_ADDR + SDRAM_SIZE)

/* flash define */
#define FLASH_BASE_ADDR     (0x00000000)
#define FLASH_SIZE          (MB(2))

/* bootloader text base address */
#define TEXT_BASE_ADDR      (__TEXT_BASE__)

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_DEFINE_H */
