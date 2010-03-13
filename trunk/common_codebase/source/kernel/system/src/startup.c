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
#include <inc/cpu_init.h>
#include <inc/drv_flash.h>
#include <inc/utl_vt.h>
#include <inc/utl_prt.h>
#include <cli/inc/cli_api.h>

/* declare start stack space.
 *
 * here, should make it align to 4byte, so, use GT_UI32 to align it.
 */
GT_UI32	start_stack[BOOT_STACK_SIZE/sizeof(GT_UI32)];


/******************************************************************************
FUNCTION NAME:
    startC
DESCRIPTION:
    The C procedure start from here
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
void startC(void)
{
    if (!CPU_Init())
    {
        UTL_PRT_Printf("\n\rCPU Init Fail!");
        while (1)
        {}
    }

    DRV_SIO_Init();
	DRV_FLASH_Init();

    /* initial VT, and clear screen */
    UTL_VT_Init();
    UTL_VT_ClearScreen();

    CLI_Init();

    /* never reach here */
    return;
}

