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


#if 1

/* all OS sem control sems are here */
static OS_SEM_API_SCB  os_sem_SCB[OS_API_SEM_MAX_NUM];


static OS_SEM_API_SCB *os_sem_api_MallocSCB(void)
{
    const OS_SEM_API_SCB *entry_end = os_sem_SCB + sizeof(os_sem_SCB)/sizeof(os_sem_SCB[0]);
    OS_SEM_API_SCB       *p_SCB;

    for (p_SCB = os_sem_SCB; p_SCB < entry_end; p_SCB++)
    {
        if (p_SCB->sem_id == OS_SEM_API_INVALID_SCB)
        {
            p_SCB->sem_id = (p_SCB - os_sem_SCB);
            return p_SCB;
        }
    }

    return (OS_SEM_API_SCB *)(NULL);
}


static void os_sem_api_FreeSCB(IN OS_SEM_API_SCB *p_free_SCB)
{
    const OS_SEM_API_SCB *entry_end = os_sem_SCB + sizeof(os_sem_SCB)/sizeof(os_sem_SCB[0]);
    OS_SEM_API_SCB       *p_SCB;
    OS_SEM_API_SCB        SCB_bak;

    if ((p_free_SCB == NULL) || (p_free_SCB->sem_id == OS_SEM_API_INVALID_SCB))
    {
        return;
    }

    for (p_SCB = os_sem_SCB; p_SCB < entry_end; p_SCB++)
    {
        if (p_SCB->sem_id == p_free_SCB->sem_id)
        {
            memcpy(&SCB_bak, p_SCB, sizeof(*p_SCB));
            memset(p_SCB, 0x0, sizeof(*p_SCB));
            p_SCB->prev       = SCB_bak.prev;
            p_SCB->next       = SCB_bak.next;
            p_SCB->sem_id   = OS_SEM_API_INVALID_SCB;

            return;
        }
    }
}

#endif


/* always point to the first sem control sem entry */
static OS_SEM_API_SCB *p_sem_list  = NULL;

static GT_BOOL os_sem_api_CheckExist(IN const OS_SEM_API_SCB *p_check_SCB)
{
    OS_SEM_API_SCB       *p_SCB;

    if ((p_check_SCB == NULL)
        || (p_check_SCB->sem_id == OS_SEM_API_INVALID_SCB))
    {
        return GT_FALSE;
    }

    for (p_SCB = p_sem_list;
         ((p_SCB != NULL) && (p_SCB->next != NULL));
         p_SCB = p_SCB->next)
    {
        if ((p_SCB->sem_id == p_check_SCB->sem_id)
            && (p_SCB->sem_id != OS_SEM_API_INVALID_SCB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


void OS_SEM_API_Init(void)
{
    const OS_SEM_API_SCB *entry_end = os_sem_SCB + sizeof(os_sem_SCB)/sizeof(os_sem_SCB[0]);
    OS_SEM_API_SCB       *p_current_SCB;
    OS_SEM_API_SCB       *p_next_SCB;

    memset(os_sem_SCB, 0x0, sizeof(os_sem_SCB));

    /* init SCB buffer */
    for (p_current_SCB = os_sem_SCB;
         p_current_SCB < (entry_end - 1);
         p_current_SCB++)
    {
        p_next_SCB              = p_current_SCB + 1;
        p_current_SCB->next     = p_next_SCB;
        p_next_SCB->prev        = p_current_SCB;
        p_current_SCB->sem_id  = OS_SEM_API_INVALID_SCB;
    }

    if (!OS_SEM_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS sem fail!",
                          __FILE__, __LINE__);
    }
}

