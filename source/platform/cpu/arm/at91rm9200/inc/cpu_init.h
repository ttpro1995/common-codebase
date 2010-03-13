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

#ifndef __CPU_INIT_H
#define __CPU_INIT_H

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_Init
 * DESCRIPTION:
 *      Init CPU.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : init success;
 *      GT_FALSE : init fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __CPU_INIT_H */

