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


/* this malloc method will be updated later.
 * By Panda Xiong, 2007.12.25
 */
#if 1

/* all tasks stacks are here */
static GT_UI32  os_task_stack[OS_API_TASK_MAX_NUM][OS_API_TASK_MAX_STACK_SIZE];

/* all OS element control blocks are here */
static OS_TASK_API_TCB  os_task_TCB[OS_API_TASK_MAX_NUM];


static OS_TASK_API_TCB *os_task_api_MallocTCB(IN const GT_UI32 task_stack_size)
{
    const OS_TASK_API_TCB *entry_end = os_task_TCB + sizeof(os_task_TCB)/sizeof(os_task_TCB[0]);
    OS_TASK_API_TCB       *p_TCB;

    NO_WARNING(task_stack_size);

    for (p_TCB = os_task_TCB; p_TCB < entry_end; p_TCB++)
    {
        if (p_TCB->task_id == OS_TASK_API_INVALID_TCB)
        {
            p_TCB->task_id = (p_TCB - os_task_TCB);
            return p_TCB;
        }
    }

    return (OS_TASK_API_TCB *)(NULL);
}

static void os_task_api_FreeTCB(IN OS_TASK_API_TCB *p_free_TCB)
{
    const OS_TASK_API_TCB *entry_end = os_task_TCB + sizeof(os_task_TCB)/sizeof(os_task_TCB[0]);
    OS_TASK_API_TCB       *p_TCB;
    OS_TASK_API_TCB        TCB_bak;

    if ((p_free_TCB == NULL)
        || (p_free_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return;
    }

    for (p_TCB = os_task_TCB; p_TCB < entry_end; p_TCB++)
    {
        if (p_TCB->task_id == p_free_TCB->task_id)
        {
            MEMCPY(&TCB_bak, p_TCB, sizeof(*p_TCB));
            MEMSET(p_TCB, 0x0, sizeof(*p_TCB));
            p_TCB->prev       = TCB_bak.prev;
            p_TCB->next       = TCB_bak.next;
            p_TCB->stack_base = TCB_bak.stack_base;
            p_TCB->task_id    = OS_TASK_API_INVALID_TCB;

            return;
        }
    }
}

#endif


/* always point to the first task control block entry */
static OS_TASK_API_TCB *p_task_list  = NULL;

static GT_BOOL os_task_api_CheckExist(IN const OS_TASK_API_TCB *p_check_TCB)
{
    OS_TASK_API_TCB       *p_TCB;

    if ((p_check_TCB == NULL)
        || (p_check_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return GT_FALSE;
    }

    for (p_TCB = p_task_list;
         ((p_TCB != NULL) && (p_TCB->next != NULL));
         p_TCB = p_TCB->next)
    {
        if ((p_TCB->task_id == p_check_TCB->task_id)
            && (p_TCB->task_id != OS_TASK_API_INVALID_TCB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


OS_API_HANDLE OS_TASK_API_Create
(
    IN const GT_UI8     *task_name,
    IN OS_TASK_API_ENTRY entry,
	IN const void       *arg,
	IN const GT_UI32     stack_size,
	IN const GT_UI32     priority,
	IN const GT_BOOL     b_start
)
{
    OS_TASK_API_TCB     *p_TCB;

    if ((entry == NULL) || (stack_size == 0))
    {
        return (OS_API_HANDLE)(NULL);
    }

    for (p_TCB = p_task_list;
         ((p_TCB != NULL) && (p_TCB->next != NULL));
         p_TCB++)
    {
        if ((p_TCB->entry == entry)
            && (p_TCB->task_id != OS_TASK_API_INVALID_TCB))
        {
            OS_API_FatalError("%s(line%d): Task '%s' Exists, create Task fail!",
                              __FILE__, __LINE__,
                              p_TCB->name);
        }
    }

    p_TCB = os_task_api_MallocTCB(stack_size);
    if (p_TCB == NULL)
    {
        OS_API_FatalError("%s(line%d): Malloc Task '%s' TCB fail!",
                          __FILE__, __LINE__,
                          ((task_name == NULL) ? (GT_UI8 *)" " : task_name));
    }

    if (p_task_list == NULL)
    {
        p_task_list = p_TCB;
    }

    if (task_name != NULL)
    {
        STRNCPY(p_TCB->name, task_name, sizeof(*(p_TCB->name)));
    }

    p_TCB->entry       = entry;
    p_TCB->arg         = (void *)arg;
    p_TCB->priority    = priority;
    p_TCB->stack_size  = stack_size;
    p_TCB->b_start     = b_start;

    /* set task not start flag, this will be updated in OS_TASK_PORT_Create.
     * cause some RTOS don't support create a task without start it,
     * e.g: uC/OS-II.
     */
    p_TCB->task_status = OS_TASK_API_STATE_NOT_START;

    if (!OS_TASK_PORT_Create(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Create Task '%s' fail!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    return p_TCB;
}


void OS_TASK_API_Delete(IN OS_TASK_API_TCB *p_TCB)
{
    if ((p_TCB == NULL) || (p_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return;
    }

    if (!os_task_api_CheckExist(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Delete Task '%s' fail, task does not exist!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    if (!OS_TASK_PORT_Delete(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Delete Task '%s' fail!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    os_task_api_FreeTCB(p_TCB);
}


void OS_TASK_API_Suspend(IN OS_TASK_API_TCB *p_TCB)
{
    if ((p_TCB == NULL) || (p_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return;
    }

    if (!os_task_api_CheckExist(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Suspend Task '%s' fail, task does not exist!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    if ((p_TCB->task_status == OS_TASK_API_STATE_NOT_START)
        || (p_TCB->task_status == OS_TASK_API_STATE_SUSPEND))
    {
        return;
    }

    if (!OS_TASK_PORT_Suspend(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Suspend Task '%s' fail!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    p_TCB->task_status = OS_TASK_API_STATE_SUSPEND;
}


void OS_TASK_API_Resume(IN OS_TASK_API_TCB *p_TCB)
{
    if ((p_TCB == NULL) || (p_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return;
    }

    if (!os_task_api_CheckExist(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Resume Task '%s' fail, task does not exist!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    if (p_TCB->task_status == OS_TASK_API_STATE_SUSPEND)
    {
        if (!OS_TASK_PORT_Resume(p_TCB))
        {
            OS_API_FatalError("%s(line%d): Resume Task '%s' fail!",
                              __FILE__, __LINE__,
                              p_TCB->name);
        }

        /* this will be updated later */
        p_TCB->task_status = OS_TASK_API_STATE_RUNNING;
    }
}


GT_BOOL OS_TASK_API_GetTCB(IN OUT OS_TASK_API_TCB *p_TCB)
{
    if ((p_TCB == NULL) || (p_TCB->task_id == OS_TASK_API_INVALID_TCB))
    {
        return GT_FALSE;
    }

    if (!os_task_api_CheckExist(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Get Task '%s' TCB fail, task does not exist!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    if (!OS_TASK_PORT_UpdateTCB(p_TCB))
    {
        OS_API_FatalError("%s(line%d): Update Task '%s' TCB fail!",
                          __FILE__, __LINE__,
                          p_TCB->name);
    }

    return GT_TRUE;
}


GT_BOOL OS_TASK_API_GetNextTCB(IN OUT OS_TASK_API_TCB *p_get_TCB)
{
    OS_TASK_API_TCB  *p_TCB;

    if (p_get_TCB == NULL)
    {
        return GT_FALSE;
    }

    if ((p_get_TCB->task_id == OS_TASK_API_INVALID_TCB)
        || (p_get_TCB->entry == NULL))
    {
        if (p_task_list == NULL)
        {
            return GT_FALSE;
        }

        p_TCB = p_task_list;
        if (!OS_TASK_PORT_UpdateTCB(p_TCB))
        {
            OS_API_FatalError("%s(line%d): Update Task '%s' TCB fail!",
                              __FILE__, __LINE__,
                              p_TCB->name);
        }
    }
    else
    {
        p_TCB = p_get_TCB->next;

        if (!os_task_api_CheckExist(p_TCB))
        {
            return GT_FALSE;
        }

        if (!OS_TASK_PORT_UpdateTCB(p_TCB))
        {
            OS_API_FatalError("%s(line%d): Update Task '%s' TCB fail!",
                              __FILE__, __LINE__,
                              p_TCB->name);
        }
    }

    return GT_TRUE;
}


void OS_TASK_API_Sleep(IN const GT_UI32 sleep_ms)
{
    GT_UI32     ticks;

    if (sleep_ms == 0)
    {
        return;
    }

    /* traslate ms to OS tick */
    if (sleep_ms == 0)
    {
        return;
    }
    else if (sleep_ms < OS_API_PER_TICK_TIME)
    {
        ticks = 1;
    }
    else
    {
        ticks = (sleep_ms + OS_API_PER_TICK_TIME - 1) / OS_API_PER_TICK_TIME;
    }

    OS_TASK_PORT_Sleep(ticks);
}


void OS_TASK_API_Init(void)
{
    const OS_TASK_API_TCB *entry_end = os_task_TCB + sizeof(os_task_TCB)/sizeof(os_task_TCB[0]);
    OS_TASK_API_TCB       *p_current_TCB;
    OS_TASK_API_TCB       *p_next_TCB;

    MEMSET(os_task_stack, 0x0, sizeof(os_task_stack));
    MEMSET(os_task_TCB,   0x0, sizeof(os_task_TCB));

    /* init TCB buffer */
    for (p_current_TCB = os_task_TCB;
         p_current_TCB < (entry_end - 1);
         p_current_TCB++)
    {
        p_next_TCB             = p_current_TCB + 1;
        p_current_TCB->next    = p_next_TCB;
        p_next_TCB->prev       = p_current_TCB;
        p_current_TCB->task_id = OS_TASK_API_INVALID_TCB;

        p_current_TCB->stack_base = &(os_task_stack[p_current_TCB - os_task_TCB][OS_API_TASK_MAX_STACK_SIZE - 1]);
    }

    if (!OS_TASK_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS Task fail!",
                          __FILE__, __LINE__);
    }
}

