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
 *   typedef.h
 * DESCRIPTION:
 *   Base type definition.
 * HISTORY:
 *   2010.6.2        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __TYPEDEF_H
#define __TYPEDEF_H


typedef unsigned char           BOOL;       /* Boolean: TRUE/FALSE */
typedef unsigned char           BYTE;
typedef unsigned char           UINT8;      /*  8-bit */
typedef signed   char           SINT8;
typedef unsigned short          UINT16;     /* 16-bit */
typedef signed   short          SINT16;
typedef unsigned long           UINT32;     /* 32-bit */
typedef signed   long           SINT32;
typedef unsigned long long      UINT64;     /* 64-bit */
typedef signed   long long      SINT64;

#define TRUE                    1
#define FALSE                   0

/* NULL pointer */
#define NULL                    ((void *)0)


/* bit operation */
#define SET_BIT(n, b)       do { (n) |= (1 << (b));  } while (0)
#define CLR_BIT(n, b)       do { (n) &= ~(1 << (b)); } while (0)
#define READ_BIT(n, b)      ((BOOL)(((n) >> (b)) & 1))
#define REVERSE_BIT(n, b)   do { (n) ^= (1 << (b)); } while (0)


#endif  /* __TYPEDEF_H */

