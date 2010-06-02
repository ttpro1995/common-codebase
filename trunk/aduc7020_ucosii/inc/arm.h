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
 *   arm.h
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.6.2        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __ARM_H
#define __ARM_H


/* 7 CPU modes of ARM */
#define MODE_MASK           (0x1F)
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


#endif /* __ARM_H */

