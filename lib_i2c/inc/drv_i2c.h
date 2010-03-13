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
 *   drv_i2c.h
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.5        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __DRV_I2C_H
#define __DRV_I2C_H

#include <lib_i2c.h>


typedef GT_BOOL (*OPEN_DEVICE_FUNC)(void);
typedef GT_BOOL (*CLOSE_DEVICE_FUNC)(void);
typedef GT_BOOL (*INIT_FUNC)(void);
typedef void    (*SET_SCL_FUNC)(int vData);
typedef void    (*SET_SDA_FUNC)(int vData);
typedef int     (*GET_SCL_FUNC)(void);
typedef int     (*GET_SDA_FUNC)(void);

typedef struct
{
    LIB_I2C_HOSTTYPE_E  vHostType;

    OPEN_DEVICE_FUNC    pOpenDeviceFunc;
    CLOSE_DEVICE_FUNC   pCloseDeviceFunc;
    INIT_FUNC           pInitFunc;
    SET_SCL_FUNC        pSetSclFunc;
    SET_SDA_FUNC        pSetSdaFunc;
    GET_SCL_FUNC        pGetSclFunc;
    GET_SDA_FUNC        pGetSdaFunc;
} DRV_I2C_HOST_CTRL_S;


/*******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_RandomWrite
 * DESCRIPTION:
 *      I2C Bus random write driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      offset      : I2C chip offset to be start to write.
 *      data_len    : I2C data buffer length to be written.
 *      data_buf    : I2C data buffer to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE        : I2C write success.
 *      FALSE       : I2C write fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
GT_BOOL DRV_I2C_RandomWrite
(
    IN        GT_U8     i2c_addr,
    IN        GT_U8     offset,
    IN        GT_U32    data_len,
    IN  const GT_U8    *data_buf
);

/*******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_RandomRead
 * DESCRIPTION:
 *      I2C Bus random read driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      offset      : I2C chip offset to be start to read.
 *      data_len    : I2C data buffer length to be read.
 * OUTPUT:
 *      data_buf    : I2C data buffer to be read.
 * RETURN:
 *      TRUE        : I2C read success.
 *      FALSE       : I2C read fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
GT_BOOL DRV_I2C_RandomRead
(
    IN  GT_U8     i2c_addr,
    IN  GT_U8     offset,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
);

/*******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_CurrentWrite
 * DESCRIPTION:
 *      I2C Bus write current driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      data_len    : I2C data buffer length to be write.
 * OUTPUT:
 *      data_buf    : I2C data buffer to be write.
 * RETURN:
 *      TRUE        : I2C write success.
 *      FALSE       : I2C write fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
GT_BOOL DRV_I2C_CurrentWrite
(
    IN       GT_U8     i2c_addr,
    IN       GT_U32    data_len,
    IN const GT_U8    *data_buf
);

/*******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_CurrentRead
 * DESCRIPTION:
 *      I2C Bus read current driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      data_len    : I2C data buffer length to be read.
 * OUTPUT:
 *      data_buf    : I2C data buffer to be read.
 * RETURN:
 *      TRUE        : I2C read success.
 *      FALSE       : I2C read fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
GT_BOOL DRV_I2C_CurrentRead
(
    IN  GT_U8     i2c_addr,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
);

/*******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_Detect
 * DESCRIPTION:
 *      I2C Bus chip detect driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE     : I2C chip is been detected.
 *      GT_FALSE    : I2C chip is not been detected.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 *****************************************************************************/
GT_BOOL DRV_I2C_Detect
(
	IN GT_U8    i2c_addr
);

/******************************************************************************
 * FUNCTION NAME:
 *      DRV_I2C_Init
 * DESCRIPTION:
 *      I2C init driver.
 * PARAMETERS:
 *      None
 * RETURN:
 *      I2C Host Type.
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.10.15     Panda Xiong       Create
 ******************************************************************************/
LIB_I2C_HOSTTYPE_E DRV_I2C_Init
(
    void
);


#endif /* __DRV_I2C_H */

