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
 *   drv_i2c_usb.h
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.8.15        esus         Create/Update
 *
*****************************************************************************/

#ifndef __DRV_I2C_USB_H
#define __DRV_I2C_USB_H


GT_BOOL DRV_I2C_USB_OpenDevice(void);
GT_BOOL DRV_I2C_USB_CloseDevice(void);
GT_BOOL DRV_I2C_USB_Init(void);
void    DRV_I2C_USB_SetSCL(int vData);
void    DRV_I2C_USB_SetSDA(int vData);
int     DRV_I2C_USB_GetSDA(void);
int     DRV_I2C_USB_GetSCL(void);

#if 1

/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_USB_RandomRead
 * DESCRIPTION:
 *      Read I2C USB Slave, using Random Read Mode.
 * PARAMETERS:
 *      vAddr     : I2C Slave address;
 *      vOffset   : Offset address to start read;
 *      vLen      : Read Length;
 *      pReadBuf  : Read Buffer;
 * RETURN:
 *      =TRUE,  Success;
 *      =FALSE, Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.11        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL Drv_I2C_USB_RandomRead
(
    IN  GT_U8   vAddr,
    IN  GT_U8   vOffset,
    IN  GT_U32  vLen,
    OUT GT_U8  *pReadBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_USB_CurrentRead
 * DESCRIPTION:
 *      Read I2C USB Slave, using Current Read Mode.
 * PARAMETERS:
 *      vAddr     : I2C Slave address;
 *      vLen      : Read Length;
 *      pReadBuf  : Read Buffer;
 * RETURN:
 *      =TRUE,  Success;
 *      =FALSE, Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.11        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL Drv_I2C_USB_CurrentRead
(
    IN  GT_U8   vAddr,
    IN  GT_U32  vLen,
    OUT GT_U8  *pReadBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_USB_RandomWrite
 * DESCRIPTION:
 *      Write I2C USB Slave, using Random Write Mode.
 * PARAMETERS:
 *      vAddr     : I2C Slave address;
 *      vOffset   : Offset address to start write;
 *      vLen      : Write Length;
 *      pReadBuf  : Write Buffer;
 * RETURN:
 *      =TRUE,  Success;
 *      =FALSE, Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.11        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL Drv_I2C_USB_RandomWrite
(
    IN        GT_U8   vAddr,
    IN        GT_U8   vOffset,
    IN        GT_U32  vLen,
    OUT const GT_U8  *pReadBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_USB_CurrentWrite
 * DESCRIPTION:
 *      Write I2C USB Slave, using Current Write Mode.
 * PARAMETERS:
 *      vAddr     : I2C Slave address;
 *      vLen      : Write Length;
 *      pReadBuf  : Write Buffer;
 * RETURN:
 *      =TRUE,  Success;
 *      =FALSE, Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.11        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL Drv_I2C_USB_CurrentWrite
(
    IN        GT_U8   vAddr,
    IN        GT_U32  vLen,
    OUT const GT_U8  *pReadBuf
);

/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_USB_Detect
 * DESCRIPTION:
 *      Detect slave address.
 * PARAMETERS:
 *      vAddr : Address to be detected.
 * RETURN:
 *      TRUE  : Detect success;
 *      FALSE : Detect fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.17        Panda.Xiong         Create/Update
 *****************************************************************************/
GT_BOOL Drv_I2C_USB_Detect
(
    IN GT_U8   vAddr
);

#endif

#endif /* __DRV_I2C_USB_H */

