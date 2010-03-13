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

#include <inc/cli_api.h>
#include <inc/os_api.h>


static void task_1(void *arg)
{
    while (1)
    {
        UTL_PRT_Printf("\n\r task 1 ... ");
        OS_TASK_API_Sleep(100);
    }
}

static void task_2(void *arg)
{
    while (1)
    {
        UTL_PRT_Printf("\n\r task 2 ... ");
        OS_TASK_API_Sleep(200);
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
static CLI_REG_CMD_RETURN_T cmd_os_osDebug
(
    IN const GT_UI32     n_param,
    IN const GT_UI8     *param[]
)
{
    GT_UI32  current_param = 1;

    if (n_param < 2)
    {
        UTL_PRT_Printf("\n\rMissing parameter!");
        return CLI_PRT_FAIL;
    }

    if (strcmp(param[current_param], (GT_UI8 *)"start") == 0)
    {
        GT_UI32 task_1_arg = 1;
        GT_UI32 task_2_arg = 2;

        UTL_PRT_Printf("\n\r OS_API_Init ...");
        OS_API_Init();

        UTL_PRT_Printf("\n\r OS_TASK_API_Create Task_1 ...");
        OS_TASK_API_Create((GT_UI8 *)"Task_1", task_1, &task_1_arg, KB(1), 6, GT_TRUE);

        UTL_PRT_Printf("\n\r OS_TASK_API_Create Task_2 ...");
        OS_TASK_API_Create((GT_UI8 *)"Task_2", task_2, &task_2_arg, KB(1), 5, GT_TRUE);

        UTL_PRT_Printf("\n\r OS_API_Start ...");
        OS_API_Start();

        /* never reach here */
        UTL_PRT_Printf("\n\rOS Start OK ...");
        return CLI_PRT_SUCCESS;
    }
    else
    {
        UTL_PRT_Printf("\n\rUnknown parameter!");
        return CLI_PRT_FAIL;
    }
}


static CLI_REG_CMD_T cmd_app[] =
{
    {(GT_UI8 *)"os",         cmd_os_osDebug,     (GT_UI8 *)"os [start] (os debug)"},
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
GT_BOOL CMD_OS_RegisterCmd(void)
{
    GT_UI32  loop;

    for (loop = 0; loop < sizeof(cmd_app)/sizeof(cmd_app[0]); loop++)
    {
        if (!CLI_REG_RegisterCmd(&cmd_app[loop]))
        {
            UTL_PRT_Printf("\n\r%s(line%d): Register Command %s Fail!",
                           __FILE__, __LINE__, cmd_app[loop].cmd_name);
        }
    }

    return GT_TRUE;
}

