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
 *   boot_define.h
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.5.31        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __BOOT_DEFINE_H
#define __BOOT_DEFINE_H


/* define stack size in every mode, in the unit of Byte.
 *
 * Note: the stack size should be aligned to 8Byte.
 */
#define EXCEPTION_STACK_SIZE (32)   /* ABT/UDF mode share the same stack     */
#define FIQ_STACK_SIZE       (32)
#define IRQ_STACK_SIZE       (512)
#define SVC_STACK_SIZE       (512)  /* SVC/SWI/USR mode share the same stack */

/* define system initialize stack size */
#define BOOT_STACK_SIZE     (EXCEPTION_STACK_SIZE + FIQ_STACK_SIZE      \
                            + IRQ_STACK_SIZE + SVC_STACK_SIZE)



#define BASE_CLOCK          41780000UL    /* 41.78MHz  */
#define CDVAL               0
#define CORE_CLOCK          (BASE_CLOCK >> CDVAL)
#define INTERNAL_OSC_CLOCK  32768UL       /* 32.768KHz */

#define OS_TICK_TIME        (1000/OS_TICKS_PER_SEC)    /* ms */



#endif /* __BOOT_DEFINE_H */

