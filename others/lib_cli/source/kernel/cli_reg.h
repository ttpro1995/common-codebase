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

#ifndef __CLI_REG_H
#define __CLI_REG_H


/* User command should return these values */
typedef enum
{
    CLI_PRT_INVALID = 0,  /* the cli will print nothing      */
    CLI_PRT_NONE    = 1,  /* the cli will print a black line */
    CLI_PRT_SUCCESS = 2,  /* the cli will print success      */
    CLI_PRT_FAIL    = 3   /* the cli will print fail         */
} CLI_REG_CMD_RETURN_E;


typedef CLI_REG_CMD_RETURN_E (*CLI_REG_CMD_EXEC_T)(IN const GT_U32 n_param, IN const GT_S8 *param[]);

/* register command stucture format */
typedef struct _cli_reg_cmd_s
{
    GT_S8                 *cmd_name;       /* command name                */
    CLI_REG_CMD_EXEC_T     cmd_exec;       /* execute command function    */
    GT_S8                 *cmd_help;       /* command help                */

    struct _cli_reg_cmd_s *prev_cmd;       /* the prev command            */
    struct _cli_reg_cmd_s *next_cmd;       /* the next command            */
} CLI_REG_CMD_S;


/******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      check this command has been registed or not.
 *      if registerd, return GT_TRUE; else, return GT_FALSE.
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
GT_BOOL CLI_REG_IsCmdRegisterd(IN const CLI_REG_CMD_S *cmd);

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
GT_BOOL CLI_REG_RegisterCmd(IN CLI_REG_CMD_S *cmd);

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
GT_BOOL CLI_REG_DeRegisterCmd(IN CLI_REG_CMD_S *cmd);

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
CLI_REG_CMD_S *CLI_REG_SearchCommand(IN const GT_S8 *cmd);

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
CLI_REG_CMD_S *CLI_REG_GetNextCommand(IN const CLI_REG_CMD_S *cmd);

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
CLI_REG_CMD_S *CLI_REG_GetPrevCommand(IN const CLI_REG_CMD_S *cmd);

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
CLI_REG_CMD_S *CLI_REG_GetNextMatchCommand
(
    IN const CLI_REG_CMD_S *cmd,
    IN const GT_S8         *match_str
);

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
GT_U32 CLI_REG_GetMatchCommandInfo
(
    IN const GT_S8  *match_str,
    OUT      GT_U8  *least_matched_str
);

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
GT_BOOL CLI_REG_Init(void);


#endif /* __CLI_REG_H */

