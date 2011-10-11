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

#ifndef __CLI_CMD_H
#define __CLI_CMD_H


#if   (CLI_MAX_CMD_LEN < 256)
 typedef UINT8  CLI_CMD_LEN_T;
#elif (CLI_MAX_CMD_LEN < 65536)
 typedef UINT16 CLI_CMD_LEN_T;
#else
 typedef UINT32 CLI_CMD_LEN_T;
#endif

#if   (CLI_MAX_PARAM < 256)
 typedef UINT8  CLI_CMD_PARAM_T;
#elif (CLI_MAX_PARAM < 65536)
 typedef UINT16 CLI_CMD_PARAM_T;
#else
 typedef UINT32 CLI_CMD_PARAM_T;
#endif


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_CMD_RegisterCmd
 * DESCRIPTION:
 *      Register All Internal Commands.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2010.4.13        Panda.Xiong         Create/Update
 *****************************************************************************/
void CLI_CMD_RegisterCmd(void);


#endif /* __CLI_CMD_H */

