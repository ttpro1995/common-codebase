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

#include "cli_api.h"


static const UINT8 *p_cli_version = CLI_VERSION;

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_GetVersion
 * DESCRIPTION:
 *      Get CLI Version.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      CLI Version String.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2010.4.13        Panda.Xiong         Create/Update
 *****************************************************************************/
const UINT8 *CLI_GetVersion(void)
{
    return p_cli_version;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Init
 * DESCRIPTION:
 *      Init CLI kernel.
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
    /* Init CLI VT driver */
    CLI_VT_Init();

    /* clear screen */
    CLI_VT_ClearScreen();

    CLI_HISTORY_Init();
	CLI_REG_Init();
	CLI_PAR_Init();
    CLI_IO_Init();

    /* register all internal commands */
    CLI_CMD_RegisterCmd();
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Start
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
void CLI_Start(void)
{
    CLI_IO_Start();
}

