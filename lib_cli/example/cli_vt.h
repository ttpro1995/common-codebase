/******************************************************************************
 *
 * COPYRIGHT:
 *   Copyright (c)  2005-2010   Source Photonics Inc.    All rights reserved.
 *
 *   This is unpublished proprietary source code of Source Photonics Inc.
 *   The copyright notice above does not evidence any actual or intended
 *   publication of such source code.
 *
 * FILE NAME:
 *   cli_vt.h
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.13        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __CLI_VT_H
#define __CLI_VT_H


/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_GetChar
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_S32 CLI_VT_Printf(IN const GT_S8 *fmt, ...);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_ClearScreen
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.13        Panda.Xiong         Create/Update
 *****************************************************************************/
void CLI_VT_ClearScreen(void);

/******************************************************************************
 * FUNCTION NAME:
 *      CLI_VT_ReadKey
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.12        panda.xiong         Create/Update
 *****************************************************************************/
GT_S32 CLI_VT_ReadKey(void);


#endif /* __CLI_VT_H */


