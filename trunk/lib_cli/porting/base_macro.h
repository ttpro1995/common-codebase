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
 *   base_macro.h
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2010.4.13        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __BASE_MACRO_H
#define __BASE_MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#ifndef __GNUC__
 #define __INLINE           inline
#else
 #define __INLINE           /* nothing */
#endif

/* volatile/none-volatile address access */
#define VP32                *(volatile UINT32 *)
#define VP16                *(volatile UINT16 *)
#define VP8                 *(volatile UINT8  *)
#define P32                 *(UINT32 *)
#define P16                 *(UINT16 *)
#define P8                  *(UINT8  *)

#define KB(x)               ((x)*1024)
#define MB(x)               ((x)*1024*1024)

/* Calculate the total element of an array.
 *
 * e.g:
 *   int  array[100];
 *  To get the total element of (array), just use COUNT_OF(array).
 *   acturally, the total element in this sample is 100.
 */
#define COUNT_OF(n)         (SINT32)(sizeof(n) / sizeof((n)[0]))


/* calculate the Byte Offset of member in struct.
 *
 * e.g:
 *  typedef struct
 *  {
 *      int  a;
 *      int  b;
 *      char c;
 *  } TYPE_S;
 *
 * To get the Byte Offset of (c) in TYPE_S, just use offsetof(TYPE_S, c).
 *  acturally, the offset in this sample is 8.
 */
#undef offsetof
#ifdef __compiler_offsetof
 #define offsetof(TYPE, MEMBER)  __compiler_offsetof(TYPE, MEMBER)
#else
 #define offsetof(TYPE, MEMBER)  ((size_t) &((TYPE *)0)->MEMBER)
#endif


/* remove warning */
#define NO_WARNING(x)       do { if (x) {} } while (0)

#ifndef __GNUC__
 #define __attribute__(x)   /* nothing */
#endif


#ifndef MIN
 #define MIN(a, b)          (((a)<(b))?(a):(b))
#endif

#ifndef MAX
 #define MAX(a, b)          (((a)>(b))?(a):(b))
#endif


/* bit operation */
#define SET_BIT(n, b)       do { (n) |= (1 << (b));  } while (0)
#define CLR_BIT(n, b)       do { (n) &= ~(1 << (b)); } while (0)
#define READ_BIT(n, b)      ((BOOL)(((n) >> (b)) & 1))


/* get the absolute value */
#define ABS(x)              (((x) >= 0) ? (x) : -(x))


/* operate memory as big-endian */
#define SET_16(buf, val)                                                    \
    do {                                                                    \
        ((UINT8 *)(buf))[0] = (UINT8)(((UINT16)(val) >>  8) & 0xFF);        \
        ((UINT8 *)(buf))[1] = (UINT8)(((UINT16)(val) >>  0) & 0xFF);        \
    } while (0)

#define GET_16(buf)    (UINT16)(  ((UINT16)((const UINT8 *)(buf))[0] <<  8) \
                                | ((UINT16)((const UINT8 *)(buf))[1] <<  0))

#define SET_32(buf, val)                                                    \
    do {                                                                    \
        ((UINT8 *)(buf))[0] = (UINT8)(((UINT32)(val) >> 24) & 0xFF);        \
        ((UINT8 *)(buf))[1] = (UINT8)(((UINT32)(val) >> 16) & 0xFF);        \
        ((UINT8 *)(buf))[2] = (UINT8)(((UINT32)(val) >>  8) & 0xFF);        \
        ((UINT8 *)(buf))[3] = (UINT8)(((UINT32)(val) >>  0) & 0xFF);        \
    } while (0)

#define GET_32(buf)    (  ((UINT32)((const UINT8 *)(buf))[0] << 24)         \
                        | ((UINT32)((const UINT8 *)(buf))[1] << 16)         \
                        | ((UINT32)((const UINT8 *)(buf))[2] <<  8)         \
                        | ((UINT32)((const UINT8 *)(buf))[3] <<  0))


/* Swap data:  Big-Endian  <-->  Little-Endian */
#define SWAP_16(x)     (UINT16)(  (((UINT16)(x) >> 8) & 0x00FF)             \
                                | (((UINT16)(x) << 8) & 0xFF00))

#define SWAP_32(x)     (  (((UINT32)(x) >> 24) & 0x000000FF)                \
                        | (((UINT32)(x) >>  8) & 0x0000FF00)                \
                        | (((UINT32)(x) <<  8) & 0x00FF0000)                \
                        | (((UINT32)(x) << 24) & 0xFF000000))


#endif /* __BASE_MACRO_H */

