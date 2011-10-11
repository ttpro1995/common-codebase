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
static CLI_REG_CMD_RETURN_T cli_cmd_ShowHelp
(
    IN const CLI_CMD_PARAM_T    n_param,
    IN const UINT8             *param[]
)
{
    if (n_param == 1)
    {
        UINT32               cmd_count;
        const CLI_REG_CMD_T *cmd_loop;

        CLI_VT_Printf("\n\rAll Available Commands :");

        cmd_count = 0;
        cmd_loop  = NULL;
        while ((cmd_loop = CLI_REG_GetNextCommand(cmd_loop)) != NULL)
        {
            if (cmd_count % 4 == 0)
            {
                CLI_VT_Printf("\n\r");
            }

            if (cmd_loop->cmd_name != NULL)
            {
                CLI_VT_Printf(" %-18s", cmd_loop->cmd_name);
                cmd_count++;
            }
        }

        CLI_VT_Printf("\n\n\rTotal : %d", cmd_count);
    }
    else if (n_param == 2)
    {
        const CLI_REG_CMD_T *p_cmd;
        const UINT8         *p_help;

        p_cmd = CLI_REG_SearchCommand(param[1]);
        if (p_cmd == NULL)
        {
            CLI_VT_Printf("\n\r Unknown Command!");
            return CLI_PRT_FAIL;
        }

        CLI_VT_Printf("\n\rCommand : %s", p_cmd->cmd_name);
        CLI_VT_Printf("\n\rUsage   : ");

        if (p_cmd->cmd_help != NULL)
        {
            p_help = p_cmd->cmd_help;

            while (*p_help != '\0')
            {
                CLI_VT_Printf("%c", *p_help);

                if (*p_help == VT_KEY_CR)
                {
                    CLI_VT_Printf("%*s", 10, " ");
                }

                p_help++;
            }
        }
    }
    else
    {
        CLI_VT_Printf("\n\r Unknown parameter!");
        return CLI_PRT_FAIL;
    }

    return CLI_PRT_NONE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      clear screen
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
static CLI_REG_CMD_RETURN_T cli_cmd_ClearScreen
(
    IN const CLI_CMD_PARAM_T    n_param,
    IN const UINT8             *param[]
)
{
    NO_WARNING(param);

    if (n_param != 1)
    {
        CLI_VT_Printf("\n\r Unknown parameter!");
        return CLI_PRT_FAIL;
    }

    CLI_VT_ClearScreen();

    return CLI_PRT_INVALID;
}


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
static CLI_REG_CMD_RETURN_T cli_cmd_ShowHistory
(
    IN const CLI_CMD_PARAM_T    n_param,
    IN const UINT8             *param[]
)
{
    const UINT8 *history;

    NO_WARNING(param);

    if (n_param != 1)
    {
        CLI_VT_Printf("\n\r Unknown parameter!");
        return CLI_PRT_FAIL;
    }

    CLI_HISTORY_Reset();

    CLI_VT_Printf("\n\rCommand History:");
    while ((history = CLI_HISTORY_GetPrevItem()) != NULL)
    {
        CLI_VT_Printf("\n\r %s", history);
    }

    return CLI_PRT_NONE;
}


static CLI_REG_CMD_T cli_sys_cmd[] =
{
    {"?",       cli_cmd_ShowHelp,    "? [<command>] (show help information)",    NULL, NULL, },
    {"help",    cli_cmd_ShowHelp,    "help [<command>] (show help information)", NULL, NULL, },
    {"cls",     cli_cmd_ClearScreen, "cls (clear screen)",                       NULL, NULL, },
    {"clear",   cli_cmd_ClearScreen, "clear (clear screen)",                     NULL, NULL, },
    {"history", cli_cmd_ShowHistory, "history (show command history)",           NULL, NULL, }
};


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
void CLI_CMD_RegisterCmd(void)
{
    UINT8  loop;

    for (loop = 0; loop < COUNT_OF(cli_sys_cmd); loop++)
    {
        if (!CLI_REG_RegisterCmd(&cli_sys_cmd[loop]))
        {
            CLI_VT_Printf("\n\r%s(line%d): Register Internal Command %s Fail!",
                        __FILE__, __LINE__, cli_sys_cmd[loop].cmd_name);
        }
    }
}

