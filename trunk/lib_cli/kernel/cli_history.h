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

#ifndef __CLI_HISTORY_H
#define __CLI_HISTORY_H


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_HISTORY_AppendItem
 * DESCRIPTION:
 *      Append CLI history item.
 * INPUT:
 *      CLI history item to be appended.
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
void CLI_HISTORY_AppendItem(IN const UINT8 *history);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_HISTORY_GetNextItem
 * DESCRIPTION:
 *      Get CLI next item history.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      CLI next item history.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
const UINT8 *CLI_HISTORY_GetNextItem(void);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_HISTORY_GetPrevItem
 * DESCRIPTION:
 *      Get CLI previous item history.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      CLI previous item history.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
const UINT8 *CLI_HISTORY_GetPrevItem(void);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_HISTORY_Reset
 * DESCRIPTION:
 *      CLI history internal reset.
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
void CLI_HISTORY_Reset(void);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_HISTORY_Init
 * DESCRIPTION:
 *      Init CLI history.
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
void CLI_HISTORY_Init(void);


#endif /* __CLI_HISTORY_H */

