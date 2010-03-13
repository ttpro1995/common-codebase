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

#include <cli_api.h>

static GT_S8  _cli_prompt_buf[CLI_PROMPT_LEN + 2];

static const GT_S8  *_cmd_result_str[] =
{
    "",                     /* CLI_PRT_INVALID */
    "\n\r",                 /* CLI_PRT_NONE    */
    "\n\r Success!\n\r",    /* CLI_PRT_SUCCESS */
    "\n\r Fail!\n\r",       /* CLI_PRT_FAIL    */
};


static void _cli_io_MoveCursorLeft(void)
{
    GT_U8   x, y;

    CLI_VT_GetCursorPosition(&x, &y);

    if (x == 0)
    {
        if (y > 0)
        {
            x = CLI_SCREEN_WIDTH-1;
            y--;
        }
    }
    else
    {
        x--;
    }

    CLI_VT_SetCursorToXY(x, y);
}


static GT_BOOL _cli_io_GetCmd
(
   OUT GT_S8   *p_cmd_buf,
   IN  GT_U32   buf_len
)
{
	GT_S32          key_value;
    CLI_REG_CMD_S  *p_match_cmd;
	GT_S8          *p_history;
	GT_S8		   *p_cmd_buf_loop;

    /* insert mode or not, default set to insert mode.
     *    = GT_TRUE  : insert mode;
     *    = GT_FALSE : overwrite mode.
     */
    static GT_BOOL  insertEnable = GT_TRUE;

	memset(p_cmd_buf, 0x0, buf_len);
	p_cmd_buf_loop = p_cmd_buf;

	/* display prompt */
	CLI_VT_Printf("\n\r%s>", _cli_prompt_buf);

	while (key_value = CLI_VT_ReadKey(),
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
            p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
            CLI_VT_Printf("\n\r%s>%s", _cli_prompt_buf, p_cmd_buf);
            break;

		case VT_KEY_CTRL_Z:  /* clear current command line */
		case VT_KEY_CTRL_U:
			/* move cursor to end */
            while (p_cmd_buf_loop < p_cmd_buf + strlen(p_cmd_buf))
            {
                CLI_VT_Printf("%c", *p_cmd_buf_loop++);
            }

            /* clear the current command in screen */
			while (p_cmd_buf_loop > p_cmd_buf)
			{
                p_cmd_buf_loop--;
                _cli_io_MoveCursorLeft();
				CLI_VT_Printf(" ");
                _cli_io_MoveCursorLeft();
			}

			/* clear command line in buffer */
			memset(p_cmd_buf, 0x0, buf_len);
			p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
			break;

        case VT_KEY_TAB:
        {
            GT_U32  vRealCmdOffset;
            GT_U32  vMatchCount;

            p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);

            vRealCmdOffset = 0;
            if (strncmp(p_cmd_buf, CLI_HELP_STRING_1, strlen(CLI_HELP_STRING_1)) ==0)
            {
                vRealCmdOffset += strlen(CLI_HELP_STRING_1);
            }
            else if (strncmp(p_cmd_buf, CLI_HELP_STRING_2, strlen(CLI_HELP_STRING_2)) ==0)
            {
                vRealCmdOffset += strlen(CLI_HELP_STRING_2);
            }

            switch (CLI_REG_GetMatchCommandInfo(p_cmd_buf+vRealCmdOffset, p_cmd_buf+vRealCmdOffset))
            {
                case 0:
                case 1:
				    CLI_VT_Printf("%s", p_cmd_buf_loop);
                    break;

                default:
                    /* display all matched commands */
                    p_match_cmd = NULL;
                    vMatchCount = 0;
                    while ((p_match_cmd = CLI_REG_GetNextMatchCommand(p_match_cmd, p_cmd_buf+vRealCmdOffset)) != NULL)
                    {
                        if (vMatchCount % 4 == 0)
                        {
                            CLI_VT_Printf("\n\r");
                        }

    				    CLI_VT_Printf(" %-18s", p_match_cmd->cmd_name);
                        vMatchCount++;
                    }

                    CLI_VT_Printf("\n\r%s>%s", _cli_prompt_buf, p_cmd_buf);
                    break;
            }

            p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
        }
            break;

		case VT_KEY_HOME:
            /* move cursor to head */
            while (p_cmd_buf_loop > p_cmd_buf)
            {
                p_cmd_buf_loop--;
                _cli_io_MoveCursorLeft();
            }
			break;

		case VT_KEY_END:
		    /* move cursor to end */
            while (p_cmd_buf_loop < p_cmd_buf + strlen(p_cmd_buf))
            {
                CLI_VT_Printf("%c", *p_cmd_buf_loop++);
            }
			p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
			break;

		case VT_KEY_LEFT:
			if (p_cmd_buf_loop > p_cmd_buf)
			{
				p_cmd_buf_loop--;
                _cli_io_MoveCursorLeft();
			}
			break;

		case VT_KEY_RIGHT:
			if (p_cmd_buf_loop < (p_cmd_buf + strlen(p_cmd_buf)))
			{
                CLI_VT_Printf("%c", *p_cmd_buf_loop++);
			}
			break;

		case VT_KEY_BS:
            /* if already to the head, do nothing */
			if (p_cmd_buf_loop == p_cmd_buf)
			{
                break;
			}

			if (p_cmd_buf_loop == (p_cmd_buf + strlen(p_cmd_buf)))
			{
                _cli_io_MoveCursorLeft();
                CLI_VT_Printf(" ");
                _cli_io_MoveCursorLeft();

				*(--p_cmd_buf_loop) = '\0';
			}
			else
			{
				GT_S8 *loop;

				p_cmd_buf_loop--;
                _cli_io_MoveCursorLeft();
				CLI_VT_Printf("%s ", p_cmd_buf_loop+1);

				for (loop = p_cmd_buf_loop;
					 loop <= (p_cmd_buf + strlen(p_cmd_buf));
					 loop++)
				{
					*loop = *(loop+1);
                    _cli_io_MoveCursorLeft();
				}
			}
			break;

		case VT_KEY_DEL:
			if (p_cmd_buf_loop < (p_cmd_buf + strlen(p_cmd_buf)))
			{
				GT_S8 *loop;

				CLI_VT_Printf("%s ", p_cmd_buf_loop+1);

				for (loop = p_cmd_buf_loop;
					 loop <= (p_cmd_buf + strlen(p_cmd_buf));
					 loop++)
				{
					*loop = *(loop+1);
                    _cli_io_MoveCursorLeft();
				}
			}
			break;

		case VT_KEY_UP:  /* get the previous command */
		    /* move cursor to end */
            while (p_cmd_buf_loop < p_cmd_buf + strlen(p_cmd_buf))
            {
                CLI_VT_Printf("%c", *p_cmd_buf_loop++);
            }

            /* clear the current command in screen */
			while (p_cmd_buf_loop > p_cmd_buf)
			{
                p_cmd_buf_loop--;

                _cli_io_MoveCursorLeft();
                CLI_VT_Printf(" ");
                _cli_io_MoveCursorLeft();
			}

            /* get the prev. command history */
            p_history = CLI_HISTORY_GetPrevHistory();
            if (p_history == NULL)
            {
                memset(p_cmd_buf, 0x0, buf_len);
            }
            else
            {
                strcpy(p_cmd_buf, p_history);
            }

			p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
			CLI_VT_Printf("%s", p_cmd_buf);
			break;

		case VT_KEY_DOWN:  /* get the next command */
		    /* move cursor to end */
            while (p_cmd_buf_loop < p_cmd_buf + strlen(p_cmd_buf))
            {
                CLI_VT_Printf("%c", *p_cmd_buf_loop++);
            }

            /* clear the current command in screen */
			while (p_cmd_buf_loop > p_cmd_buf)
			{
                p_cmd_buf_loop--;

                _cli_io_MoveCursorLeft();
                CLI_VT_Printf(" ");
                _cli_io_MoveCursorLeft();
			}

            /* get the next command history */
            p_history = CLI_HISTORY_GetNextHistory();
            if (p_history == NULL)
            {
                memset(p_cmd_buf, 0x0, buf_len);
            }
            else
            {
                strcpy(p_cmd_buf, p_history);
            }

			p_cmd_buf_loop = p_cmd_buf + strlen(p_cmd_buf);
			CLI_VT_Printf("%s", p_cmd_buf);
			break;

		default:
			if ((key_value < 0x20) || (key_value > 0x7F))
			{
                /* this is not a visible charactor */
                break;
			}

			if (strlen(p_cmd_buf) >= buf_len)
			{
                /* command buffer full */
                break;
			}

			if (p_cmd_buf_loop == (p_cmd_buf + strlen(p_cmd_buf)))
			{
				*p_cmd_buf_loop++ = (GT_U8)key_value;
				*p_cmd_buf_loop	 = '\0';
				CLI_VT_Printf("%c", (GT_U8)key_value);
			}
			else
			{
				if (insertEnable == GT_TRUE) /* insert mode */
				{
					GT_S8 *loop;

					CLI_VT_Printf("%c%s", (GT_U8)key_value, p_cmd_buf_loop);

					for (loop = (p_cmd_buf + strlen(p_cmd_buf));
						 loop > p_cmd_buf_loop;
						 loop--)
					{
						*loop = *(loop-1);
                        _cli_io_MoveCursorLeft();
					}
					*p_cmd_buf_loop++ = (GT_U8)key_value;
				}
				else /* overwrite mode */
				{
					*p_cmd_buf_loop++ = (GT_U8)key_value;
					CLI_VT_Printf("%c", (GT_U8)key_value);
				}
			}
			break;
		}
	}

    return GT_TRUE;
}


