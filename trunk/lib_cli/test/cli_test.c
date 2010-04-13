/******************************************************************************
 *
 * (C) Copyright 2007-2010
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 *   MA 02111-1307 USA
 *
 * FILE NAME:
 *   cli_test.c
 * DESCRIPTION:
 *   A test tool for CLI, based on win32 platform, windows XP.
 * HISTORY:
 *   2010.4.13        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#include "typedef.h"
#include "cli.h"


#define TEST_CLI_PROMPT    "TEST"

static CLI_REG_CMD_T _cmd_entry[];

static CLI_REG_CMD_RETURN_T cmd_entry_InternalTest
(
    IN const UINT32     n_param,
    IN const SINT8     *param[]
)
{
    NO_WARNING(n_param || param);

    printf("\n\r ------------  test start  ---------------");
    printf("\n\r  CLI Version         : %s", CLI_GetVersion());
    printf("\n\r  Set New Prompt      : %s", (CLI_IO_SetPrompt("TEST_NEW"), "TEST_NEW"));
    printf("\n\r  De-Register Command : %s", (CLI_REG_DeRegisterCmd(&_cmd_entry[0]), _cmd_entry[0].cmd_name));
    printf("\n\r ------------  test end    ---------------");

    return CLI_PRT_NONE;
}


static CLI_REG_CMD_T _cmd_entry[] =
{
    {"_test",    cmd_entry_InternalTest,       "_test (CLI Internal Test)"},
};


static BOOL _register_command(void)
{
    UINT32  loop;

    for (loop = 0; loop < COUNT_OF(_cmd_entry); loop++)
    {
        if (!CLI_REG_RegisterCmd(&_cmd_entry[loop]))
        {
            printf("\n\r Register Command %s Fail!", _cmd_entry[loop].cmd_name);
            return FALSE;
        }
    }

    return TRUE;
}


static void _init_VT(void)
{
    /* CLI init */
    CLI_Init();

    /* register test commands */
    _register_command();

    /* set CLI prompt */
    CLI_IO_SetPrompt(TEST_CLI_PROMPT);
}


int main(int argc, char *argv[])
{
    NO_WARNING(argc || argv);

    /* Init VT */
    _init_VT();

    /* start CLI, and never return */
    CLI_Start();

    /* never reach here */
    return 0;
}

