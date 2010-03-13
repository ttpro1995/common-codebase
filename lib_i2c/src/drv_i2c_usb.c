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

#include <windows.h>
#include <typedef.h>
#include <drv_i2c.h>
#include <drv_i2c_usb.h>


#define I2C_USB_INDEX   0   /* I2C   */
#define I2C_USB_MODE    0   /* 20KHz */

#define I2C_USB_OUT_SCL_BIT     18
#define I2C_USB_OUT_SDA_BIT     19
#define I2C_USB_IN_SDA_BIT      23


/* CH341A API */
#if 1

#define mCH341A_CMD_I2C_STREAM      0xAA
#define mCH341A_CMD_I2C_STM_STA     0x74
#define mCH341A_CMD_I2C_STM_END     0x00
#define mCH341A_CMD_I2C_STM_STO     0x75
#define mCH341A_CMD_I2C_STM_OUT     0x80

#define mCH341_DEVICE_ARRIVAL       3       /* 设备插入事件,已经插入 */
#define mCH341_DEVICE_REMOVE_PEND   1       /* 设备将要拔出          */
#define mCH341_DEVICE_REMOVE        0       /* 设备拔出事件,已经拔出 */


#define LIB_CH341DLL_NAME   "ch341dll.dll"

static HINSTANCE  hLib_ch341dll;

typedef BOOL _stdcall (*p_CH341SetStream)
(
    ULONG       iIndex,
    ULONG       iMode
);
static p_CH341SetStream CH341SetStream;
#define CH341SetStream_FUNC_NAME    "CH341SetStream"

typedef HANDLE _stdcall (*p_CH341OpenDevice)
(
    ULONG       iIndex
);
static p_CH341OpenDevice CH341OpenDevice;
#define CH341OpenDevice_FUNC_NAME    "CH341OpenDevice"

typedef VOID _stdcall (*p_CH341CloseDevice)
(
    ULONG       iIndex
);
static p_CH341CloseDevice CH341CloseDevice;
#define CH341CloseDevice_FUNC_NAME    "CH341CloseDevice"

typedef BOOL _stdcall (*p_CH341StreamI2C)
(
    ULONG       iIndex,
    ULONG       iWriteLength,
    PVOID       iWriteBuffer,
    ULONG       iReadLength,
    PVOID       oReadBuffer
);
static p_CH341StreamI2C CH341StreamI2C;
#define CH341StreamI2C_FUNC_NAME    "CH341StreamI2C"

typedef BOOL _stdcall (*p_CH341GetInput)
(
    ULONG       iIndex,
    PULONG      iStatus
);
static p_CH341GetInput CH341GetInput;
#define CH341GetInput_FUNC_NAME    "CH341GetInput"

typedef BOOL _stdcall (*p_CH341SetOutput)
(
    ULONG       iIndex,
    ULONG       iEnable,
    ULONG       iSetDirOut,
    ULONG       iSetDataOut
);
static p_CH341SetOutput CH341SetOutput;
#define CH341SetOutput_FUNC_NAME    "CH341SetOutput"

typedef BOOL _stdcall (*p_CH341ReadI2C)
(
    ULONG       iIndex,
    UCHAR       iDevice,
    UCHAR       iAddr,
    PUCHAR      oByte
);
static p_CH341ReadI2C CH341ReadI2C;
#define CH341ReadI2C_FUNC_NAME    "CH341ReadI2C"

typedef BOOL _stdcall (*p_CH341WriteData)
(
    ULONG       iIndex,
    PVOID       iBuffer,
    PULONG      ioLength
);
static p_CH341WriteData CH341WriteData;
#define CH341WriteData_FUNC_NAME    "CH341WriteData"

typedef BOOL _stdcall (*p_CH341WriteRead)
(
    ULONG       iIndex,
    ULONG       iWriteLength,
    PVOID       iWriteBuffer,
    ULONG       iReadStep,
    ULONG       iReadTimes,
    PULONG      oReadLength,
    PVOID       oReadBuffer
);
static p_CH341WriteRead CH341WriteRead;
#define CH341WriteRead_FUNC_NAME    "CH341WriteRead"

