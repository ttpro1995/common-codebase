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


GT_BOOL OS_TASK_PORT_Create(IN OS_TASK_API_TCB *p_TCB)
{
    if (p_TCB == NULL)
    {
        return GT_FALSE;
    }

    if (OS_NO_ERR != OSTaskCreate(p_TCB->entry,
                                  p_TCB->arg,
                                  p_TCB->stack_base,
                                  p_TCB->priority))
    {
        return GT_FALSE;
    }

    p_TCB->task_status = OS_TASK_API_STATE_RUNNING;

    return GT_TRUE;
}


GT_BOOL OS_TASK_PORT_Delete(IN const OS_TASK_API_TCB *p_TCB)
{
    if (p_TCB == NULL)
    {
        return GT_FALSE;
    }

    if (OS_NO_ERR != OSTaskDel(p_TCB->priority))
    {
        return GT_FALSE;
    }
    else
    {
        return GT_TRUE;
    }
}


GT_BOOL OS_TASK_PORT_Suspend(IN const OS_TASK_API_TCB *p_TCB)
{
    if (p_TCB == NULL)
    {
        return GT_FALSE;
    }

    if (OS_NO_ERR != OSTaskSuspend(p_TCB->priority))
    {
        return GT_FALSE;
    }
    else
    {
        return GT_TRUE;
    }
}


GT_BOOL OS_TASK_PORT_Resume(IN const OS_TASK_API_TCB *p_TCB)
{
    if (p_TCB == NULL)
    {
        return GT_FALSE;
    }

    if (OS_NO_ERR != OSTaskResume(p_TCB->priority))
    {
        return GT_FALSE;
    }
    else
    {
        return GT_TRUE;
    }
}


GT_BOOL OS_TASK_PORT_UpdateTCB(IN OUT OS_TASK_API_TCB *p_TCB)
{
    OS_TCB  os_TCB;

    if (p_TCB == NULL)
    {
        return GT_FALSE;
    }

    MEMSET(&os_TCB, 0x0, sizeof(os_TCB));

    if (OS_NO_ERR != OSTaskQuery(p_TCB->priority, &os_TCB))
    {
        return GT_FALSE;
    }

    if (os_TCB.OSTCBStat & OS_STAT_RDY)
    {
        p_TCB->task_status = OS_TASK_API_STATE_NOT_START;
    }
    else if ((os_TCB.OSTCBStat & OS_STAT_SEM)
                || (os_TCB.OSTCBStat & OS_STAT_MUTEX))
    {
        p_TCB->task_status = OS_TASK_API_STATE_WAIT_SEM;
        STRNCPY(p_TCB->wait_element_name, (const GT_UI8 *)"Unknown", sizeof(p_TCB->wait_element_name));
    }
    else if (os_TCB.OSTCBStat & OS_STAT_MBOX)
    {
        p_TCB->task_status = OS_TASK_API_STATE_WAIT_MBOX;
        STRNCPY(p_TCB->wait_element_name, (const GT_UI8 *)"Unknown", sizeof(p_TCB->wait_element_name));
    }
    else if (os_TCB.OSTCBStat & OS_STAT_Q)
    {
        p_TCB->task_status = OS_TASK_API_STATE_WAIT_QUEUE;
        STRNCPY(p_TCB->wait_element_name, (const GT_UI8 *)"Unknown", sizeof(p_TCB->wait_element_name));
    }
    else if (os_TCB.OSTCBStat & OS_STAT_FLAG)
    {
        p_TCB->task_status = OS_TASK_API_STATE_WAIT_EVENT;
        STRNCPY(p_TCB->wait_element_name, (const GT_UI8 *)"Unknown", sizeof(p_TCB->wait_element_name));
    }
    else if (os_TCB.OSTCBStat & OS_STAT_SUSPEND)
    {
        p_TCB->task_status = OS_TASK_API_STATE_SUSPEND;
    }

    p_TCB->task_time_counts = os_TCB.OSTCBCyclesTot;
    p_TCB->stack_base       = os_TCB.OSTCBStkBase;
    p_TCB->stack_used       = os_TCB.OSTCBStkUsed;

    return GT_TRUE;
}


void OS_TASK_PORT_Sleep(IN const GT_UI32 ticks)
{
    OSTimeDly(ticks);
}


GT_BOOL OS_TASK_PORT_Init(void)
{
    return GT_TRUE;
}

