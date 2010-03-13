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

#define _CLI_KERNEL_SOURCE_

#include "../inc/cli_api.h"
#include "../cmd/inc/cmd_app.h"
#include "../cmd/inc/cmd_os.h"


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Init
 * DESCRIPTION:
 *      CLI Entry, will not return.
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
void CLI_Init(void)
{
    /* clear screen */
    CLI_VT_ClearScreen();

    CLI_HISTORY_Init();
	CLI_REG_Init();
	CLI_PAR_Init();

    /* register all cli reserved commands */
    CLI_CMD_RegisterCmd();

    /* register all application commands */
    CMD_APP_RegisterCmd();
    CMD_OS_RegisterCmd();

    CLI_IO_Init();
}

