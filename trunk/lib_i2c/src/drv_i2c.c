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
 *   drv_i2c.c
 * NODULE NAME:
 *   I2C Common driver.
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.5        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#include <typedef.h>
#include <drv_i2c.h>
#include <drv_i2c_lpt.h>
#include <drv_i2c_usb.h>


static const DRV_I2C_HOST_CTRL_S    aI2cHostCtrlTable[] =
{
    /* LPTA */
    {
        LIB_I2C_HOSTTYPE_LPTA,
        DRV_I2C_LPTA_OpenDevice,
        DRV_I2C_LPTA_CloseDevice,
        DRV_I2C_LPTA_Init,
        DRV_I2C_LPTA_SetSCL,
        DRV_I2C_LPTA_SetSDA,
        DRV_I2C_LPTA_GetSCL,
        DRV_I2C_LPTA_GetSDA,
    },

    /* LPTB */
    {
        LIB_I2C_HOSTTYPE_LPTB,
        DRV_I2C_LPTB_OpenDevice,
        DRV_I2C_LPTB_CloseDevice,
        DRV_I2C_LPTB_Init,
        DRV_I2C_LPTB_SetSCL,
        DRV_I2C_LPTB_SetSDA,
        DRV_I2C_LPTB_GetSCL,
        DRV_I2C_LPTB_GetSDA,
    },

    /* USB */
    {
        LIB_I2C_HOSTTYPE_USB,
        DRV_I2C_USB_OpenDevice,
        DRV_I2C_USB_CloseDevice,
        DRV_I2C_USB_Init,
        DRV_I2C_USB_SetSCL,
        DRV_I2C_USB_SetSDA,
        DRV_I2C_USB_GetSCL,
        DRV_I2C_USB_GetSDA,
    },
};


static const DRV_I2C_HOST_CTRL_S    *pCurrentHost = NULL;

#define DRV_I2C_SET_SCL(vData)  (pCurrentHost->pSetSclFunc(vData))
#define DRV_I2C_SET_SDA(vData)  (pCurrentHost->pSetSdaFunc(vData))
#define DRV_I2C_GET_SCL(vData)  (pCurrentHost->pGetSclFunc())
#define DRV_I2C_GET_SDA(vData)  (pCurrentHost->pGetSdaFunc())
#define DRV_I2C_OPEN_DEVICE(err_return)                 \
    do {                                                \
        if (!pCurrentHost->pOpenDeviceFunc())           \
            return (err_return);                        \
    } while (0)
#define DRV_I2C_CLOSE_DEVICE(err_return)                \
    do {                                                \
        if (!pCurrentHost->pCloseDeviceFunc())          \
            return (err_return);                        \
    } while (0)

/* low-level I2C driver */
#if 1

#define DRV_I2C_TIMEOUT         25      /* ms */


/* return : GT_TRUE,  an ACK received;
 *          GT_FALSE, no ACK received.
 */
static GT_BOOL _drv_i2c_CheckAck(void)
{
    GT_BOOL vAck;

    DRV_I2C_SET_SCL(0);
    DRV_I2C_SET_SDA(1);
    DRV_I2C_SET_SCL(1);
    vAck = DRV_I2C_GET_SDA();
    DRV_I2C_SET_SCL(0);

    return !vAck;
}


static void _drv_i2c_SendAck(void)
{
    DRV_I2C_SET_SCL(0);
    DRV_I2C_SET_SDA(0);

    DRV_I2C_SET_SCL(1);
    DRV_I2C_SET_SCL(0);
    DRV_I2C_SET_SDA(1);
}


static void _drv_i2c_SendNoAck(void)
{
    DRV_I2C_SET_SDA(1);
    DRV_I2C_SET_SCL(1);
    DRV_I2C_SET_SCL(0);
}

static GT_BOOL _drv_i2c_ReadBit(void)
{
    GT_BOOL vBit;

    DRV_I2C_SET_SCL(0);

    DRV_I2C_SET_SCL(1);
    vBit = DRV_I2C_GET_SDA();
    DRV_I2C_SET_SCL(0);

    return vBit;
}

static void _drv_i2c_WriteBit(IN GT_BOOL vBit)
{
    DRV_I2C_SET_SCL(0);
    DRV_I2C_SET_SDA(vBit);
    DRV_I2C_SET_SCL(1);
}


static GT_BOOL _drv_i2c_SendData(IN GT_U8 vData)
{
    GT_U8   vTimeoutCount = 0;
    GT_U8   vLoop;

    for (vLoop = 8; vLoop > 0; vLoop--)
    {
        _drv_i2c_WriteBit(READ_BIT(vData, vLoop-1));
    }

    /* clock stretch */
    while (DRV_I2C_GET_SCL() == 0)
    {
        if (vTimeoutCount++ > DRV_I2C_TIMEOUT)
        {
            return GT_FALSE;
        }

        Delay_Ms(1);
    }

    return _drv_i2c_CheckAck();
}


