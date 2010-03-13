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

/* all OS event control events are here */
static OS_EVENT_API_ECB  os_event_ECB[OS_API_EVENT_MAX_NUM];


static OS_EVENT_API_ECB *os_event_api_MallocECB(void)
{
    const OS_EVENT_API_ECB *entry_end = os_event_ECB + sizeof(os_event_ECB)/sizeof(os_event_ECB[0]);
    OS_EVENT_API_ECB       *p_ECB;

    for (p_ECB = os_event_ECB; p_ECB < entry_end; p_ECB++)
    {
        if (p_ECB->event_id == OS_EVENT_API_INVALID_ECB)
        {
            p_ECB->event_id = (p_ECB - os_event_ECB);
            return p_ECB;
        }
    }

    return (OS_EVENT_API_ECB *)(NULL);
}


static void os_event_api_FreeECB(IN OS_EVENT_API_ECB *p_free_ECB)
{
    const OS_EVENT_API_ECB *entry_end = os_event_ECB + sizeof(os_event_ECB)/sizeof(os_event_ECB[0]);
    OS_EVENT_API_ECB       *p_ECB;
    OS_EVENT_API_ECB        ECB_bak;

    if ((p_free_ECB == NULL) || (p_free_ECB->event_id == OS_EVENT_API_INVALID_ECB))
    {
        return;
    }

    for (p_ECB = os_event_ECB; p_ECB < entry_end; p_ECB++)
    {
        if (p_ECB->event_id == p_free_ECB->event_id)
        {
            memcpy(&ECB_bak, p_ECB, sizeof(*p_ECB));
            memset(p_ECB, 0x0, sizeof(*p_ECB));
            p_ECB->prev       = ECB_bak.prev;
            p_ECB->next       = ECB_bak.next;
            p_ECB->event_id   = OS_EVENT_API_INVALID_ECB;

            return;
        }
    }
}

#endif


/* always point to the first event control event entry */
static OS_EVENT_API_ECB *p_event_list  = NULL;

static GT_BOOL os_event_api_CheckExist(IN const OS_EVENT_API_ECB *p_check_ECB)
{
    OS_EVENT_API_ECB       *p_ECB;

    if ((p_check_ECB == NULL)
        || (p_check_ECB->event_id == OS_EVENT_API_INVALID_ECB))
    {
        return GT_FALSE;
    }

    for (p_ECB = p_event_list;
         ((p_ECB != NULL) && (p_ECB->next != NULL));
         p_ECB = p_ECB->next)
    {
        if ((p_ECB->event_id == p_check_ECB->event_id)
            && (p_ECB->event_id != OS_EVENT_API_INVALID_ECB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


void OS_EVENT_API_Init(void)
{
    const OS_EVENT_API_ECB *entry_end = os_event_ECB + sizeof(os_event_ECB)/sizeof(os_event_ECB[0]);
    OS_EVENT_API_ECB       *p_current_ECB;
    OS_EVENT_API_ECB       *p_next_ECB;

    memset(os_event_ECB, 0x0, sizeof(os_event_ECB));

    /* init ECB buffer */
    for (p_current_ECB = os_event_ECB;
         p_current_ECB < (entry_end - 1);
         p_current_ECB++)
    {
        p_next_ECB              = p_current_ECB + 1;
        p_current_ECB->next     = p_next_ECB;
        p_next_ECB->prev        = p_current_ECB;
        p_current_ECB->event_id = OS_EVENT_API_INVALID_ECB;
    }

    if (!OS_EVENT_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS event fail!",
                          __FILE__, __LINE__);
    }
}

