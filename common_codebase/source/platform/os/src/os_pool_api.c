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

/* all OS pool control pools are here */
static OS_POOL_API_PCB  os_pool_PCB[OS_API_POOL_MAX_NUM];


static OS_POOL_API_PCB *os_pool_api_MallocPCB(void)
{
    const OS_POOL_API_PCB *entry_end = os_pool_PCB + sizeof(os_pool_PCB)/sizeof(os_pool_PCB[0]);
    OS_POOL_API_PCB       *p_PCB;

    for (p_PCB = os_pool_PCB; p_PCB < entry_end; p_PCB++)
    {
        if (p_PCB->pool_id == OS_POOL_API_INVALID_PCB)
        {
            p_PCB->pool_id = (p_PCB - os_pool_PCB);
            return p_PCB;
        }
    }

    return (OS_POOL_API_PCB *)(NULL);
}


static void os_pool_api_FreePCB(IN OS_POOL_API_PCB *p_free_PCB)
{
    const OS_POOL_API_PCB *entry_end = os_pool_PCB + sizeof(os_pool_PCB)/sizeof(os_pool_PCB[0]);
    OS_POOL_API_PCB       *p_PCB;
    OS_POOL_API_PCB        PCB_bak;

    if ((p_free_PCB == NULL) || (p_free_PCB->pool_id == OS_POOL_API_INVALID_PCB))
    {
        return;
    }

    for (p_PCB = os_pool_PCB; p_PCB < entry_end; p_PCB++)
    {
        if (p_PCB->pool_id == p_free_PCB->pool_id)
        {
            memcpy(&PCB_bak, p_PCB, sizeof(*p_PCB));
            memset(p_PCB, 0x0, sizeof(*p_PCB));
            p_PCB->prev       = PCB_bak.prev;
            p_PCB->next       = PCB_bak.next;
            p_PCB->pool_id   = OS_POOL_API_INVALID_PCB;

            return;
        }
    }
}

#endif


/* always point to the first pool control pool entry */
static OS_POOL_API_PCB *p_pool_list  = NULL;

static GT_BOOL os_pool_api_CheckExist(IN const OS_POOL_API_PCB *p_check_PCB)
{
    OS_POOL_API_PCB       *p_PCB;

    if ((p_check_PCB == NULL)
        || (p_check_PCB->pool_id == OS_POOL_API_INVALID_PCB))
    {
        return GT_FALSE;
    }

    for (p_PCB = p_pool_list;
         ((p_PCB != NULL) && (p_PCB->next != NULL));
         p_PCB = p_PCB->next)
    {
        if ((p_PCB->pool_id == p_check_PCB->pool_id)
            && (p_PCB->pool_id != OS_POOL_API_INVALID_PCB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


void OS_POOL_API_Init(void)
{
    const OS_POOL_API_PCB *entry_end = os_pool_PCB + sizeof(os_pool_PCB)/sizeof(os_pool_PCB[0]);
    OS_POOL_API_PCB       *p_current_PCB;
    OS_POOL_API_PCB       *p_next_PCB;

    memset(os_pool_PCB, 0x0, sizeof(os_pool_PCB));

    /* init PCB buffer */
    for (p_current_PCB = os_pool_PCB;
         p_current_PCB < (entry_end - 1);
         p_current_PCB++)
    {
        p_next_PCB              = p_current_PCB + 1;
        p_current_PCB->next     = p_next_PCB;
        p_next_PCB->prev        = p_current_PCB;
        p_current_PCB->pool_id  = OS_POOL_API_INVALID_PCB;
    }

    if (!OS_POOL_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS pool fail!",
                          __FILE__, __LINE__);
    }
}

