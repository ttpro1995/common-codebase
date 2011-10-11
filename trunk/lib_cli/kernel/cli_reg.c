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


/* at first, there is no registerd command;
 * and, this is always point to the last command.
 */
static CLI_REG_CMD_T *p_cmd_end;


/******************************************************************************
 * FUNCTION NAME:
 *      cli_reg_IsCmdRegistered
 * DESCRIPTION:
 *      check this command has been registered or not.
 *      if registered, return TRUE; else, return FALSE.
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
static BOOL cli_reg_IsCmdRegistered(IN const CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return FALSE;
    }

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
		if (cmd_loop->cmd_name == NULL)
		{
			continue;
		}

        if (strcmp(cmd_loop->cmd_name, cmd->cmd_name) == 0)
        {
            /* this command has been registerd, return fail */
            return TRUE;
        }
    }

    return FALSE;
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
BOOL CLI_REG_RegisterCmd(IN CLI_REG_CMD_T *cmd)
{
    /* if the command has no command name, register fail */
    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return FALSE;
    }

    if (p_cmd_end == NULL)
    {
        p_cmd_end           = cmd;
        p_cmd_end->prev_cmd = NULL;
        p_cmd_end->next_cmd = NULL;
    }
    else
    {
        /* first, we should check this command has been registered or not */
        if (cli_reg_IsCmdRegistered(cmd))
        {
            return TRUE;
        }

        /* this is a new command, register it */
        cmd->prev_cmd       = p_cmd_end;
        p_cmd_end->next_cmd = cmd;
        p_cmd_end           = p_cmd_end->next_cmd;
        p_cmd_end->next_cmd = NULL;
    }

    return TRUE;
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
BOOL CLI_REG_DeRegisterCmd(IN CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    /* if the command has no command name, register fail */
    if ((cmd == NULL) || (cmd->cmd_name == NULL))
    {
        return FALSE;
    }

    /* if there is no registerd command, de-register command success. */
    if (p_cmd_end == NULL)
    {
        return TRUE;
    }

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
        /* this command has been registerd, de-register it */
        if (strcmp(cmd_loop->cmd_name, cmd->cmd_name) == 0)
        {
            if (cmd_loop->prev_cmd != NULL)     /* not the head command */
            {
                cmd_loop->prev_cmd->next_cmd = cmd_loop->next_cmd;

                if (cmd_loop->next_cmd != NULL)
                {
                    /* not the tail command */
                    cmd_loop->next_cmd->prev_cmd = cmd_loop->prev_cmd;
                }
                else
                {
                    /* the tail command */
                    p_cmd_end = cmd_loop->prev_cmd;
                }
            }
            else    /* head command */
            {
                cmd_loop->next_cmd->prev_cmd = NULL;
            }

            /* reset the de-registered command to initial status */
            cmd_loop->prev_cmd = NULL;
            cmd_loop->next_cmd = NULL;

            break;
        }
    }

    /*  if this command is no registerd, or is de-registerd, return success. */
    return TRUE;
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
const CLI_REG_CMD_T *CLI_REG_SearchCommand(IN const UINT8 *cmd)
{
    const CLI_REG_CMD_T *cmd_loop;

	if (cmd == NULL)
	{
		return NULL;
	}

    for (cmd_loop=p_cmd_end; cmd_loop!=NULL; cmd_loop=cmd_loop->prev_cmd)
    {
        if (strcmp(cmd_loop->cmd_name, cmd) == 0)
        {
            /* find command */
            return cmd_loop;
        }
    }

    /* not find command, return NULL */
	return NULL;
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
const CLI_REG_CMD_T *CLI_REG_GetNextCommand(IN const CLI_REG_CMD_T *cmd)
{
    CLI_REG_CMD_T *cmd_loop;

    if (cmd == NULL)
    {
        /* if input NULL, return the first registered command */
        cmd_loop = p_cmd_end;
        while (cmd_loop->prev_cmd != NULL)
        {
            cmd_loop = cmd_loop->prev_cmd;
        }

        return cmd_loop;
    }
    else
    {
        /* else, return the next command */
        return cmd->next_cmd;
    }
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
const CLI_REG_CMD_T *CLI_REG_GetPrevCommand(IN const CLI_REG_CMD_T *cmd)
{
    if (cmd == NULL)
    {
        /* if input NULL, return the last registered command */
        return p_cmd_end;
    }
    else
    {
        /* else, return the prev. command */
        return cmd->prev_cmd;
    }
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
const CLI_REG_CMD_T *CLI_REG_GetNextMatchCommand
(
    IN const CLI_REG_CMD_T *cmd,
    IN const UINT8         *match_str
)
{
    const CLI_REG_CMD_T *cmd_loop;
    CLI_CMD_LEN_T        match_cmd_len;

    if (match_str == NULL)
    {
        return NULL;
    }

    match_cmd_len = strlen(match_str);
    cmd_loop      = cmd;

    if (cmd_loop == NULL)
    {
        /* if input NULL, move to the first registered command */
        cmd_loop = p_cmd_end;
        while (cmd_loop->prev_cmd != NULL)
        {
            cmd_loop = cmd_loop->prev_cmd;
        }
    }

    /* search for the next match command */
    while (cmd_loop->next_cmd != NULL)
    {
        cmd_loop = cmd_loop->next_cmd;

        if ((cmd_loop->cmd_name != NULL)
                && (memcmp(cmd_loop->cmd_name, match_str, match_cmd_len) == 0))
        {
            return cmd_loop;
        }
    }

    return NULL;
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
UINT32 CLI_REG_GetMatchCommandInfo
(
    IN const UINT8  *match_str,
    OUT      UINT8  *least_matched_str
)
{
    const CLI_REG_CMD_T *cmd_loop;
    UINT32               count;

    const CLI_REG_CMD_T *last_matched_cmd;
    CLI_CMD_LEN_T        least_matched_len;
    CLI_CMD_LEN_T        loop;

    if ((match_str == NULL) || (least_matched_str == NULL))
    {
        return 0;
    }

    cmd_loop = NULL;
    count    = 0;
    last_matched_cmd  = NULL;
    least_matched_len = strlen(match_str);
    while ((cmd_loop = CLI_REG_GetNextMatchCommand(cmd_loop, match_str)) != NULL)
    {
        if (last_matched_cmd == NULL)
        {
            least_matched_len = strlen(cmd_loop->cmd_name);
        }
        else
        {
            for (loop = strlen(match_str);
                 loop <= MIN(strlen(cmd_loop->cmd_name), strlen(last_matched_cmd->cmd_name));
                 loop++)
            {
                if (cmd_loop->cmd_name[loop] != last_matched_cmd->cmd_name[loop])
                {
                    break;
                }
            }

            if (loop < least_matched_len)
            {
                least_matched_len = loop;
            }
        }

        last_matched_cmd = cmd_loop;
        count++;
    }

    if (last_matched_cmd != NULL)
    {
        memcpy(least_matched_str, last_matched_cmd->cmd_name, least_matched_len);
    }
    else
    {
        memcpy(least_matched_str, match_str, least_matched_len);
    }
    least_matched_str[least_matched_len] = '\0';

    return count;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_REG_Init
 * DESCRIPTION:
 *      CLI Register Init.
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
void CLI_REG_Init(void)
{
	p_cmd_end = NULL;
}

