/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    None
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __CLI_H__
#define __CLI_H__

#include <basetype.h>
#include <utl_vt.h>
#include <utl_string.h>


/*******************************************************************************
 *  CLI external define
*******************************************************************************/
/* user commands defined here, these commands will stored in ROM.
 * the format must be (CLI_CMD_T)
 */
#define CLI_API_COMMANDS                                                \
{                                                                       \
    /* 0 is used to indicate an invalid index */                        \
                                                                        \
    /* The commands below are reserved for CLI kernel */                \
    {1, "?",             CLI_CMD_Help},                                 \
    {2, "help",          CLI_CMD_Help},                                 \
    {3, "cls",           CLI_CMD_ClearScreen},                          \
    {4, "clear",         CLI_CMD_ClearScreen},                          \
    {5, "show board",    CLI_CMD_ShowBoard},                              \
                                                                        \
    /* The commands below are for user */                               \
}
/******************************************************************************/


/*******************************************************************************
 *  CLI internal define
*******************************************************************************/
#define CLI_CMD_MAX_COMMANDS    (255)
#define CLI_CMD_MAX_LEN         (16)

/* if max commands less than 255, use UI8_T to save memory */
#if (CLI_CMD_MAX_COMMANDS <= 255)
  #define CLI_CMD_INDEX_T       UI8_T
#else
  #define CLI_CMD_INDEX_T       UI16_T
#endif

typedef UI8_T  (*CLI_CMD_EXEC_T)(void);

/* user command format */
typedef struct
{
    CLI_CMD_INDEX_T index;                     /* command index, start from 1 */
    UI8_T           cmd[CLI_CMD_MAX_LEN + 1];  /* command                     */
    CLI_CMD_EXEC_T  cmd_exec;                  /* execute command function    */
} CLI_CMD_T;

/* User command should return these value below */
#define CLI_PRINT_NONE      (0)
#define CLI_PRINT_SUCCESS   (1)
#define CLI_PRINT_FAIL      (2)


/*******************************************************************************
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
*******************************************************************************/
void CLI_Init(void);



/*******************************************************************************
 * CLI Kernel reserved commands
*******************************************************************************/

/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_ClearScreen(void);


/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_Help(void);

/*******************************************************************************
 * COPYRIGHT:
 *      None
 * FILE NAME:
 *      None
 * DESCRIPTION:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
UI8_T CLI_CMD_ShowBoard(void);

#endif /* __CLI_H__ */