typedef void (*NotifyCallBack_Func)(ULONG iEventStatus);
typedef BOOL _stdcall (*p_CH341SetDeviceNotify)
(
    ULONG               iIndex,
    const char         *iDeviceID,
    NotifyCallBack_Func iNotifyRoutine
);
static p_CH341SetDeviceNotify CH341SetDeviceNotify;
#define CH341SetDeviceNotify_FUNC_NAME    "CH341SetDeviceNotify"

#endif


static int vPrevStatus = (1<<I2C_USB_OUT_SCL_BIT) | (1<<I2C_USB_OUT_SDA_BIT);



static GT_BOOL _load_dll(void)
{
    static GT_BOOL b_init = GT_FALSE;

    if (!b_init)
    {
        /* Load the library */
        hLib_ch341dll = LoadLibrary(LIB_CH341DLL_NAME);
        if (hLib_ch341dll == NULL)
        {
            printf("Load Library %s Failed.\n", LIB_CH341DLL_NAME);
            return GT_FALSE;
        }

        CH341SetStream = (p_CH341SetStream)GetProcAddress(hLib_ch341dll, CH341SetStream_FUNC_NAME);
        if (CH341SetStream == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341SetStream_FUNC_NAME);
            return GT_FALSE;
        }

        CH341OpenDevice = (p_CH341OpenDevice)GetProcAddress(hLib_ch341dll, CH341OpenDevice_FUNC_NAME);
        if (CH341OpenDevice == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341OpenDevice_FUNC_NAME);
            return GT_FALSE;
        }

        CH341CloseDevice = (p_CH341CloseDevice)GetProcAddress(hLib_ch341dll, CH341CloseDevice_FUNC_NAME);
        if (CH341CloseDevice == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341CloseDevice_FUNC_NAME);
            return GT_FALSE;
        }

        CH341StreamI2C = (p_CH341StreamI2C)GetProcAddress(hLib_ch341dll, CH341StreamI2C_FUNC_NAME);
        if (CH341StreamI2C == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341StreamI2C_FUNC_NAME);
            return GT_FALSE;
        }

        CH341GetInput = (p_CH341GetInput)GetProcAddress(hLib_ch341dll, CH341GetInput_FUNC_NAME);
        if (CH341GetInput == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341GetInput_FUNC_NAME);
            return GT_FALSE;
        }

        CH341SetOutput = (p_CH341SetOutput)GetProcAddress(hLib_ch341dll, CH341SetOutput_FUNC_NAME);
        if (CH341SetOutput == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341SetOutput_FUNC_NAME);
            return GT_FALSE;
        }

        CH341ReadI2C = (p_CH341ReadI2C)GetProcAddress(hLib_ch341dll, CH341ReadI2C_FUNC_NAME);
        if (CH341ReadI2C == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341ReadI2C_FUNC_NAME);
            return GT_FALSE;
        }

        CH341WriteData = (p_CH341WriteData)GetProcAddress(hLib_ch341dll, CH341WriteData_FUNC_NAME);
        if (CH341WriteData == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341WriteData_FUNC_NAME);
            return GT_FALSE;
        }

        CH341WriteRead = (p_CH341WriteRead)GetProcAddress(hLib_ch341dll, CH341WriteRead_FUNC_NAME);
        if (CH341WriteRead == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341WriteRead_FUNC_NAME);
            return GT_FALSE;
        }

        CH341SetDeviceNotify = (p_CH341SetDeviceNotify)GetProcAddress(hLib_ch341dll, CH341SetDeviceNotify_FUNC_NAME);
        if (CH341SetDeviceNotify == NULL)
        {
            printf("GetProcAddress for %s Failed.\n", CH341SetDeviceNotify_FUNC_NAME);
            return GT_FALSE;
        }

        b_init = GT_TRUE;
    }

    return GT_TRUE;
}


