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
static void cli_io_Task(void)
{
	GT_I32		    key_value;
	GT_UI32		    n_param;
	CLI_REG_CMD_T  *cmd = NULL;
	GT_UI8		   *p_cmd_buf;
	GT_UI8         *p_history;

    GT_UI32         match_cmd_num;
    CLI_REG_CMD_T  *p_match_cmd;

    GT_UI8	        cmd_buffer[CLI_MAX_CMD_LEN+1];  /* command buffer      */
	GT_UI8		    tmp_buffer[CLI_MAX_CMD_LEN+1];  /* temp command buffer */
    GT_UI8	       *commandParams[CLI_MAX_PARAM];

    /* insert mode or not, default set to insert mode.
     *    = GT_TRUE  : insert mode;
     *    = GT_FALSE : overwrite mode.
     */
    GT_BOOL       insertEnable = GT_TRUE;

    while (1)
    {
    	memset(cmd_buffer, 0x0, sizeof(cmd_buffer));
    	p_cmd_buf = cmd_buffer;

    	/* display prompt */
    	CLI_VT_Printf("\n\r%s>", CLI_PROMPT);

    	while (key_value = CLI_IO_ReadKey(),
    	            (key_value != VT_KEY_CR) && (key_value != VT_KEY_LF))
    	{
    		switch (key_value)
    		{
			case -1:  /* not a valid key */
				break;

			case VT_KEY_INSERT:  /* set insert/overwrite mode */
				insertEnable = (insertEnable)? GT_FALSE : GT_TRUE;
				break;

            case VT_KEY_CTRL_L:  /* clear screen, but don't clear the current command line */
                CLI_VT_ClearScreen();
                p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
                CLI_VT_Printf("\n\r%s>%s", CLI_PROMPT, cmd_buffer);
                break;

			case VT_KEY_CTRL_Z:  /* clear current command line */
			case VT_KEY_CTRL_U:
				/* move cursor to end */
				CLI_VT_MoveRight(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

                /* move to the end of this command */
				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				while (p_cmd_buf-- > cmd_buffer)
				{
					CLI_VT_Printf("\b \b");
				}

				/* clear command line in buffer */
				memset(cmd_buffer, 0x0, sizeof(cmd_buffer));
				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				break;

            case VT_KEY_TAB:
                /* here, we use tmp_buffer to get all matched commands */
                memcpy(tmp_buffer, cmd_buffer, sizeof(cmd_buffer));

                match_cmd_num = CLI_REG_GetMatchCommandNum(tmp_buffer);
                if (match_cmd_num == 0)
                {
                    break;
                }
                else if (match_cmd_num == 1)
                {
                    /* move to the end of this command */
    				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
    				while (p_cmd_buf-- > cmd_buffer)
    				{
    					CLI_VT_Printf("\b \b");
    				}

                    p_match_cmd = CLI_REG_GetNextMatchCommand(NULL, tmp_buffer);

                    strcpy(cmd_buffer, p_match_cmd->cmd_name);
				    p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				    CLI_VT_Printf("%s", cmd_buffer);
                }
                else
                {
                    p_match_cmd = NULL;
                    CLI_VT_Printf("\n\r");
                    while ((p_match_cmd = CLI_REG_GetNextMatchCommand(p_match_cmd, tmp_buffer)) != NULL)
                    {
                        strcpy(cmd_buffer, p_match_cmd->cmd_name);
    				    p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
    				    CLI_VT_Printf("%-19s ", cmd_buffer);
                    }

                    /* restore the command buffer */
                    strcpy(cmd_buffer, tmp_buffer);
				    p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
                    CLI_VT_Printf("\n\r%s>%s", CLI_PROMPT, cmd_buffer);
                }
                break;

			case VT_KEY_HOME:
                /* move cursor to head */
				CLI_VT_MoveLeft(p_cmd_buf - cmd_buffer);
				p_cmd_buf = cmd_buffer;
				break;

			case VT_KEY_END:
			    /* move cursor to end */
				CLI_VT_MoveRight(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);
				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				break;

			case VT_KEY_LEFT:
				if (p_cmd_buf > cmd_buffer)
				{
					p_cmd_buf--;
					CLI_VT_MoveLeft(1);
				}
				break;

			case VT_KEY_RIGHT:
				if (p_cmd_buf < (cmd_buffer + strlen(cmd_buffer)))
				{
					p_cmd_buf++;
					CLI_VT_MoveRight(1);
				}
				break;

			case VT_KEY_BS:
                /* if already to the head, do nothing */
				if (p_cmd_buf == cmd_buffer)
				{
                    break;
				}

				if (p_cmd_buf == (cmd_buffer + strlen(cmd_buffer)))
				{
					CLI_VT_Printf("\b \b");
					*(--p_cmd_buf) = '\0';
				}
				else
				{
					GT_UI8 *loop;

					p_cmd_buf--;
					CLI_VT_Printf("\b%s ", p_cmd_buf+1);
					CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

					for (loop = p_cmd_buf;
						 loop <= (cmd_buffer + strlen(cmd_buffer));
						 loop++)
					{
						*loop = *(loop+1);
					}
				}
				break;

			case VT_KEY_DEL:
				if (p_cmd_buf < (cmd_buffer + strlen(cmd_buffer)))
				{
					GT_UI8 *loop;

					CLI_VT_Printf("%s ", p_cmd_buf+1);
					CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

					for (loop = p_cmd_buf;
						 loop <= (cmd_buffer + strlen(cmd_buffer));
						 loop++)
					{
						*loop = *(loop+1);
					}
				}
				break;

			case VT_KEY_UP:  /* get the previous command */
			    /* move cursor to end */
				CLI_VT_MoveRight(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

                /* move to the end of this command */
				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				while (p_cmd_buf-- > cmd_buffer)
				{
					CLI_VT_Printf("\b \b");
				}

                /* get the prev. command history */
                p_history = CLI_HISTORY_GetPrevHistory();
                if (p_history == NULL)
                {
                    memset(cmd_buffer, 0x0, sizeof(cmd_buffer));
                }
                else
                {
                    strcpy(cmd_buffer, p_history);
                }

				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				CLI_VT_Printf("%s", cmd_buffer);
				break;

			case VT_KEY_DOWN:  /* get the next command */
			    /* move cursor to end */
				CLI_VT_MoveRight(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

                /* move to the end of this command, and clear the current command in screen */
				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				while (p_cmd_buf-- > cmd_buffer)
				{
					CLI_VT_Printf("\b \b");
				}

                /* get the next command history */
                p_history = CLI_HISTORY_GetNextHistory();
                if (p_history == NULL)
                {
                    memset(cmd_buffer, 0x0, sizeof(cmd_buffer));
                }
                else
                {
                    strcpy(cmd_buffer, p_history);
                }

				p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
				CLI_VT_Printf("%s", cmd_buffer);
				break;

			default:
				if ((key_value < 0x20) || (key_value > 0x7F))
				{
                    /* this is not a visible charactor */
                    break;
				}

				if (strlen(cmd_buffer) >= sizeof(cmd_buffer))
				{
                    /* command buffer full */
                    break;
				}

				if (p_cmd_buf == (cmd_buffer + strlen(cmd_buffer)))
				{
					*p_cmd_buf++ = (GT_UI8)key_value;
					*p_cmd_buf	 = '\0';
					CLI_VT_Printf("%c", (GT_UI8)key_value);
				}
				else
				{
					if (insertEnable == GT_TRUE) /* insert mode */
					{
						GT_UI8 *loop;

						CLI_VT_Printf("%c%s", (GT_UI8)key_value, p_cmd_buf);
						CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

						for (loop = (cmd_buffer + strlen(cmd_buffer));
							 loop > p_cmd_buf;
							 loop--)
						{
							*loop = *(loop-1);
						}
						*p_cmd_buf++ = (GT_UI8)key_value;
					}
					else /* overwrite mode */
					{
						*p_cmd_buf++ = (GT_UI8)key_value;
						CLI_VT_Printf("%c", (GT_UI8)key_value);
					}
				}
				break;
    		}
    	}

    	/* use tmp_buffer to parse, cause the parse function will modify command */
    	memcpy(tmp_buffer, cmd_buffer, sizeof(cmd_buffer));
    	n_param = CLI_PAR_ParseCmd(tmp_buffer, commandParams);
    	if (n_param == 0)
    	{
    		continue;
    	}

#if CLI_ENABLE_EXIT
		if (strcmp(commandParams[0], CLI_EXIT_CMD) == 0)
		{
			return;
		}
#endif

    	CLI_VT_Printf("\n\r");

    	cmd = CLI_REG_SearchCommand(commandParams[0]);
    	if (cmd == NULL)
    	{
    		CLI_VT_Printf("\n\rInvalid command, type '?' or 'help' to get help.\n\r");
    	}
    	else
    	{
    	    CLI_REG_CMD_RETURN_T    cmd_ret;

    	    if (cmd->cmd_exec != NULL)
    	    {
    	        /* execute this command */
    		    cmd_ret = (*cmd->cmd_exec)(n_param, (const GT_UI8 **)commandParams);
    	    }
    		else
    		{
    		    /* if this command doesn't have the execute function registered,
    		     * we print nothing.
    		     */
    		    cmd_ret = CLI_PRT_EMPTY;
    		}

    		switch (cmd_ret)
    		{
    		    case CLI_PRT_SUCCESS:
    		        CLI_VT_Printf("\n\rSuccess!\n\r");
    		        break;

    		    case CLI_PRT_FAIL:
    		        CLI_VT_Printf("\n\rFail!\n\r");
    		        break;

    		    case CLI_PRT_NONE:
    		        CLI_VT_Printf("\n\r");
    		        break;

                case CLI_PRT_EMPTY:
    		    default:
    		        break;
    		}
    	}

    	/* add this command into history */
        CLI_HISTORY_AddHistory(cmd_buffer);
    }

    /* never reach here */
    return;
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
GT_I32 CLI_IO_ReadKey(void)
{
    static GT_I32  lastkeys[2] = {-1, -1};
    GT_I32         i, key, key1;

    for (i = 0; i < sizeof(lastkeys)/sizeof(lastkeys[0]); i++)
    {
        if (lastkeys[i] != -1)
        {
            key = lastkeys[i];
            lastkeys[i] = -1;
            return key;
        }
    }

    key = CLI_VT_GetChar();
    if ((key == -1) || (key != VT_KEY_ESC))
    {
        return key;
    }

    /* wait '[' or 'O' */
	for (i=0; i<CLI_READKEY_TIMEOUT; i++)
	{
		key = CLI_VT_GetChar();
		if (key != -1)
		{
			break;
	    }
	}

    if (key == -1)
    {
        return VT_KEY_ESC;
    }

    switch (key)
    {
        case '[':
        case 'O':
            key1 = key;
            break;
        default:
            lastkeys[0] = key;
            return VT_KEY_ESC;
    }

    /* wait 'A/B/C/D' or 'PQRS' */
	for (i=0; i<CLI_READKEY_TIMEOUT; i++)
	{
		key = CLI_VT_GetChar();
		if (key != -1)
		{
			break;
		}
	}

    if (key == -1)
    {
        return VT_KEY_ESC;
    }

    switch (key1)
    {
        case  '[':
            switch (key)
            {
                case 'A':
                    key = VT_KEY_UP;
                    break;
                case 'B':
                    key = VT_KEY_DOWN;
                    break;
                case 'C':
                    key = VT_KEY_RIGHT;
                    break;
                case 'D':
                    key = VT_KEY_LEFT;
                    break;
				case 'H':
					key = VT_KEY_HOME;
					break;
				case 'K':
					key = VT_KEY_END;
					break;
				case '@':
					key = VT_KEY_INSERT;
					break;

                default:
                    lastkeys[0] = '[';
                    lastkeys[1] = key;
                    key = VT_KEY_ESC;
                    break;
            }
            break;

        case  'O':
            switch (key)
            {
                case 'P':
                    key = VT_KEY_F1;
                    break;
                case 'Q':
                    key = VT_KEY_F2;
                    break;
                case 'R':
                    key = VT_KEY_F3;
                    break;
                case 'S':
                    key = VT_KEY_F4;
                    break;

                default:
                    lastkeys[0] = 'O';
                    lastkeys[1] = key;
                    key = VT_KEY_ESC;
                    break;
            }
            break;

        default:
            break;
    }

    return key;
}


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      Read a line from console by user input.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      Press ESC/CTRL+C will quit.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CLI_IO_ReadLine
(
	IN  GT_UI32  buf_len,
	OUT GT_UI8  *cmd_buffer
)
{
    /* insert mode or not, default set to insert mode.
     *    = GT_TRUE  : insert mode;
     *    = GT_FALSE : overwrite mode.
     */
    GT_BOOL       insertEnable = GT_TRUE;

	GT_UI8		 *p_cmd_buf;
	GT_I32		  key_value;

	if ((buf_len == 0) || (cmd_buffer == NULL))
	{
		return GT_FALSE;
	}

	memset(cmd_buffer, 0x0, buf_len);
	p_cmd_buf = cmd_buffer;

	while (key_value = CLI_IO_ReadKey(),
	            (key_value != VT_KEY_CR) && (key_value != VT_KEY_LF))
	{
		switch (key_value)
		{
		case -1:  /* not a valid key */
			break;

		case VT_KEY_ESC:
		case VT_KEY_CTRL_C:
			/* quit */
			return GT_FALSE;

		case VT_KEY_INSERT:  /* set insert/overwrite mode */
			insertEnable = (insertEnable)? GT_FALSE : GT_TRUE;
			break;

		case VT_KEY_HOME:
            /* move cursor to head */
			CLI_VT_MoveLeft(p_cmd_buf - cmd_buffer);
			p_cmd_buf = cmd_buffer;
			break;

		case VT_KEY_END:
		    /* move cursor to end */
			CLI_VT_MoveRight(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);
			p_cmd_buf = cmd_buffer + strlen(cmd_buffer);
			break;

		case VT_KEY_LEFT:
			if (p_cmd_buf > cmd_buffer)
			{
				p_cmd_buf--;
				CLI_VT_MoveLeft(1);
			}
			break;

		case VT_KEY_RIGHT:
			if (p_cmd_buf < (cmd_buffer + strlen(cmd_buffer)))
			{
				p_cmd_buf++;
				CLI_VT_MoveRight(1);
			}
			break;

		case VT_KEY_BS:
            /* if already to the head, do nothing */
			if (p_cmd_buf == cmd_buffer)
			{
                break;
			}

			if (p_cmd_buf == (cmd_buffer + strlen(cmd_buffer)))
			{
				CLI_VT_Printf("\b \b");
				*(--p_cmd_buf) = '\0';
			}
			else
			{
				GT_UI8 *loop;

				p_cmd_buf--;
				CLI_VT_Printf("\b%s ", p_cmd_buf+1);
				CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

				for (loop = p_cmd_buf;
					 loop <= (cmd_buffer + strlen(cmd_buffer));
					 loop++)
				{
					*loop = *(loop+1);
				}
			}
			break;

		case VT_KEY_DEL:
			if (p_cmd_buf < (cmd_buffer + strlen(cmd_buffer)))
			{
				GT_UI8 *loop;

				CLI_VT_Printf("%s ", p_cmd_buf+1);
				CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

				for (loop = p_cmd_buf;
					 loop <= (cmd_buffer + strlen(cmd_buffer));
					 loop++)
				{
					*loop = *(loop+1);
				}
			}
			break;


		default:
			if ((key_value < 0x20) || (key_value > 0x7F))
			{
                /* this is not a visable charactor */
                break;
			}

			if (strlen(cmd_buffer) >= buf_len)
			{
                /* command buffer full */
                break;
			}

			if (p_cmd_buf == (cmd_buffer + strlen(cmd_buffer)))
			{
				*p_cmd_buf++ = (GT_UI8)key_value;
				*p_cmd_buf	 = '\0';
				CLI_VT_Printf("%c", (GT_UI8)key_value);
			}
			else
			{
				if (insertEnable == GT_TRUE) /* insert mode */
				{
					GT_UI8 *loop;

					CLI_VT_Printf("%c%s", (GT_UI8)key_value, p_cmd_buf);
					CLI_VT_MoveLeft(cmd_buffer + strlen(cmd_buffer) - p_cmd_buf);

					for (loop = (cmd_buffer + strlen(cmd_buffer));
						 loop > p_cmd_buf;
						 loop--)
					{
						*loop = *(loop-1);
					}
					*p_cmd_buf++ = (GT_UI8)key_value;
				}
				else /* overwrite mode */
				{
					*p_cmd_buf++ = (GT_UI8)key_value;
					CLI_VT_Printf("%c", (GT_UI8)key_value);
				}
			}
			break;
		}
	}

	return GT_TRUE;
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
void CLI_IO_Init(void)
{
    cli_io_Task();
}

