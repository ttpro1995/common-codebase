/******************************************************************************
 *
 * (C) Copyright 2008
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
 *    2008.11.30    Panda Xiong       Create
 *
******************************************************************************/

#ifndef __BASE_MACRO_H
#define __BASE_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif


#include <inc/utl_string.h>


#ifndef __GNUC__
 #define __attribute__(x)	/* nothing */
#endif

#define KB(x)               ((x)*1024)
#define MB(x)               ((x)*1024*1024)

#define NO_WARNING(x)       if(x){}

/* calucate element number of an array */
#define COUNT_OF(x)		(int)(sizeof(x) / sizeof((x)[0]))

#ifndef MIN
#define	MIN(a, b)		(((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define	MAX(a, b)		(((a)>(b))?(a):(b))
#endif


#ifdef __cplusplus
}
#endif

#endif /* __BASE_MACRO_H */