static void _callback_SetDeviceNotify(ULONG iEventStatus)
{
    switch (iEventStatus)
    {
        case mCH341_DEVICE_ARRIVAL:
            if (CH341OpenDevice(I2C_USB_INDEX) == INVALID_HANDLE_VALUE)
            {
                printf("\n %s(line%d): CH341OpenDevice failed!", __func__, __LINE__);
            }
            else
            {
                if (!CH341SetStream(I2C_USB_INDEX, I2C_USB_MODE))
                {
                    printf("\n %s(line%d): CH341SetStream failed!", __func__, __LINE__);
                    exit(-1);
                }
            }
            break;

        case mCH341_DEVICE_REMOVE:
            CH341CloseDevice(I2C_USB_INDEX);
            break;

        default:
            /* do nothing */
            break;
    }
}


GT_BOOL DRV_I2C_USB_OpenDevice(void)
{
    return GT_TRUE;
}


GT_BOOL DRV_I2C_USB_CloseDevice(void)
{
    return GT_TRUE;
}


void DRV_I2C_USB_SetSCL(int vData)
{
    if (READ_BIT(vPrevStatus, I2C_USB_OUT_SCL_BIT) != vData)
    {
        GT_U32  vEnableBit;

        /* Note: set bit 4 to 1, to enable bit16-23 output */
        vEnableBit = (1 << 4) | (1 << I2C_USB_OUT_SCL_BIT);

        if (vData)
        {
            SET_BIT(vPrevStatus, I2C_USB_OUT_SCL_BIT);
        }
        else
        {
            CLR_BIT(vPrevStatus, I2C_USB_OUT_SCL_BIT);
        }

        CH341SetOutput(I2C_USB_INDEX, vEnableBit, 0, vPrevStatus);
    }
}

void DRV_I2C_USB_SetSDA(int vData)
{
    if (READ_BIT(vPrevStatus, I2C_USB_OUT_SDA_BIT) != vData)
    {
        GT_U32  vEnableBit;

        /* Note: set bit 4 to 1, to enable bit16-23 output */
        vEnableBit = (1 << 4) | (1 << I2C_USB_OUT_SDA_BIT);

        if (vData)
        {
            SET_BIT(vPrevStatus, I2C_USB_OUT_SDA_BIT);
        }
        else
        {
            CLR_BIT(vPrevStatus, I2C_USB_OUT_SDA_BIT);
        }

        CH341SetOutput(I2C_USB_INDEX, vEnableBit, 0, vPrevStatus);
    }
}


int DRV_I2C_USB_GetSDA(void)
{
    unsigned long  vStatus;

    if (!CH341GetInput(I2C_USB_INDEX, &vStatus))
    {
        return 1;
    }

    return READ_BIT(vStatus, I2C_USB_IN_SDA_BIT);
}


int DRV_I2C_USB_GetSCL(void)
{
    /* currently, has no way to read the SCL line level,
     * so, only return the OUTPUT SCL line level.
     */
    return READ_BIT(vPrevStatus, I2C_USB_OUT_SCL_BIT);
}


GT_BOOL DRV_I2C_USB_Init(void)
{
    if (!_load_dll())
    {
        return GT_FALSE;
    }

    CH341CloseDevice(I2C_USB_INDEX);
    if (CH341OpenDevice(I2C_USB_INDEX) == INVALID_HANDLE_VALUE)
    {
        return GT_FALSE;
    }

    if (!CH341SetStream(I2C_USB_INDEX, I2C_USB_MODE))
    {
        return GT_FALSE;
    }

    /* Setup Notify Routine */
    CH341SetDeviceNotify(I2C_USB_INDEX, NULL, _callback_SetDeviceNotify);

    return GT_TRUE;
}


#if 1

