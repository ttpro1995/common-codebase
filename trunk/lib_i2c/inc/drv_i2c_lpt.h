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
 *   drv_i2c_lpt.h
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.4        PANDA         Create/Update
 *
*****************************************************************************/

#ifndef __DRV_I2C_LPT_H
#define __DRV_I2C_LPT_H


#if 1

GT_BOOL DRV_I2C_LPTA_OpenDevice(void);
GT_BOOL DRV_I2C_LPTA_CloseDevice(void);
GT_BOOL DRV_I2C_LPTA_Init(void);
void    DRV_I2C_LPTA_SetSCL(int vData);
void    DRV_I2C_LPTA_SetSDA(int vData);
int     DRV_I2C_LPTA_GetSCL(void);
int     DRV_I2C_LPTA_GetSDA(void);

#endif

#if 1

GT_BOOL DRV_I2C_LPTB_OpenDevice(void);
GT_BOOL DRV_I2C_LPTB_CloseDevice(void);
GT_BOOL DRV_I2C_LPTB_Init(void);
void    DRV_I2C_LPTB_SetSCL(int vData);
void    DRV_I2C_LPTB_SetSDA(int vData);
int     DRV_I2C_LPTB_GetSCL(void);
int     DRV_I2C_LPTB_GetSDA(void);

#endif

/******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_LPT_SetLptAddress
 * DESCRIPTION:
 *      Set LPT Base address.
 * PARAMETERS:
 *      vLptAddress : LPT Base address to be set.
 * RETURN:
 *      TRUE  : Success;
 *      FALSE : Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.15        PANDA         Create/Update
 *****************************************************************************/
GT_BOOL DRV_I2C_LPT_SetLptAddress
(
    IN GT_U32   vLptAddress
);


#endif /* __DRV_I2C_LPT_H */