#if CLI_ENABLE_DYNAMIC_DISPLAY

static void _cli_io_DynamicDisplay
(
    IN GT_S8          *p_cmd_buf,
    IN CLI_REG_CMD_S  *p_cmd,
    IN GT_S8         **p_command_params,
    IN GT_U32          n_param,
    IN GT_U32          n_exec_times     /* if =0, means dynamic display forever */
)
{
    GT_BOOL                 bBreak = GT_FALSE;
	GT_S32                  key_value;
    CLI_REG_CMD_RETURN_E    cmd_ret;
    GT_U8                   x, y;
    GT_BOOL                 bLoopForever;

    if (p_cmd->cmd_exec != NULL)
    {
        /* clear screen */
        CLI_VT_ClearScreen();

    	/* display command */
    	CLI_VT_Printf("\n\r%s>%s\n\r", _cli_prompt_buf, p_cmd_buf);
        CLI_VT_GetCursorPosition(&x, &y);

        bLoopForever = (n_exec_times == 0)? GT_TRUE : GT_FALSE;

        while (bLoopForever || (n_exec_times-- > 0))
        {
            /* restore cursor to default start point */
            CLI_VT_SetCursorToXY(x, y);

            /* execute this command */
    	    cmd_ret = (*p_cmd->cmd_exec)(n_param, (const GT_S8 **)p_command_params);

            if (cmd_ret < COUNT_OF(_cmd_result_str))
            {
                CLI_VT_Printf(_cmd_result_str[cmd_ret]);
            }
            else
            {
                CLI_VT_Printf("\n\r Invalid Command Result %d!", cmd_ret);
            }

            /* display status bar */
            CLI_VT_InverseScreenColor();
            CLI_VT_Printf("\n\r  ESC / CTRL+C / q : Exit       r / SPACE : Refresh       Remain: %-5d        ", n_exec_times);
            CLI_VT_InverseScreenColor();

            /* check refresh/exit */
            {
                GT_BOOL     bRefresh = GT_FALSE;

                key_value = CLI_VT_ReadKey();

                switch (key_value)
                {
                    case VT_KEY_ESC:
                    case VT_KEY_CTRL_C:
                    case 'q':
                    case 'Q':
                        bBreak = GT_TRUE;

                    case VT_KEY_SPACE:
                    case 'r':
                    case 'R':
                        bRefresh = GT_TRUE;
                        break;

                    default:
                        break;
                }

                if (bBreak)
                {
                    break;
                }

                if (!bRefresh)
                {
                    /* delay */
                    CLI_VT_DelayMs(300);
                }
            }
        }

    /* empty the status line */
    {
        GT_U8   x, y;

        CLI_VT_GetCursorPosition(&x, &y);
        CLI_VT_SetCursorToXY(0, y);
        CLI_VT_Printf("%*s", CLI_SCREEN_WIDTH, " ");
        CLI_VT_SetCursorToXY(0, y);
    }
    }
	else
	{
	    /* if this command doesn't have the execute function registered,
	     * we print nothing.
	     */
	    CLI_VT_Printf("\n\r No execuable function for this command!");
	}
}

