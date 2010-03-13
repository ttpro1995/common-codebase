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

#ifndef __STDARG_H
#define __STDARG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef char *va_list;

/* align all argument length to multiple of sizeof(int) bytes */
#define _INTSIZEOF(n)           ((sizeof(n)+sizeof(int)-1) & ~(sizeof(int)-1))

#define va_start(ap, v)         ((ap) = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap, t)           (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define va_end(ap)              (ap = ((va_list)0))

#ifdef __cplusplus
}
#endif

#endif /* __STDARG_H */

