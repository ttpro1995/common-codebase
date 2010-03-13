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

#ifndef __CLI_API_H
#define __CLI_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <h/basetype.h>
#include <h/stdarg.h>
#include <inc/utl_vt.h>
#include <inc/drv_sio.h>
#include <inc/utl_string.h>
#include <inc/utl_tran.h>
#include <inc/utl_prt.h>
#include <inc/cpu_io.h>

#include "./cli_port.h"        /* cli porting header            */
#include "./cli_io.h"          /* cli io entry header           */
#include "./cli_reg.h"         /* cli command register header   */


#ifdef _CLI_KERNEL_SOURCE_
/* these lines are only valid in cli kernel source files */

#include "../h/cli_par.h"      /* cli command parser header     */
#include "../h/cli_cmd.h"      /* cli reserved command header   */
#include "../h/cli_history.h"  /* cli history                   */

#define CLI_VT_GetChar()		DRV_SIO_GetChar()
#define CLI_VT_Printf           UTL_PRT_Printf

#define CLI_VT_ClearScreen()	UTL_VT_ClearScreen()
#define CLI_VT_MoveLeft(n)		UTL_VT_MoveLeft(n)
#define CLI_VT_MoveRight(n)		UTL_VT_MoveRight(n)
#define CLI_VT_MoveUp(n)		UTL_VT_MoveUp(n)
#define CLI_VT_MoveDown(n)		UTL_VT_MoveDown(n)


#else
/* these lines are not valid in cli kernel source files */

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Init
 * DESCRIPTION:
 *      CLI Entry, will not return.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
void CLI_Init(void);

#endif /* _CLI_KERNEL_SOURCE_ */


#ifdef __cplusplus
}
#endif

#endif /* __CLI_API_H */