#endif


static void _cli_io_StaticDisplay
(
    IN CLI_REG_CMD_S  *p_cmd,
    IN GT_S8         **p_command_params,
    IN GT_U32          n_param
)
{
    CLI_REG_CMD_RETURN_E    cmd_ret;

    if (p_cmd->cmd_exec != NULL)
    {
        /* execute this command */
	    cmd_ret = (*p_cmd->cmd_exec)(n_param, (const GT_S8 **)p_command_params);

        if (cmd_ret < COUNT_OF(_cmd_result_str))
        {
            CLI_VT_Printf(_cmd_result_str[cmd_ret]);
        }
        else
        {
            CLI_VT_Printf("\n\r Invalid Command Result %d!", cmd_ret);
        }
    }
	else
	{
	    /* if this command doesn't have the execute function registered,
	     * we print nothing.
	     */
	    CLI_VT_Printf("\n\r No execuable function for this command!");
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
static void cli_io_Task(void)
{
	GT_U32		    n_param;
	CLI_REG_CMD_S  *cmd = NULL;

    GT_S8	        cmd_buffer[CLI_MAX_CMD_LEN+1];  /* command buffer      */
	GT_S8		    tmp_buffer[CLI_MAX_CMD_LEN+1];  /* temp command buffer */
    GT_S8	       *commandParams[CLI_MAX_PARAM];

#if CLI_ENABLE_DYNAMIC_DISPLAY
    GT_BOOL         b_dynamic_display;
    GT_U32          n_dynamic_time;
#endif

    while (1)
    {
        /* get command from user */
        if (!_cli_io_GetCmd(cmd_buffer, sizeof(cmd_buffer)))
        {
            continue;
        }

    	/* parse command:
    	 *   use tmp_buffer to parse,
    	 *   cause the command buffer will be modified by parser.
    	 */
    	memcpy(tmp_buffer, cmd_buffer, sizeof(cmd_buffer));
    	n_param = CLI_PAR_ParseCmd(tmp_buffer, commandParams);
    	if (n_param == 0)
    	{
    		continue;
    	}

#if CLI_ENABLE_EXIT
        /* check need to exit */
		if (strcmp(commandParams[0], CLI_EXIT_CMD) == 0)
		{
			break;
		}
#endif

    	CLI_VT_Printf("\n\r");

        /* execute command */
#if CLI_ENABLE_DYNAMIC_DISPLAY
        if (strncmp(commandParams[n_param-1],
                    CLI_DYNAMIC_DISPLAY_KEYWORD,
                    strlen(CLI_DYNAMIC_DISPLAY_KEYWORD)) == 0)
        {
            b_dynamic_display = GT_TRUE;
            n_dynamic_time = ATOI(commandParams[n_param-1] + strlen(CLI_DYNAMIC_DISPLAY_KEYWORD));
        }
        else
        {
            b_dynamic_display = GT_FALSE;
            n_dynamic_time = 0;
        }
#endif

        cmd = CLI_REG_SearchCommand(commandParams[0]);
    	if (cmd == NULL)
    	{
    		CLI_VT_Printf("\n\r Invalid command, type '?' or 'help' to get help!\n\r");
    	}
        else
        {
#if CLI_ENABLE_DYNAMIC_DISPLAY
            if (b_dynamic_display)
            {
                _cli_io_DynamicDisplay(cmd_buffer, cmd, &commandParams[0],
                                       n_param-1, n_dynamic_time);
            }
            else
#endif
            {
                _cli_io_StaticDisplay(cmd, &commandParams[0], n_param);
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
GT_BOOL CLI_IO_SetPrompt(IN const GT_S8  *pCliPrompt)
{
    if (pCliPrompt != NULL)
    {
        if (strlen(pCliPrompt) <= CLI_PROMPT_LEN)
        {
            strcpy(_cli_prompt_buf, pCliPrompt);
            return GT_TRUE;
        }
    }

    return GT_FALSE;
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
    /* set default CLI prompt to empty */
    strcpy(_cli_prompt_buf, "");
}


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
void CLI_IO_Start(void)
{
    cli_io_Task();
}