static GT_BOOL _drv_i2c_ReceiveData(OUT GT_U8 *pData)
{
    GT_U8   vLoop;
    GT_U8   vData  = 0;

    for (vLoop = 8; vLoop > 0; vLoop--)
    {
        vData <<= 1;
        vData  |= _drv_i2c_ReadBit();
    }

    *pData = vData;
    _drv_i2c_SendAck();

    return GT_TRUE;
}


static GT_BOOL _drv_i2c_ReceiveLastData(OUT GT_U8 *pData)
{
    GT_U8   vLoop;
    GT_U8   vData = 0;

    for (vLoop = 8; vLoop > 0; vLoop--)
    {
        vData <<= 1;
        vData  |= _drv_i2c_ReadBit();
    }

    *pData = vData;
    _drv_i2c_SendNoAck();

    return GT_TRUE;
}


static GT_BOOL _drv_i2c_SendAddr_R(IN GT_U8 i2c_addr)
{
    GT_U8   vData;

    vData = i2c_addr;
    SET_BIT(vData, 0);

    return _drv_i2c_SendData(vData);
}


static GT_BOOL _drv_i2c_SendAddr_W(IN GT_U8 i2c_addr)
{
    GT_U8   vData;

    vData = i2c_addr;
    CLR_BIT(vData, 0);

    return _drv_i2c_SendData(vData);
}


static GT_BOOL _drv_i2c_Start(void)
{
    DRV_I2C_SET_SDA(1);
    DRV_I2C_SET_SCL(1);
    DRV_I2C_SET_SDA(0);
    DRV_I2C_SET_SCL(0);

    return GT_TRUE;
}


static GT_BOOL _drv_i2c_ReStart(void)
{
    return _drv_i2c_Start();
}


static void _drv_i2c_Stop(void)
{
    DRV_I2C_SET_SCL(0);
    DRV_I2C_SET_SDA(0);
    DRV_I2C_SET_SCL(1);
    DRV_I2C_SET_SDA(1);
}


