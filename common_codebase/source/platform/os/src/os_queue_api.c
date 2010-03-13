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

/* all OS queue control queues are here */
static OS_QUEUE_API_QCB  os_queue_QCB[OS_API_QUEUE_MAX_NUM];


static OS_QUEUE_API_QCB *os_queue_api_MallocQCB(void)
{
    const OS_QUEUE_API_QCB *entry_end = os_queue_QCB + sizeof(os_queue_QCB)/sizeof(os_queue_QCB[0]);
    OS_QUEUE_API_QCB       *p_QCB;

    for (p_QCB = os_queue_QCB; p_QCB < entry_end; p_QCB++)
    {
        if (p_QCB->queue_id == OS_QUEUE_API_INVALID_QCB)
        {
            p_QCB->queue_id = (p_QCB - os_queue_QCB);
            return p_QCB;
        }
    }

    return (OS_QUEUE_API_QCB *)(NULL);
}


static void os_queue_api_FreeQCB(IN OS_QUEUE_API_QCB *p_free_QCB)
{
    const OS_QUEUE_API_QCB *entry_end = os_queue_QCB + sizeof(os_queue_QCB)/sizeof(os_queue_QCB[0]);
    OS_QUEUE_API_QCB       *p_QCB;
    OS_QUEUE_API_QCB        QCB_bak;

    if ((p_free_QCB == NULL) || (p_free_QCB->queue_id == OS_QUEUE_API_INVALID_QCB))
    {
        return;
    }

    for (p_QCB = os_queue_QCB; p_QCB < entry_end; p_QCB++)
    {
        if (p_QCB->queue_id == p_free_QCB->queue_id)
        {
            memcpy(&QCB_bak, p_QCB, sizeof(*p_QCB));
            memset(p_QCB, 0x0, sizeof(*p_QCB));
            p_QCB->prev       = QCB_bak.prev;
            p_QCB->next       = QCB_bak.next;
            p_QCB->queue_id   = OS_QUEUE_API_INVALID_QCB;

            return;
        }
    }
}

#endif


/* always point to the first queue control queue entry */
static OS_QUEUE_API_QCB *p_queue_list  = NULL;

static GT_BOOL os_queue_api_CheckExist(IN const OS_QUEUE_API_QCB *p_check_QCB)
{
    OS_QUEUE_API_QCB       *p_QCB;

    if ((p_check_QCB == NULL)
        || (p_check_QCB->queue_id == OS_QUEUE_API_INVALID_QCB))
    {
        return GT_FALSE;
    }

    for (p_QCB = p_queue_list;
         ((p_QCB != NULL) && (p_QCB->next != NULL));
         p_QCB = p_QCB->next)
    {
        if ((p_QCB->queue_id == p_check_QCB->queue_id)
            && (p_QCB->queue_id != OS_QUEUE_API_INVALID_QCB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


void OS_QUEUE_API_Init(void)
{
    const OS_QUEUE_API_QCB *entry_end = os_queue_QCB + sizeof(os_queue_QCB)/sizeof(os_queue_QCB[0]);
    OS_QUEUE_API_QCB       *p_current_QCB;
    OS_QUEUE_API_QCB       *p_next_QCB;

    memset(os_queue_QCB, 0x0, sizeof(os_queue_QCB));

    /* init QCB buffer */
    for (p_current_QCB = os_queue_QCB;
         p_current_QCB < (entry_end - 1);
         p_current_QCB++)
    {
        p_next_QCB              = p_current_QCB + 1;
        p_current_QCB->next     = p_next_QCB;
        p_next_QCB->prev        = p_current_QCB;
        p_current_QCB->queue_id  = OS_QUEUE_API_INVALID_QCB;
    }

    if (!OS_QUEUE_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS queue fail!",
                          __FILE__, __LINE__);
    }
}

