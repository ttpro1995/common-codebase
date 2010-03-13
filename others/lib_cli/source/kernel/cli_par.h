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

#ifndef __CLI_PAR_H
#define __CLI_PAR_H


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_PAR_ParseCmd
 * DESCRIPTION:
 *      Parse command.
 * INPUT:
 *      ptr   : The input command to be parsed.
 * OUTPUT:
 *      ptr   : The new command which is parsed.
 *      param : All the parsed parameters' start address.
 * RETURN:
 *      The number of parameters.
 * NOTES:
 *      This function will directly modify the command in 'ptr'.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_U32 CLI_PAR_ParseCmd(IN OUT GT_S8 *ptr, OUT GT_S8 *param[]);

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_PAR_Init(void);


#endif /* __CLI_PAR_H */

