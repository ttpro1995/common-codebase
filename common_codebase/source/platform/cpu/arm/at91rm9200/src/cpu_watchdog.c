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

#include <h/basetype.h>
#include <inc/boot_define.h>
#include <inc/reg.h>


/******************************************************************************
FUNCTION NAME:
    CPU_WatchDog_Set
DESCRIPTION:
    Enable/Disable WatchDog.
INPUT:
    enable : =GT_TRUE, enable watchdog;
             else,     disable watchdog.
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    The MAX watchdog timeout is 256sec,
    but here, we select 2sec to timeout. (2 * 0x100 * 128 / 65536)
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void CPU_WatchDog_Set(IN GT_BOOL enable)
{
    if (enable == GT_TRUE)
    {
        (VP32(ST_WDMR)) = 0x100 | (1<<16);
    }
    else
    {
        (VP32(ST_WDMR)) = 0x0;
    }

    /* reset watchdog */
    (VP32(ST_CR)) = (1<<0);
}


/******************************************************************************
FUNCTION NAME:
    CPU_WatchDog_Kick
DESCRIPTION:
    Kick WatchDog.
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void CPU_WatchDog_Kick(void)
{
    /* reset watchdog */
    (VP32(ST_CR)) = (1<<0);
}

