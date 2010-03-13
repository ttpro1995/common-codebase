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
 *   lib_i2c.c
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.11        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#define _LIB_I2C_BUILD_DLL_

#include <typedef.h>
#include <drv_i2c.h>
#include <drv_i2c_lpt.h>
#include <lib_i2c.h>


#define DLL_MAJOR_VER       0x02
#define DLL_MINOR_VER       0x00
#define DLL_DEV_VER         0x00
#define DLL_BUILD_VER       0x00

#if 1


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_RandomWriteByte
 * DESCRIPTION:
 *      I2C Bus Random write byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 *      vData    : I2C data to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE     : I2C write byte success.
 *      FALSE    : I2C write byte fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_BOOL LIB_I2C_RandomWriteByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset,
    IN        GT_U8     vData
)
{
    return DRV_I2C_RandomWrite(vI2cAddr, vOffset, 1, &vData);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_RandomReadByte
 * DESCRIPTION:
 *      I2C Bus Random read byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 * OUTPUT:
 *      None
 * RETURN:
 *      -1   : Random Read Byte Fail;
 *      else : The Read Byte Data. (only bit[8:0] valid)
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_S32 LIB_I2C_RandomReadByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset
)
{
    GT_U8   vData;

    if (DRV_I2C_RandomRead(vI2cAddr, vOffset, 1, &vData))
    {
        return (GT_S32)vData;
    }
    else
    {
        return -1;
    }
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_CurrentWriteByte
 * DESCRIPTION:
 *      I2C Bus current write byte driver.
 * INPUT:
 *      vI2cAddr    : I2C chip address.
 *      vData       : I2C data to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE        : I2C write byte success.
 *      FALSE       : I2C write byte fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_BOOL LIB_I2C_CurrentWriteByte
(
    IN  GT_U8     vI2cAddr,
    IN  GT_U8     vData
)
{
    return DRV_I2C_CurrentWrite(vI2cAddr, 1, &vData);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_CurrentReadByte
 * DESCRIPTION:
 *      I2C Bus Current read byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 * OUTPUT:
 *      None
 * RETURN:
 *      -1   : Current Read Byte Fail;
 *      else : The Read Byte Data. (only bit[8:0] valid)
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_S32 LIB_I2C_CurrentReadByte
(
    IN        GT_U8     vI2cAddr
)
{
    GT_U8   vData;

    if (DRV_I2C_CurrentRead(vI2cAddr, 1, &vData))
    {
        return (GT_S32)vData;
    }
    else
    {
        return -1;
    }
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_RandomWrite
 * DESCRIPTION:
 *      I2C Bus Random write driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      offset      : I2C chip offset to be start to write.
 *      data_buf    : I2C data buffer to be written.
 *      data_len    : I2C data buffer length to be written.
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
DECLEAR_DLL GT_BOOL LIB_I2C_RandomWrite
(
    IN        GT_U8     i2c_addr,
    IN        GT_U8     offset,
    IN        GT_U32    data_len,
    IN  const GT_U8    *data_buf
)
{
    return DRV_I2C_RandomWrite(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_RandomRead
 * DESCRIPTION:
 *      I2C Bus Random read driver.
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
DECLEAR_DLL GT_BOOL LIB_I2C_RandomRead
(
    IN  GT_U8     i2c_addr,
    IN  GT_U8     offset,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
)
{
    return DRV_I2C_RandomRead(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_CurrentWrite
 * DESCRIPTION:
 *      I2C Bus current write driver.
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
DECLEAR_DLL GT_BOOL LIB_I2C_CurrentWrite
(
    IN       GT_U8     i2c_addr,
    IN       GT_U32    data_len,
    IN const GT_U8    *data_buf
)
{
    return DRV_I2C_CurrentWrite(i2c_addr, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_CurrentRead
 * DESCRIPTION:
 *      I2C Bus current read driver.
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
DECLEAR_DLL GT_BOOL LIB_I2C_CurrentRead
(
    IN  GT_U8     i2c_addr,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
)
{
    return DRV_I2C_CurrentRead(i2c_addr, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_Detect
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
DECLEAR_DLL GT_BOOL LIB_I2C_Detect
(
    IN GT_U8    i2c_addr
)
{
    return DRV_I2C_Detect(i2c_addr);
}


/******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_Init
 * DESCRIPTION:
 *      I2C init driver.
 * PARAMETERS:
 *      None
 * RETURN:
 *      The I2C Host Type.
 *      If I2C lib init fail, return LIB_I2C_HOSTTYPE_INVALID.
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.10.15     Panda Xiong       Create
 ******************************************************************************/
DECLEAR_DLL LIB_I2C_HOSTTYPE_E LIB_I2C_Init
(
    void
)
{
    return DRV_I2C_Init();
}


/******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_SetLptAddress
 * DESCRIPTION:
 *      Set LPT Base Address.
 * PARAMETERS:
 *      vLptAddress : LPT Base Address to be set.
 * RETURN:
 *      TRUE  : Success;
 *      FALSE : Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.15        PANDA         Create/Update
 *****************************************************************************/
DECLEAR_DLL GT_BOOL LIB_I2C_SetLptAddress
(
    IN GT_U32   vLptAddress
)
{
    return DRV_I2C_LPT_SetLptAddress(vLptAddress);
}


/******************************************************************************
 * FUNCTION NAME:
 *      LIB_I2C_GetDllVersion
 * DESCRIPTION:
 *      Get i2c lib dll version.
 * PARAMETERS:
 *      None
 * RETURN:
 *      lib_i2c.dll version; (32-bit)
 *      formatting:
 *              ---------------------------------------------
 *        bit  | 31      24|23       16|15      8|7         0|
 *             |---------------------------------------------|
 *     Version | Major Ver | Minor Ver | Dev Ver | Build Ver |
 *              ---------------------------------------------
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.10.15     Panda Xiong       Create
 ******************************************************************************/
DECLEAR_DLL GT_U32 LIB_I2C_GetDllVersion
(
    void
)
{
    GT_U32  vRev = 0;

    vRev |= ((DLL_MAJOR_VER & 0xFF) << 24);
    vRev |= ((DLL_MINOR_VER & 0xFF) << 16);
    vRev |= ((DLL_DEV_VER   & 0xFF) <<  8);
    vRev |= ((DLL_BUILD_VER & 0xFF) <<  0);

    return vRev;
}


#endif


/* this is defined for short-calling usage, such as python/C# */
#if 1

/*******************************************************************************
 * FUNCTION NAME:
 *      RandomWriteByte
 * DESCRIPTION:
 *      I2C Bus Random write byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 *      vData    : I2C data to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE     : I2C write byte success.
 *      FALSE    : I2C write byte fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_BOOL RandomWriteByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset,
    IN        GT_U8     vData
)
{
    return DRV_I2C_RandomWrite(vI2cAddr, vOffset, 1, &vData);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      RandomReadByte
 * DESCRIPTION:
 *      I2C Bus Random read byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 * OUTPUT:
 *      None
 * RETURN:
 *      -1   : Random Read Byte Fail;
 *      else : The Read Byte Data. (only bit[8:0] valid)
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_S32 RandomReadByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset
)
{
    GT_U8   vData;

    if (DRV_I2C_RandomRead(vI2cAddr, vOffset, 1, &vData))
    {
        return (GT_S32)vData;
    }
    else
    {
        return -1;
    }
}



/*******************************************************************************
 * FUNCTION NAME:
 *      CurrentWriteByte
 * DESCRIPTION:
 *      I2C Bus current write byte driver.
 * INPUT:
 *      vI2cAddr    : I2C chip address.
 *      vData       : I2C data to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE        : I2C write byte success.
 *      FALSE       : I2C write byte fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_BOOL CurrentWriteByte
(
    IN  GT_U8     vI2cAddr,
    IN  GT_U8     vData
)
{
    return DRV_I2C_CurrentWrite(vI2cAddr, 1, &vData);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      CurrentReadByte
 * DESCRIPTION:
 *      I2C Bus Current read byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 * OUTPUT:
 *      None
 * RETURN:
 *      -1   : Current Read Byte Fail;
 *      else : The Read Byte Data. (only bit[8:0] valid)
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_S32 CurrentReadByte
(
    IN        GT_U8     vI2cAddr
)
{
    GT_U8   vData;

    if (DRV_I2C_CurrentRead(vI2cAddr, 1, &vData))
    {
        return (GT_S32)vData;
    }
    else
    {
        return -1;
    }
}


/*******************************************************************************
 * FUNCTION NAME:
 *      RandomWriteBytes
 * DESCRIPTION:
 *      I2C Bus Random write driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      offset      : I2C chip offset to be start to write.
 *      data_buf    : I2C data buffer to be written.
 *      data_len    : I2C data buffer length to be written.
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
DECLEAR_DLL GT_BOOL RandomWriteBytes
(
    IN        GT_U8     i2c_addr,
    IN        GT_U8     offset,
    IN        GT_U32    data_len,
    IN  const GT_U8    *data_buf
)
{
    return DRV_I2C_RandomWrite(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      RandomReadBytes
 * DESCRIPTION:
 *      I2C Bus Random read driver.
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
DECLEAR_DLL GT_BOOL RandomReadBytes
(
    IN  GT_U8     i2c_addr,
    IN  GT_U8     offset,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
)
{
    return DRV_I2C_RandomRead(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      CurrentWriteBytes
 * DESCRIPTION:
 *      I2C Bus current write driver.
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
DECLEAR_DLL GT_BOOL CurrentWriteBytes
(
    IN       GT_U8     i2c_addr,
    IN       GT_U32    data_len,
    IN const GT_U8    *data_buf
)
{
    return DRV_I2C_CurrentWrite(i2c_addr, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      CurrentReadBytes
 * DESCRIPTION:
 *      I2C Bus current read driver.
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
DECLEAR_DLL GT_BOOL CurrentReadBytes
(
    IN  GT_U8     i2c_addr,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
)
{
    return DRV_I2C_CurrentRead(i2c_addr, data_len, data_buf);
}


#endif


#if 1

/*******************************************************************************
 * FUNCTION NAME:
 *      WriteByte
 * DESCRIPTION:
 *      I2C Bus Random write byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 *      vData    : I2C data to be written.
 * OUTPUT:
 *      None
 * RETURN:
 *      TRUE     : I2C write byte success.
 *      FALSE    : I2C write byte fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_BOOL WriteByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset,
    IN        GT_U8     vData
)
{
    return DRV_I2C_RandomWrite(vI2cAddr, vOffset, 1, &vData);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      ReadByte
 * DESCRIPTION:
 *      I2C Bus Random read byte driver.
 * INPUT:
 *      vI2cAddr : I2C chip address.
 *      vOffset  : I2C chip offset to be start to write.
 * OUTPUT:
 *      None
 * RETURN:
 *      -1   : Random Read Byte Fail;
 *      else : The Read Byte Data. (only bit[8:0] valid)
 * NOTES:
 *      N/A
 * HISTORY:
 *      Ver1.00     2008.08.21      Panda Xiong         Create
 ******************************************************************************/
DECLEAR_DLL GT_S32 ReadByte
(
    IN        GT_U8     vI2cAddr,
    IN        GT_U8     vOffset
)
{
    GT_U8   vData;

    if (DRV_I2C_RandomRead(vI2cAddr, vOffset, 1, &vData))
    {
        return (GT_S32)vData;
    }
    else
    {
        return -1;
    }
}


/*******************************************************************************
 * FUNCTION NAME:
 *      WriteBytes
 * DESCRIPTION:
 *      I2C Bus Random write driver.
 * INPUT:
 *      i2c_addr    : I2C chip address.
 *      offset      : I2C chip offset to be start to write.
 *      data_buf    : I2C data buffer to be written.
 *      data_len    : I2C data buffer length to be written.
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
DECLEAR_DLL GT_BOOL WriteBytes
(
    IN        GT_U8     i2c_addr,
    IN        GT_U8     offset,
    IN        GT_U32    data_len,
    IN  const GT_U8    *data_buf
)
{
    return DRV_I2C_RandomWrite(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      ReadBytes
 * DESCRIPTION:
 *      I2C Bus Random read driver.
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
DECLEAR_DLL GT_BOOL ReadBytes
(
    IN  GT_U8     i2c_addr,
    IN  GT_U8     offset,
    IN  GT_U32    data_len,
    OUT GT_U8    *data_buf
)
{
    return DRV_I2C_RandomRead(i2c_addr, offset, data_len, data_buf);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      Detect
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
DECLEAR_DLL GT_BOOL Detect
(
    IN GT_U8    i2c_addr
)
{
    return DRV_I2C_Detect(i2c_addr);
}


/******************************************************************************
 * FUNCTION NAME:
 *      Init
 * DESCRIPTION:
 *      I2C init driver.
 * PARAMETERS:
 *      None
 * RETURN:
 *      The I2C Host Type.
 *      If I2C lib init fail, return LIB_I2C_HOSTTYPE_INVALID.
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.10.15     Panda Xiong       Create
 ******************************************************************************/
DECLEAR_DLL LIB_I2C_HOSTTYPE_E Init
(
    void
)
{
    return DRV_I2C_Init();
}


/******************************************************************************
 * FUNCTION NAME:
 *      SetLptAddress
 * DESCRIPTION:
 *      Set LPT Base Address.
 * PARAMETERS:
 *      vLptAddress : LPT Base Address to be set.
 * RETURN:
 *      TRUE  : Success;
 *      FALSE : Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.8.15        PANDA         Create/Update
 *****************************************************************************/
DECLEAR_DLL GT_BOOL SetLptAddress
(
    IN GT_U32   vLptAddress
)
{
    return DRV_I2C_LPT_SetLptAddress(vLptAddress);
}


/******************************************************************************
 * FUNCTION NAME:
 *      GetDllVersion
 * DESCRIPTION:
 *      Get i2c lib dll version.
 * PARAMETERS:
 *      None
 * RETURN:
 *      lib_i2c.dll version; (32-bit)
 *      formatting:
 *              ---------------------------------------------
 *        bit  | 31      24|23       16|15      8|7         0|
 *             |---------------------------------------------|
 *     Version | Major Ver | Minor Ver | Dev Ver | Build Ver |
 *              ---------------------------------------------
 * NOTES:
 *      None
 * HISTORY:
 *      V1.00     2008.10.15     Panda Xiong       Create
 ******************************************************************************/
DECLEAR_DLL GT_U32 GetDllVersion
(
    void
)
{
    GT_U32  vRev = 0;

    vRev |= ((DLL_MAJOR_VER & 0xFF) << 24);
    vRev |= ((DLL_MINOR_VER & 0xFF) << 16);
    vRev |= ((DLL_DEV_VER   & 0xFF) <<  8);
    vRev |= ((DLL_BUILD_VER & 0xFF) <<  0);

    return vRev;
}


#endif

