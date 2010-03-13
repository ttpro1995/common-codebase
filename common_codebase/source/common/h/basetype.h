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

#ifndef __BASETYPE_H
#define __BASETYPE_H

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned char       GT_UI8;
typedef unsigned short      GT_UI16;
typedef unsigned int        GT_UI32;

typedef char                GT_I8;
typedef short               GT_I16;
typedef int                 GT_I32;

typedef GT_UI32             GT_BOOL;
#define GT_FALSE            (0)
#define GT_TRUE             (1)

/* volatile address access */
#define VP32                *(volatile GT_UI32 *)
#define VP16                *(volatile GT_UI16 *)
#define VP8                 *(volatile GT_UI8  *)

/* parameter type of a function(INPUT/OUTPUT),
 * have no meanings */
#define IN                  /* input parameter  */
#define OUT                 /* output parameter */

#ifndef NULL
 #define NULL               (0)
#endif

#define INLINE              __inline
#define REENTRANT           reentrant
#define CONST               const


#include <h/base_macro.h>


#ifdef __cplusplus
}
#endif

#endif /* __BASETYPE_H */

