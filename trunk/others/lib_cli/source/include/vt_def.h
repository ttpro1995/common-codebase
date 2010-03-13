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
 *    2009.5.13    Panda Xiong       Create
 *
******************************************************************************/

#ifndef __VT_DEF_H
#define __VT_DEF_H


/* Define key ascii value */
#define VT_KEY_ESC         (27)
#define VT_KEY_CR          (13)  /* CR, '\n' */
#define VT_KEY_SPACE       (32)
#define VT_KEY_LF          (10)  /* LF, '\r' */
#define VT_KEY_TAB         (9)
#define VT_KEY_BS          (8)

/* Super Terminal not support yet,
 * but supported by "Tera Term" and "SecureCRT" */
#define VT_KEY_DEL         (0x7F)

#define VT_KEY_CTRL_D      (0x04)
#define VT_KEY_CTRL_L      (0x0C)
#define VT_KEY_CTRL_C      (0x03)
#define VT_KEY_CTRL_T      (0x14)
#define VT_KEY_CTRL_R      (0x12)
#define VT_KEY_CTRL_S      (0x13)
#define VT_KEY_CTRL_U      (0x15)
#define VT_KEY_CTRL_Z      (0x1A)

#define VT_OTHER_KEY_BASE  (200)

/* the keys below are not in the ascii table, so we should map them */
#define VT_KEY_LEFT        (VT_OTHER_KEY_BASE + 0)
#define VT_KEY_RIGHT       (VT_OTHER_KEY_BASE + 1)
#define VT_KEY_UP          (VT_OTHER_KEY_BASE + 2)
#define VT_KEY_DOWN        (VT_OTHER_KEY_BASE + 3)
#define VT_KEY_F1          (VT_OTHER_KEY_BASE + 4)
#define VT_KEY_F2          (VT_OTHER_KEY_BASE + 5)
#define VT_KEY_F3          (VT_OTHER_KEY_BASE + 6)
#define VT_KEY_F4          (VT_OTHER_KEY_BASE + 7)
#define VT_KEY_HOME        (VT_OTHER_KEY_BASE + 8)
#define VT_KEY_END         (VT_OTHER_KEY_BASE + 9)
#define VT_KEY_INSERT      (VT_OTHER_KEY_BASE + 10)

#define VT_KEY_NULL        '\0'


#endif /* __VT_DEF_H */

