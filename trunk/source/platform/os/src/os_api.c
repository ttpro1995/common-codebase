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

#include <inc/os_api.h>
#include <inc/os_port.h>


void OS_API_FatalError(IN const void *fmt, ...)
{
    OS_API_Printf("\n\r----------------- OS FATAL ERROR --------------------\n\r");
    OS_API_Printf(fmt);
    OS_API_Printf("\n\r-----------------------------------------------------\n\r");

    /* dead here */
    while (1)
    {}
}

static void os_api_ST_PIT_Handler(void)
{
    extern void OSTickISR(void);

    OS_API_Printf("\n\r -- os_api_ST_PIT_Handler --");

    OSTickISR();
}

GT_BOOL OS_API_Init(void)
{
    OS_TASK_API_Init();
    OS_SEM_API_Init();
    OS_QUEUE_API_Init();
    OS_EVENT_API_Init();
    OS_POOL_API_Init();
    OS_BLOCK_API_Init();

    /* hook Periodic Interval Timer vector handler, and enable it */
    if (!CPU_INT_HookInterrupt(CPU_INT_SRC_ST_PIT,
                               CPU_INT_HIGHEST_AIC_PRIORITY,
                               CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                               os_api_ST_PIT_Handler)
        || !CPU_INT_SetInterrupt(CPU_INT_SRC_ST_PIT, GT_TRUE))
    {
        return GT_FALSE;
    }

    OS_PORT_Init();

    return GT_TRUE;
}


void OS_API_Start(void)
{
    OS_PORT_Start();
}