/* check I2C hardware is ready to operate or not */
static GT_BOOL _drv_i2c_CheckReady(void)
{
    if (DRV_I2C_GET_SCL() == 1)
    {
        /* The I2C only can be started, while SCL is at high level */
        return GT_TRUE;
    }
    else
    {
        /* SCL is been stretched, operation fail */
        return GT_FALSE;
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      drv_i2c_Init
 * DESCRIPTION:
 *      Init I2C.
 * PARAMETERS:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.12.5     Panda Xiong       Create
 ******************************************************************************/
static void _drv_i2c_Init(void)
{
    /* force stop all operations on I2C bus */
    _drv_i2c_Stop();

    return;
}

#endif


#if 1

/******************************************************************************
 * FUNCTION NAME:
 *      drv_i2c_ReadBytes
 * DESCRIPTION:
 *      Read one or more bytes, with/without offset.
 * PARAMETERS:
 *      i2c_addr       : I2C slave chip address.
 *      offset_len     : offset buffer length to be send.
 *      offset_buf     : offset buffer.
 *      data_len       : data length to be read.
 *      data_buf       : data buffer.
 * RETURN:
 *      TRUE   : read success.
 *      FALSE  : read fail.
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.12.5     Panda Xiong       Create
 ******************************************************************************/
static GT_BOOL drv_i2c_ReadBytes
(
    IN       GT_U8      i2c_addr,
    IN       GT_U32     offset_len,
    IN const GT_U8      *offset_buf,
    IN       GT_U32     data_len,
    OUT      GT_U8      *data_buf
)
{
    GT_U32      loop;

    /* make sure the IIC is ready */
    if (!_drv_i2c_CheckReady())
    {
        goto _error_exit;
    }

    /* send IIC start */
    if (!_drv_i2c_Start())
    {
        goto _error_exit;
    }

    if (offset_len > 0)
    {
        /* send IIC chip address, and check ACK */
        if (!_drv_i2c_SendAddr_W(i2c_addr))
        {
            goto _error_exit;
        }

        /* send offset */
        for (loop = 0; loop < offset_len; loop++)
        {
            /* send offset, and check ACK */
            if (!_drv_i2c_SendData(offset_buf[loop]))
            {
                goto _error_exit;
            }
        }

        /* IIC repeat start */
        if (!_drv_i2c_ReStart())
        {
            goto _error_exit;
        }
    }

    /* send IIC chip address, and check ACK */
    if (!_drv_i2c_SendAddr_R(i2c_addr))
    {
        goto _error_exit;
    }

    /* read data */
    for (loop = 0; loop < data_len; loop++)
    {
        /* if it's the last byte data, don't send ACK */
        if (loop == (data_len - 1))
        {
            if (!_drv_i2c_ReceiveLastData(&data_buf[loop]))
            {
                goto _error_exit;
            }
        }
        else
        {
            /* normal read data */
            if (!_drv_i2c_ReceiveData(&data_buf[loop]))
            {
                goto _error_exit;
            }
        }
    }

/* success exit */
    /* send IIC stop */
    _drv_i2c_Stop();
    return GT_TRUE;

_error_exit:
    /* we should reset the I2C, or the I2C bus will keep on fail */
    _drv_i2c_Init();
    return GT_FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      drv_i2c_WriteBytes
 * DESCRIPTION:
 *      Write one or more bytes, with/without offset.
 * PARAMETERS:
 *      i2c_addr       : I2C slave chip address.
 *      offset_buf     : offset buffer.
 *      offset_len     : offset buffer length to be send.
 *      data_buf       : data buffer.
 *      data_len       : data length to be write.
 * RETURN:
 *      TRUE   : write success.
 *      FALSE  : write fail.
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.12.5     Panda Xiong       Create
 ******************************************************************************/
static GT_BOOL drv_i2c_WriteBytes
(
    IN       GT_U8       i2c_addr,
    IN       GT_U32      offset_len,
    IN const GT_U8      *offset_buf,
    IN       GT_U32      data_len,
    IN const GT_U8      *data_buf
)
{
    GT_U32      loop;

    /* make sure the IIC is ready */
    if (!_drv_i2c_CheckReady())
    {
        goto _error_exit;
    }

    /* send IIC start */
    if (!_drv_i2c_Start())
    {
        goto _error_exit;
    }

    /* send IIC chip address, and check ACK */
    if (!_drv_i2c_SendAddr_W(i2c_addr))
    {
        goto _error_exit;
    }

    /* send offset */
    for (loop = 0; loop < offset_len; loop++)
    {
        /* send offset, and check ACK */
        if (!_drv_i2c_SendData(offset_buf[loop]))
        {
            goto _error_exit;
        }
    }

    /* send data */
    for (loop = 0; loop < data_len; loop++)
    {
        if (!_drv_i2c_SendData(data_buf[loop]))
        {
            goto _error_exit;
        }
    }

/* success exit */
    /* send IIC stop */
    _drv_i2c_Stop();
    return GT_TRUE;

_error_exit:
    /* we should reset the I2C, or the I2C bus will keep on fail */
    _drv_i2c_Init();
    return GT_FALSE;
}

/*******************************************************************************
 * FUNCTION NAME:
 *      drv_i2c_Detect
 * DESCRIPTION:
 *      I2C detect chip.
 * PARAMETERS:
 *      i2c_addr : I2C chip address.
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2009.1.16     Panda Xiong       Create
 ******************************************************************************/

static GT_BOOL drv_i2c_Detect
(
    IN GT_U8    i2c_addr
)
{
    /* make sure the IIC is ready */
    if (!_drv_i2c_CheckReady())
    {
        goto _error_exit;
    }

    /* send IIC start */
    if (!_drv_i2c_Start())
    {
        goto _error_exit;
    }

    /* send IIC chip address, and check ACK */
    if (!_drv_i2c_SendAddr_W(i2c_addr))
    {
        goto _error_exit;
    }

/* success exit */
    /* send IIC stop */
    _drv_i2c_Stop();
    return GT_TRUE;

_error_exit:
    /* we should reset the I2C, or the I2C bus will keep on fail */
    _drv_i2c_Init();
    return GT_FALSE;
}


static LIB_I2C_HOSTTYPE_E drv_i2c_SearchHost(void)
{
    GT_U32  vLoop;
    const DRV_I2C_HOST_CTRL_S   *pHostCtrl;

    for (vLoop = 0; vLoop < COUNT_OF(aI2cHostCtrlTable); vLoop++)
    {
        pHostCtrl = &aI2cHostCtrlTable[vLoop];

        if (!pHostCtrl->pOpenDeviceFunc())
        {
            continue;
        }

        if (!pHostCtrl->pInitFunc())
        {
            continue;
        }

        if (!pHostCtrl->pCloseDeviceFunc())
        {
            continue;
        }

        pCurrentHost = pHostCtrl;
        return pHostCtrl->vHostType;
    }

    /* no I2C Host found */
    return LIB_I2C_HOSTTYPE_INVALID;
}

#endif


/* I2C API, for external usage */
#if 1

#define DRV_I2C_CHECK_INIT                                              \
    do {                                                                \
        if (pCurrentHost == NULL)                                       \
        {                                                               \
            if (DRV_I2C_Init() == LIB_I2C_HOSTTYPE_INVALID)             \
            {                                                           \
                return GT_FALSE;                                        \
            }                                                           \
        }                                                               \
    } while (0)


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
)
{
    GT_BOOL    result;

    if ((data_buf == NULL) || (data_len == 0))
    {
        return GT_FALSE;
    }

    DRV_I2C_CHECK_INIT;

    DRV_I2C_OPEN_DEVICE(GT_FALSE);
    if (pCurrentHost->vHostType == LIB_I2C_HOSTTYPE_USB)
    {
        /* for USB Device, use this API, to improve accessing speed */
        result = Drv_I2C_USB_RandomWrite(i2c_addr,
                                         offset,
                                         data_len,
                                         data_buf);
    }
    else
    {
        result = drv_i2c_WriteBytes(i2c_addr,
                                    1,
                                    &offset,
                                    data_len,
                                    data_buf);
    }
    DRV_I2C_CLOSE_DEVICE(GT_FALSE);

    return result;
}


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
)
{
    GT_BOOL    result;

    if ((data_buf == NULL) || (data_len == 0))
    {
        return GT_FALSE;
    }

    DRV_I2C_CHECK_INIT;

    DRV_I2C_OPEN_DEVICE(GT_FALSE);
    if (pCurrentHost->vHostType == LIB_I2C_HOSTTYPE_USB)
    {
        /* for USB Device, use this API, to improve accessing speed */
        result = Drv_I2C_USB_RandomRead(i2c_addr,
                                        offset,
                                        data_len,
                                        data_buf);
    }
    else
    {
        GT_U8   offset_buf[1];

        offset_buf[0] = offset;
        result = drv_i2c_ReadBytes(i2c_addr,
                                   COUNT_OF(offset_buf),
                                   offset_buf,
                                   data_len,
                                   data_buf);
    }
    DRV_I2C_CLOSE_DEVICE(GT_FALSE);

    return result;
}


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
)
{
    GT_BOOL    result;

    if ((data_buf == NULL) || (data_len == 0))
    {
        return GT_FALSE;
    }

    DRV_I2C_CHECK_INIT;

    DRV_I2C_OPEN_DEVICE(GT_FALSE);
    if (pCurrentHost->vHostType == LIB_I2C_HOSTTYPE_USB)
    {
        /* for USB Device, use this API, to improve accessing speed */
        result = Drv_I2C_USB_CurrentWrite(i2c_addr,
                                          data_len,
                                          data_buf);
    }
    else
    {
        result = drv_i2c_WriteBytes(i2c_addr,
                                    0,
                                    NULL,
                                    data_len,
                                    data_buf);
    }
    DRV_I2C_CLOSE_DEVICE(GT_FALSE);

    return result;
}


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
)
{
    GT_BOOL    result;

    if ((data_buf == NULL) || (data_len == 0))
    {
        return GT_FALSE;
    }

    DRV_I2C_CHECK_INIT;

    DRV_I2C_OPEN_DEVICE(GT_FALSE);
    if (pCurrentHost->vHostType == LIB_I2C_HOSTTYPE_USB)
    {
        /* for USB Device, use this API, to improve accessing speed */
        result = Drv_I2C_USB_CurrentRead(i2c_addr,
                                         data_len,
                                         data_buf);
    }
    else
    {
        result = drv_i2c_ReadBytes(i2c_addr,
                                   0,
                                   NULL,
                                   data_len,
                                   data_buf);
    }
    DRV_I2C_CLOSE_DEVICE(GT_FALSE);

    return result;
}


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
)
{
    GT_BOOL    result;

    DRV_I2C_CHECK_INIT;

    DRV_I2C_OPEN_DEVICE(GT_FALSE);
    if (pCurrentHost->vHostType == LIB_I2C_HOSTTYPE_USB)
    {
        result = Drv_I2C_USB_Detect(i2c_addr);
    }
    else
    {
        result = drv_i2c_Detect(i2c_addr);
    }
    DRV_I2C_CLOSE_DEVICE(GT_FALSE);

    return result;
}


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
)
{
    LIB_I2C_HOSTTYPE_E  vHostType;

    vHostType = drv_i2c_SearchHost();

    if (vHostType != LIB_I2C_HOSTTYPE_INVALID)
    {
        DRV_I2C_OPEN_DEVICE(LIB_I2C_HOSTTYPE_INVALID);
        _drv_i2c_Init();
        DRV_I2C_CLOSE_DEVICE(LIB_I2C_HOSTTYPE_INVALID);
    }

    return vHostType;
}

#endif

