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

#if   (CLI_MAX_CMD_HISTORY < 256)
 typedef UINT8  CLI_HISTORY_ID_T;
#elif (CLI_MAX_CMD_HISTORY < 65536)
 typedef UINT16 CLI_HISTORY_ID_T;
#else
 typedef UINT32 CLI_HISTORY_ID_T;
#endif


static UINT8 cmd_history[CLI_MAX_CMD_HISTORY][CLI_MAX_CMD_LEN+1];

/* history default start from 0,
 * always point to the last empty history space.
 */
static CLI_HISTORY_ID_T n_cmd_history;

/* current history number */
static CLI_HISTORY_ID_T current_history;


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
void CLI_HISTORY_AppendItem(IN const UINT8 *history)
{
    if (n_cmd_history == (CLI_MAX_CMD_HISTORY-1))
    {
        CLI_HISTORY_ID_T loop;

		for (loop=0; loop<(CLI_MAX_CMD_HISTORY-2); loop++)
		{
			memcpy(cmd_history[loop],
			       cmd_history[loop+1],
			       sizeof(cmd_history[0]));
		}

		strcpy(cmd_history[n_cmd_history-1], history);
    }
    else
    {
        strcpy(cmd_history[n_cmd_history++], history);
    }

    /* reset current history */
    current_history = n_cmd_history;
}


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
const UINT8 *CLI_HISTORY_GetNextItem(void)
{
    if (current_history == n_cmd_history)
    {
        return NULL;
    }
    else
    {
        return cmd_history[++current_history];
    }
}


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
const UINT8 *CLI_HISTORY_GetPrevItem(void)
{
    if (current_history == 0)
    {
        return NULL;
    }
    else
    {
        return cmd_history[--current_history];
    }
}


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
void CLI_HISTORY_Reset(void)
{
    current_history = n_cmd_history;
}


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
void CLI_HISTORY_Init(void)
{
	memset(cmd_history, 0x0, sizeof(cmd_history));

    /* history start from 0 */
	n_cmd_history = 0;

	/* default, current history is equal to n_cmd_history */
    current_history = n_cmd_history;
}

