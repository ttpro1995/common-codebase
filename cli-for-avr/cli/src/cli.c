/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    None
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#include <cli.h>

/* User command pre-define, stored in ROM */
const CLI_CMD_T cli_cmd[] IN_ROM = CLI_API_COMMANDS;

const static UI8_T cli_prompt[]      IN_ROM = "\n\r>";
const static UI8_T cli_cmd_unknown[] IN_ROM = "\n\rUnknown command!";

#define CLI_BS_ASCII    ((const UI8_T *)"\b \b")


/*******************************************************************************
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
*******************************************************************************/
UI8_T cli_ExecuteCmd(IN const CLI_CMD_INDEX_T index)
{
    CLI_CMD_EXEC_T  p_exec_func;

#ifdef __CPU_AVR__

    CLI_CMD_T       cmd;

    MEMSET(&cmd, 0x0, sizeof(cmd));
    P_MEMCPY(&cmd, (ROM_VOID_T)&cli_cmd[index-1], sizeof(CLI_CMD_T));
    p_exec_func = cmd.cmd_exec;

#else

    p_exec_func = cli_cmd[index-1].cmd_exec;

#endif

    return (*p_exec_func)();
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      p_cmd : search command buffer, must ended with '\0'
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
CLI_CMD_INDEX_T cli_SearchCmd(IN const UI8_T *start_addr)
{

#ifdef __CPU_AVR__

    const CLI_CMD_INDEX_T total_index = (sizeof(cli_cmd) / sizeof(cli_cmd[0]));
    CLI_CMD_INDEX_T  index;
    CLI_CMD_T        cmd;

    MEMSET(&cmd, 0x0, sizeof(cmd));

    if (POINTER_CHECK(start_addr))
    {
        return 0;
    }

    for (index = 1; index <= total_index; index++)
    {
        MEMSET(&cmd, 0x0, sizeof(cmd));
        P_MEMCPY(&cmd, (ROM_VOID_T)&cli_cmd[index-1], sizeof(cli_cmd[0]));

        if (STRCMP(cmd.cmd, start_addr) == 0)
        {
            return index;
        }
    }

#else

    const CLI_CMD_INDEX_T total_index = (sizeof(cli_cmd) / sizeof(cli_cmd[0]));
    CLI_CMD_INDEX_T  index;
    CLI_CMD_T       *p_cmd = cli_cmd;

    if (POINTER_CHECK(start_addr))
    {
        return 0;
    }

    for (index = 1; index <= total_index; index++, p_cmd++)
    {
        /* need to update later */
        if (STRCMP(p_cmd->cmd, start_addr) == 0)
        {
            return index;
        }
    }

#endif

    /* not found command */
    return 0;
}


/*******************************************************************************
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
*******************************************************************************/
static void cli_MainProcess(void)
{
    UI8_T           key_value;
    UI8_T           cmd_buffer[CLI_CMD_MAX_LEN + 1];
    CLI_CMD_INDEX_T cmd_index;
    UI8_T          *p_cmd_buf;

    /* display prompt message */
#ifdef __CPU_AVR__
    MEMSET(cmd_buffer, 0x0, sizeof(cmd_buffer));
    P_MEMCPY(cmd_buffer, (ROM_VOID_T)cli_prompt, sizeof(cli_prompt));
    UTL_VT_PutString(cmd_buffer);
#else
    UTL_VT_PutString(cli_prompt);
#endif

    MEMSET(cmd_buffer, 0x0, sizeof(cmd_buffer));
    p_cmd_buf = cmd_buffer;

    while ((key_value = UTL_VT_ReadKey()) != KEY_ENTER)
    {
        switch (key_value)
        {
            case KEY_BS:
                if (p_cmd_buf != cmd_buffer)
                {
                    *(--p_cmd_buf) = '\0';
                    UTL_VT_PutString(CLI_BS_ASCII);
                }
                break;

            default:
                if ((key_value >= 0x20) && (key_value <= 0x7F))
                {
                    /* not to the end of buffer */
                    if (p_cmd_buf != (cmd_buffer + CLI_CMD_MAX_LEN))
                    {
                        *p_cmd_buf++ = key_value;
                        *p_cmd_buf   = '\0';
                        UTL_VT_PutAscii(&key_value, 1);
                    }
                }
                break;
        }
    }

    /* No command input */
    if (cmd_buffer[0] == '\0')
    {
        return;
    }

    if ((cmd_index = cli_SearchCmd(cmd_buffer)) != 0)
    {
        /* found command, and execute the input command */
        cli_ExecuteCmd(cmd_index);
    }
    else
    {
        /* not found this command */
      #ifdef __CPU_AVR__
        MEMSET(cmd_buffer, 0x0, sizeof(cmd_buffer));
        P_MEMCPY(cmd_buffer, (ROM_VOID_T)cli_cmd_unknown, sizeof(cli_cmd_unknown));
        UTL_VT_PutString(cmd_buffer);
      #else
        UTL_VT_PutString(cli_cmd_unknown);
      #endif
    }
}


/*******************************************************************************
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
*******************************************************************************/
void CLI_Init(void)
{
    /* clear screen */
    UTL_VT_ClearScreen();

    while (1)
    {
        cli_MainProcess();
    }
}


/*******************************************************************************
 * CLI Kernel reserved commands
*******************************************************************************/

const static UI8_T cli_cmd_help_prompt[] IN_ROM = "\n\rAll commands:";

/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_Help(void)
{

#ifdef __CPU_AVR__

    const CLI_CMD_INDEX_T total_index = (sizeof(cli_cmd) / sizeof(cli_cmd[0]));
    UI8_T           cmd[CLI_CMD_MAX_LEN + 1];
    CLI_CMD_INDEX_T index;

    FUNCTION_BEGIN("CLI_CMD_Help");

    MEMSET(cmd, 0x0, sizeof(cmd));
    P_MEMCPY(cmd, (ROM_VOID_T)cli_cmd_help_prompt, sizeof(cmd));
    UTL_VT_PutString(cmd);

    for (index = 1; index <= total_index; index++)
    {
        MEMSET(cmd, 0x0, sizeof(cmd));
        P_MEMCPY(cmd, (ROM_VOID_T)cli_cmd[index-1].cmd, sizeof(cmd));
        UTL_VT_PutString("\n\r ");
        UTL_VT_PutString(cmd);
    }

#else

    CLI_CMD_T      *p_cmd = cli_cmd;
    CLI_CMD_INDEX_T index;
    const CLI_CMD_INDEX_T total_index = (sizeof(cli_cmd) / sizeof(cli_cmd[0]));

    FUNCTION_BEGIN("CLI_CMD_Help");

    UTL_VT_PutString(cli_cmd_help_prompt);

    for (index = 1; index <= total_index; index++, p_cmd++)
    {
        UTL_VT_PutString("\n\r ");
        UTL_VT_PutString(p_cmd->cmd);
    }

#endif

    return 0;
}


/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_ClearScreen(void)
{
    FUNCTION_BEGIN("CLI_CMD_ClearScreen");

    UTL_VT_ClearScreen();

    return 0;
}



const static UI8_T cli_cmd_showboard_pre[] IN_ROM =
    "\n\r"
    "\n\r *****************************"
#ifdef __CPU_AVR__
    "\n\r   Project   : CLI for AVR"
    "\n\r   CPU Type  : ATMega16L-8PI";
#else
    "\n\r   Project   : CLI for Common"
    "\n\r   CPU Type  : Unknown";
#endif

const static UI8_T cli_cmd_showboard_end[] IN_ROM =
    "\n\r"
    "\n\r   Author    : Panda Xiong"
    "\n\r   Date      : 2007.03.15"
    "\n\r *****************************"
    "\n\r";

/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_ShowBoard(void)
{
    UI8_T   cmd[sizeof(cli_cmd_showboard_end) + 1];

    FUNCTION_BEGIN("CLI_CMD_ShowBoard");

    MEMSET(cmd, 0x0, sizeof(cmd));
    P_MEMCPY(cmd, (ROM_VOID_T)cli_cmd_showboard_pre, sizeof(cmd));
    UTL_VT_PutString(cmd);

#if USE_LIB_PRINTF
   printf("\n\r   Frequency : %d MHz", (UI8_T)(F_CPU/1000/1000));
   printf("\n\r   BaudRate  : %u bps", (UI16_T)(UART_BAUDRATE));
#endif

    MEMSET(cmd, 0x0, sizeof(cmd));
    P_MEMCPY(cmd, (ROM_VOID_T)cli_cmd_showboard_end, sizeof(cmd));
    UTL_VT_PutString(cmd);

    return 0;
}

