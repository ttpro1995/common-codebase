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

#ifndef __CLI_H
#define __CLI_H


#include "cli_porting.h"


/* User command should return these values */
typedef enum
{
    CLI_PRT_INVALID = 0,  /* the cli will print nothing      */
    CLI_PRT_NONE    = 1,  /* the cli will print a black line */
    CLI_PRT_SUCCESS = 2,  /* the cli will print success      */
    CLI_PRT_FAIL    = 3   /* the cli will print fail         */
} CLI_REG_CMD_RETURN_T;

#if   (CLI_MAX_PARAM < 256)
 typedef UINT8  CLI_CMD_PARAM_T;
#elif (CLI_MAX_PARAM < 65536)
 typedef UINT16 CLI_CMD_PARAM_T;
#else
 typedef UINT32 CLI_CMD_PARAM_T;
#endif

typedef CLI_REG_CMD_RETURN_T (*CLI_REG_CMD_EXEC_T)
(
    IN const CLI_CMD_PARAM_T  n_param,
    IN const UINT8           *param[]
);

/* register command stucture format */
typedef struct _cli_reg_cmd_t
{
    const UINT8           *cmd_name;       /* command name                */
    CLI_REG_CMD_EXEC_T     cmd_exec;       /* execute command function    */
    const UINT8           *cmd_help;       /* command help                */

    struct _cli_reg_cmd_t *prev_cmd;       /* the prev command            */
    struct _cli_reg_cmd_t *next_cmd;       /* the next command            */
} CLI_REG_CMD_T;


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_GetVersion
 * DESCRIPTION:
 *      Get CLI Version.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      CLI Version String.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2010.4.13        Panda.Xiong         Create/Update
 *****************************************************************************/
const UINT8 *CLI_GetVersion(void);

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
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
BOOL CLI_REG_RegisterCmd(IN CLI_REG_CMD_T *cmd);

/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
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
BOOL CLI_REG_DeRegisterCmd(IN CLI_REG_CMD_T *cmd);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_IO_SetPrompt
 * DESCRIPTION:
 *      Set CLI Prompt string.
 * PARAMETERS:
 *      pCliPrompt : CLI Prompt string to be set.
 * RETURN:
 *      TRUE    : Success.
 *      FALSE   : Fail.
 * NOTES:
 *      The CLI Prompt string can not over 255, else, set fail.
 * HISTORY:
 *      2009.5.14        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL CLI_IO_SetPrompt(IN const UINT8  *pCliPrompt);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Init
 * DESCRIPTION:
 *      Init CLI kernel.
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

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_Start
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
void CLI_Start(void);


#endif /* __CLI_H */

