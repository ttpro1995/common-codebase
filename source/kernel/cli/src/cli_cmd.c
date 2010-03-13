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
static CLI_REG_CMD_RETURN_T cli_cmd_ShowHelp(
    IN       GT_UI32     n_param,
    IN const GT_UI8     *param[])
{
    #define CLI_MAX_HELP_LEN    (CLI_SCREEN_WIDTH - 14)

    CLI_REG_CMD_T *cmd_loop;
    GT_UI32        help_len;
    GT_UI8        *p_help;
    GT_UI8         str_buf[CLI_MAX_HELP_LEN + 1];

    CLI_VT_Printf("\n\rAll Commands:");

    cmd_loop = NULL;
    while ((cmd_loop = CLI_REG_GetNextCommand(cmd_loop)) != NULL)
    {
        if (cmd_loop->cmd_name != NULL)
        {
            CLI_VT_Printf("\n\r %-10s : ", cmd_loop->cmd_name);

            if (cmd_loop->cmd_help != NULL)
            {
                p_help   = cmd_loop->cmd_help;
                help_len = strlen(cmd_loop->cmd_help);

                /* here, if the help is more than one line,
                 * we will segment it into multi-lines to display.
                 */
                while ((*p_help != '\0') && (help_len > CLI_MAX_HELP_LEN))
                {
                    strncpy(str_buf, p_help, CLI_MAX_HELP_LEN);
                    CLI_VT_Printf("%s%*s",
                                  str_buf,
                                  (CLI_SCREEN_WIDTH - CLI_MAX_HELP_LEN),
                                  " ");

                    help_len -= CLI_MAX_HELP_LEN;
                    p_help   += CLI_MAX_HELP_LEN;
                }

                if ((*p_help != '\0') && (help_len > 0))
                {
                    CLI_VT_Printf("%s", p_help);
                }
            }
        }
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
static CLI_REG_CMD_RETURN_T cli_cmd_ClearScreen(
    IN       GT_UI32     n_param,
    IN const GT_UI8     *param[])
{
    CLI_VT_ClearScreen();

    return CLI_PRT_EMPTY;
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
static CLI_REG_CMD_RETURN_T cli_cmd_ShowHistory(
    IN       GT_UI32     n_param,
    IN const GT_UI8     *param[])
{
    GT_UI8 *history;

    CLI_HISTORY_ResetCurrentPtr();

    CLI_VT_Printf("\n\rCommand History:");
    while ((history = CLI_HISTORY_GetPrevHistory()) != NULL)
    {
        CLI_VT_Printf("\n\r %s", history);
    }

    return CLI_PRT_NONE;
}


static CLI_REG_CMD_T cli_sys_cmd[] =
{
    {(GT_UI8 *)"?",       cli_cmd_ShowHelp,      (GT_UI8 *)"? (show help information)"},
    {(GT_UI8 *)"help",    cli_cmd_ShowHelp,      (GT_UI8 *)"help (show help information)"},
    {(GT_UI8 *)"cls",     cli_cmd_ClearScreen,   (GT_UI8 *)"cls (clear screen)"},
    {(GT_UI8 *)"clear",   cli_cmd_ClearScreen,   (GT_UI8 *)"clear (clear screen)"},
    {(GT_UI8 *)"history", cli_cmd_ShowHistory,   (GT_UI8 *)"history (show command history)"}
};


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
GT_BOOL CLI_CMD_RegisterCmd(void)
{
    GT_UI32  loop;

    for (loop = 0; loop < sizeof(cli_sys_cmd)/sizeof(cli_sys_cmd[0]); loop++)
    {
        if (!CLI_REG_RegisterCmd(&cli_sys_cmd[loop]))
        {
            CLI_VT_Printf("\n\r%s(line%d): Register Command %s Fail!",
                        __FILE__, __LINE__, cli_sys_cmd[loop].cmd_name);
        }
    }

    return GT_TRUE;
}

