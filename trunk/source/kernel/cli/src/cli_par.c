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
GT_UI8 CLI_PAR_ParseCmd(IN OUT GT_UI8 *ptr, OUT GT_UI8 *param[])
{
	GT_UI8	n = 0;

	if (ptr == NULL)
	{
		return 0;
	}

	while (*ptr != '\0')
	{
		/* skip spaces */
		while (*ptr == ' ')
		{
			ptr++;
		}

		if (*ptr == '\0')
		{
			return n;
		}

		param[n++] = ptr;
		while (*ptr != ' ')
		{
			if (*ptr == '\0')
			{
				return n;
			}

			ptr++;
		}

		/* add end character to this parameter */
		*ptr++ = '\0';
	}

	return n;
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
GT_BOOL CLI_PAR_Init(void)
{
	return GT_TRUE;
}

