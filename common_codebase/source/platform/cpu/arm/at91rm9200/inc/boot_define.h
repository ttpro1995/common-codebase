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

#define BOOT_MSG_PRINT      (0)

/* 7 cpu modes of arm */
#define USR_MODE            (0x10)      /* 0b10000 */
#define FIQ_MODE            (0x11)      /* 0b10001 */
#define IRQ_MODE            (0x12)      /* 0b10010 */
#define SVC_MODE            (0x13)      /* 0b10011 */
#define ABORT_MODE          (0x17)      /* 0b10111 */
#define UDF_MODE            (0x1B)      /* 0b11011 */
#define SYS_MODE            (0x1F)      /* 0b11111 */

/* global interrupt control */
#define IRQ_BIT             (1<<7)
#define FIQ_BIT             (1<<6)
#define NO_INT              (IRQ_BIT | FIQ_BIT)


/* define stack size in every mode */
#if defined(__START_FROM_SRAM__)
#define FIQ_STACK_SIZE      (0x4)
#define IRQ_STACK_SIZE      (0x4)
#define ABORT_STACK_SIZE    (0x4)
#define UDF_STACK_SIZE      (0x4)
#define SVC_STACK_SIZE      (0x400) /* 1KB */
#define USR_STACK_SIZE      (0x4) /* swi and user mode share the same stack */
#else
#define FIQ_STACK_SIZE      (0x8000) /* 16KB */
#define IRQ_STACK_SIZE      (0x8000)
#define ABORT_STACK_SIZE    (0x400)  /* 1KB  */
#define UDF_STACK_SIZE      (0x400)
#define SVC_STACK_SIZE      (0x8000) /* 16KB */
#define USR_STACK_SIZE      (0x8000) /* swi and user mode share the same stack */
#endif

/* define system initialize stack size, used by start.S */
#define BOOT_STACK_SIZE     (USR_STACK_SIZE + FIQ_STACK_SIZE             \
                            + IRQ_STACK_SIZE + SVC_STACK_SIZE            \
                            + ABORT_STACK_SIZE + UDF_STACK_SIZE)


/* define bootloader uart baudrate */
#define BOOT_BAUDRATE       (115200)


/*
 * define the PLL clock(Hz)
 */
#define CPU_MASTER_CLOCK    (59904000)

/* SDRAM define */
#define SDRAM_BASE_ADDR     (0x20000000)
#define SDRAM_SIZE          (MB(16))
#define SDRAM_END_ADDR      (SDRAM_BASE_ADDR + SDRAM_SIZE)

/* flash define */
#define FLASH_BASE_ADDR     (0x10000000)
#define FLASH_SIZE          (MB(4))

#define IS_ADDR_FLASH_AREA(addr, len)		                                  \
		((((addr) >= FLASH_BASE_ADDR)                                         \
				&& ((addr) + (len) < (FLASH_BASE_ADDR + FLASH_SIZE)))         \
		? GT_TRUE : GT_FALSE)

#define IS_ADDR_RAM_AREA(addr, len)		                                      \
	((((addr) >= SDRAM_BASE_ADDR) && ((addr) + (len) < SDRAM_END_ADDR))       \
	? GT_TRUE : GT_FALSE)


#define DELAY_MAIN_FREQ     100
#define DELAY_PLL           100


#ifdef __cplusplus
}
#endif

#endif /* __BOOT_DEFINE_H */

