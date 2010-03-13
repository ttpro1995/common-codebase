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

#ifndef __CLI_IO_H
#define __CLI_IO_H


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_IO_SetPrompt
 * DESCRIPTION:
 *      Set CLI Prompt string.
 * PARAMETERS:
 *      pCliPrompt : CLI Prompt string to be set.
 * RETURN:
 *      GT_TRUE    : Success.
 *      GT_FALSE   : Fail.
 * NOTES:
 *      The CLI Prompt string can not over 255, else, set fail.
 * HISTORY:
 *      2009.5.14        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL CLI_IO_SetPrompt(IN const GT_S8  *pCliPrompt);

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
void CLI_IO_Init(void);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_IO_Start
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
void CLI_IO_Start(void);


#endif /* __CLI_IO_H */

