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

/* all OS block control blocks are here */
static OS_BLOCK_API_BCB  os_block_BCB[OS_API_BLOCK_MAX_NUM];


static OS_BLOCK_API_BCB *os_block_api_MallocBCB(void)
{
    const OS_BLOCK_API_BCB *entry_end = os_block_BCB + sizeof(os_block_BCB)/sizeof(os_block_BCB[0]);
    OS_BLOCK_API_BCB       *p_BCB;

    for (p_BCB = os_block_BCB; p_BCB < entry_end; p_BCB++)
    {
        if (p_BCB->block_id == OS_BLOCK_API_INVALID_BCB)
        {
            p_BCB->block_id = (p_BCB - os_block_BCB);
            return p_BCB;
        }
    }

    return (OS_BLOCK_API_BCB *)(NULL);
}


static void os_block_api_FreeBCB(IN OS_BLOCK_API_BCB *p_free_BCB)
{
    const OS_BLOCK_API_BCB *entry_end = os_block_BCB + sizeof(os_block_BCB)/sizeof(os_block_BCB[0]);
    OS_BLOCK_API_BCB       *p_BCB;
    OS_BLOCK_API_BCB        BCB_bak;

    if ((p_free_BCB == NULL) || (p_free_BCB->block_id == OS_BLOCK_API_INVALID_BCB))
    {
        return;
    }

    for (p_BCB = os_block_BCB; p_BCB < entry_end; p_BCB++)
    {
        if (p_BCB->block_id == p_free_BCB->block_id)
        {
            memcpy(&BCB_bak, p_BCB, sizeof(*p_BCB));
            memset(p_BCB, 0x0, sizeof(*p_BCB));
            p_BCB->prev       = BCB_bak.prev;
            p_BCB->next       = BCB_bak.next;
            p_BCB->block_id   = OS_BLOCK_API_INVALID_BCB;

            return;
        }
    }
}

#endif


/* always point to the first block control block entry */
static OS_BLOCK_API_BCB *p_block_list  = NULL;

static GT_BOOL os_block_api_CheckExist(IN const OS_BLOCK_API_BCB *p_check_BCB)
{
    OS_BLOCK_API_BCB       *p_BCB;

    if ((p_check_BCB == NULL)
        || (p_check_BCB->block_id == OS_BLOCK_API_INVALID_BCB))
    {
        return GT_FALSE;
    }

    for (p_BCB = p_block_list;
         ((p_BCB != NULL) && (p_BCB->next != NULL));
         p_BCB = p_BCB->next)
    {
        if ((p_BCB->block_id == p_check_BCB->block_id)
            && (p_BCB->block_id != OS_BLOCK_API_INVALID_BCB))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}


void OS_BLOCK_API_Init(void)
{
    const OS_BLOCK_API_BCB *entry_end = os_block_BCB + sizeof(os_block_BCB)/sizeof(os_block_BCB[0]);
    OS_BLOCK_API_BCB       *p_current_BCB;
    OS_BLOCK_API_BCB       *p_next_BCB;

    memset(os_block_BCB, 0x0, sizeof(os_block_BCB));

    /* init BCB buffer */
    for (p_current_BCB = os_block_BCB;
         p_current_BCB < (entry_end - 1);
         p_current_BCB++)
    {
        p_next_BCB              = p_current_BCB + 1;
        p_current_BCB->next     = p_next_BCB;
        p_next_BCB->prev        = p_current_BCB;
        p_current_BCB->block_id = OS_BLOCK_API_INVALID_BCB;
    }

    if (!OS_BLOCK_PORT_Init())
    {
        OS_API_FatalError("%s(line%d): Init OS block fail!",
                          __FILE__, __LINE__);
    }
}