#define READ_BUFFER_LEN     1024     /* Byte */
#define WRITE_BUFFER_LEN    1024     /* Byte */

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
)
{
    int     vUsbStatus;
    GT_U8   aOutBuf[WRITE_BUFFER_LEN], aInBuf[READ_BUFFER_LEN];

    if (vLen > sizeof(aInBuf))
    {
        return GT_FALSE;
    }

    /* sending Addr+W & Offset */
    aOutBuf[0] = (vAddr & 0xFE);
    aOutBuf[1] = vOffset;
    vUsbStatus = CH341StreamI2C(I2C_USB_INDEX, 2, aOutBuf, 0, aInBuf);
    if (vUsbStatus == 0)
    {
        return GT_FALSE;
    }

    /* sending Addr+R & read */
    aOutBuf[0] = (vAddr & 0xFE) | 0x1;
    vUsbStatus = CH341StreamI2C(I2C_USB_INDEX, 1, aOutBuf, vLen, aInBuf);
    if (vUsbStatus == 0)
    {
        return GT_FALSE;
    }

    memcpy(pReadBuf, aInBuf, vLen);
    return GT_TRUE;
}


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
)
{
    int     vUsbStatus;
    GT_U8   aOutBuf[WRITE_BUFFER_LEN], aInBuf[READ_BUFFER_LEN];

    if (vLen > sizeof(aInBuf))
    {
        return GT_FALSE;
    }

    /* sending Addr+R & read */
    aOutBuf[0] = (vAddr & 0xFE) | 0x1;
    vUsbStatus = CH341StreamI2C(I2C_USB_INDEX, 1, aOutBuf, vLen, aInBuf);
    if (vUsbStatus == 0)
    {
        return GT_FALSE;
    }

    memcpy(pReadBuf, aInBuf, vLen);
    return GT_TRUE;
}


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
)
{
    int     vUsbStatus;
    GT_U8   aOutBuf[WRITE_BUFFER_LEN], aInBuf[READ_BUFFER_LEN];

    if (vLen > (sizeof(aOutBuf)-2))
    {
        return GT_FALSE;
    }

    /* sending Addr+W & offset & write */
    aOutBuf[0] = (vAddr & 0xFE);
    aOutBuf[1] = vOffset;
    memcpy(&aOutBuf[2], pReadBuf, vLen);
    vUsbStatus = CH341StreamI2C(I2C_USB_INDEX, vLen+2, aOutBuf, 0, aInBuf);
    if (vUsbStatus == 0)
    {
        return GT_FALSE;
    }
    return GT_TRUE;
}


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
)
{
    int     vUsbStatus;
    GT_U8   aOutBuf[WRITE_BUFFER_LEN], aInBuf[READ_BUFFER_LEN];

    if (vLen > (sizeof(aInBuf)-1))
    {
        return GT_FALSE;
    }

    /* sending Addr+W & write */
    aOutBuf[0] = (vAddr & 0xFE);
    memcpy(&aOutBuf[1], pReadBuf, vLen);
    vUsbStatus = CH341StreamI2C(I2C_USB_INDEX, vLen+1, aOutBuf, 0, aInBuf);
    if (vUsbStatus == 0)
    {
        return GT_FALSE;
    }
    return GT_TRUE;
}


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
)
{
    unsigned long   vLen;
    GT_U8           aOutBuf[WRITE_BUFFER_LEN], aInBuf[READ_BUFFER_LEN];

    /* send address and check ACK */
    aOutBuf[0] = mCH341A_CMD_I2C_STREAM;
    aOutBuf[1] = mCH341A_CMD_I2C_STM_STA;
    aOutBuf[2] = mCH341A_CMD_I2C_STM_OUT;
    aOutBuf[3] = (vAddr & 0xFE);
    aOutBuf[4] = mCH341A_CMD_I2C_STM_STO;
    aOutBuf[5] = mCH341A_CMD_I2C_STM_END;
    vLen       = 0;
    if (CH341WriteRead(I2C_USB_INDEX, 6, aOutBuf, 4, 1, &vLen, aInBuf))
    {
        if ((vLen != 0)
            && ((aInBuf[vLen - 1] & 0x80) == 0))
        {
            return GT_TRUE;
        }
    }

    return GT_FALSE;
}

#endif

