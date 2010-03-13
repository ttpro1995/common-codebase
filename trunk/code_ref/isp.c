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
 *   isp.c
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   This file provide all ISP API, for programming AVR usage.
 * HISTORY:
 *   2009.12.25        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#define  ISP_GLOBAL
#include "Includes.h"


/* Porting part for ISP */
#if 1

#define CONST   const

#define ISP_TARGET_CHIP_CONFIG                              \
{                                                           \
    /* chip setting is for ATTiny13 */                      \
    32,     /* Byte, Flash  Page Length        */           \
    4,      /* Byte, EEPROM Page Length        */           \
                                                            \
    2,      /* Byte, Fuse Bytes Length         */           \
    2,      /* Byte, Calibration Bytes Length  */           \
}


#define ISP_OpenSpiBus()                                    \
    do {                                                    \
        /* SPI Master, CKPHA = 0, CKPOL = 0 */              \
        SPI0CFG = 0x40;                                     \
                                                            \
        /* Reduce SPI Clock Rate = 94.96KHz < (worst case)  \
         *                                                  \
         * (worst case) = 4.8MHz / 8 / 4 = 150KHz           \
         */                                                 \
        SPI0CKR = 0x80;                                     \
                                                            \
        /* configure to 3-wire master mode */               \
        SPI0CN_bit.NSSMD1 = 0;                              \
        SPI0CN_bit.NSSMD0 = 0;                              \
                                                            \
        /* enable SPI */                                    \
        SPI0CN_bit.SPIEN  = 1;                              \
    } while (0)

#define ISP_CloseSpiBus()                                   \
    do {                                                    \
      /* Config SPI Bus to normal status */                 \
                                                            \
        /* disable SPI */                                   \
        SPI0CN_bit.SPIEN = 0;                               \
                                                            \
        /* SPI Master, CKPHA = 1, CKPOL = 0 */              \
        SPI0CFG = 0x60;                                     \
                                                            \
        /* SPI Clock Rate = 6.125 MHz */                    \
        SPI0CKR = 0x01;                                     \
                                                            \
        /* 4-wire single mode, SSN is high */               \
        SPI0CN_bit.NSSMD1 = 1;                              \
        SPI0CN_bit.NSSMD0 = 1;                              \
    } while (0)


#define ISP_LockTarget()        do { REST_PIN = 0; } while (0)
#define ISP_ReleaseTarget()     do { REST_PIN = 1; } while (0)

#define ISP_ReadByte()          (SPI0DAT)

#define ISP_WriteByte(_byte)                            \
    do {                                                \
        /* check previous SPI interrupt flag            \
         *  is cleared or not,                          \
         * if not cleared, clear it.                    \
         */                                             \
        if (SPI0CN_bit.SPIF == 1)                       \
        {                                               \
            SPI0CN_bit.SPIF = 0;                        \
        }                                               \
                                                        \
        /* write byte data */                           \
        SPI0DAT = _byte;                                \
                                                        \
        /* waiting for SPI writting finished */         \
        while (SPI0CN_bit.SPIF == 0)                    \
        {}                                              \
                                                        \
        /* SPI writting finished,                       \
         * clear this interrupt flag.                   \
         */                                             \
        SPI0CN_bit.SPIF = 0;                            \
    } while (0)

#endif


#define ISP_WriteBytes(_buf, _len)                      \
    do {                                                \
        UINT8   vLoop;                                  \
                                                        \
        for (vLoop = 0; vLoop < (_len); vLoop++)        \
        {                                               \
            ISP_WriteByte((_buf)[vLoop]);               \
        }                                               \
    } while (0)


typedef struct
{
    UINT8   vFlashPageLen;      /* Flash  Page Length, count at Byte */
    UINT8   vEepromPageLen;     /* EEPROM Page Length, count at Byte */

    UINT8   vFuseByteLength;    /* Fuse Byte Length                  */
    UINT8   vCaliByteLength;    /* Calibration Byte Length           */
} ISP_CHIP_CFG_T;

static ISP_CHIP_CFG_T _chipSetting = ISP_TARGET_CHIP_CONFIG;


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadBusy
 * DESCRIPTION:
 *      Read Target Busy status.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      Target Busy status:
 *       =TRUE  : Target busy;
 *       =FALSE : Target ready.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
static BOOL isp_ReadBusy(void)
{
    CONST UINT8 aCmdTable[] = {0xF0, 0xE0, 0x00, 0x00};

    /* Read Target Busy Status */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
    return READ_BIT(ISP_ReadByte(), 0);
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_EnableProgramming
 * DESCRIPTION:
 *      Enable In-System-Programming.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      TRUE  : Enable Programming Successfully;
 *      FALSE : Enable Programming Fail;
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_EnableProgramming(void)
{
    BOOL        bConnected = FALSE;
    UINT8       vLoop;
    CONST UINT8 aCmdTable[] = {0xAC, 0x53, 0x00, 0x00};

    for (vLoop = 0; vLoop < COUNT_OF(aCmdTable); vLoop++)
    {
        ISP_WriteByte(aCmdTable[vLoop]);

        /* read the echo byte data */
        if (vLoop == 2)
        {
            if (ISP_ReadByte() == aCmdTable[1])
            {
                bConnected = TRUE;
            }
        }
    }

    return bConnected;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ChipErase
 * DESCRIPTION:
 *      Erase total chip, including EEPROM.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      TRUE   : Erase Chip Success;
 *      FALSE  : Erase Chip Fail.
 * NOTES:
 *      The EEPROM will be erased, only if it is set to none preserved.
 *      See Fuse Bytes of datasheet for details.
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ChipErase(void)
{
    CONST UINT8 aCmdTable[] = {0xAC, 0x80, 0x00, 0x00};

    /* send Chip Erase Command */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

    /* wait for target done */
    while (isp_ReadBusy())
    {}

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadFlash
 * DESCRIPTION:
 *      Read Flash.
 * PARAMETERS:
 *      vStartAddr  : Start Address to be read;
 *      vLen        : Read Flash length, count at Byte;
 *      pBuf        : Read data buffer.
 * RETURN:
 *      TRUE   : Read Flash Success;
 *      FALSE  : Read Flash Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadFlash
(
    IN  UINT16  vStartAddr,
    IN  UINT16  vLen,
    OUT UINT8  *pBuf
)
{
    UINT16  vTotalWords;
    UINT16  vAddrLoop;
    UINT8   aCmdTable[] = {0x20, 0x00, 0x00, 0x00};

    /* format length to 16-bit */
    vTotalWords = (vLen+1) >> 1;

    for (vAddrLoop = vStartAddr;
         vAddrLoop < (vStartAddr + vTotalWords);
         vAddrLoop++)
    {
        /* set address bits */
        aCmdTable[1] = ((vAddrLoop >> 8) & 0xFF);   /* high address bits */
        aCmdTable[2] = ((vAddrLoop >> 0) & 0xFF);   /* low  address bits */

        /* read Low Byte of this address */
        CLR_BIT(aCmdTable[0], 3);   /* select Low Byte of this address  */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();

        /* read High Byte of this address */
        SET_BIT(aCmdTable[0], 3);   /* select High Byte of this address */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_WriteFlashPage
 * DESCRIPTION:
 *      Write One Flash Page.
 * PARAMETERS:
 *      vStartAddr : Start Address to be written;
 *      pBuf       : Flash data buffer to be written;
 * RETURN:
 *      TRUE   : Write One Flash Page Success;
 *      FALSE  : Write One Flash Page Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_WriteFlashPage
(
    IN       UINT16 vStartAddr,
    IN const UINT8 *pBuf
)
{
    UINT16  vTotalWords;
    UINT16  vLoop;
    UINT8   aCmdTable[] = {0x40, 0x00, 0x00, 0x00};

    /* format length to 16-bit */
    vTotalWords = _chipSetting.vFlashPageLen >> 1;


/* load flash page */
    for (vLoop = 0; vLoop < vTotalWords; vLoop++)
    {
        /* set address bits */
        aCmdTable[1] = (vLoop >> 8) & 0xFF;
        aCmdTable[2] = (vLoop >> 0) & 0xFF;

        /* load Low Byte of this address */
        CLR_BIT(aCmdTable[0], 3);   /* select Low Byte of this address  */
        aCmdTable[3] = *pBuf++;
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

        /* load High Byte of this address */
        SET_BIT(aCmdTable[0], 3);   /* select High Byte of this address */
        aCmdTable[3] = *pBuf++;
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
    }


/* write flash page */
    vStartAddr >>= 1;
    /* set commands */
    aCmdTable[0] = 0x4C;
    aCmdTable[1] = ((vStartAddr >> 8) & 0xFF);   /* high address bits */
    aCmdTable[2] = ((vStartAddr >> 0) & 0xFF);   /* low  address bits */
    aCmdTable[3] = 0x00;

    /* write flash page */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

    /* wait for target done */
    while (isp_ReadBusy())
    {}

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadEeprom
 * DESCRIPTION:
 *      Read EEPROM.
 * PARAMETERS:
 *      vStartAddr  : Start Address to be read;
 *      vLen        : Read EEPROM length;
 *      pBuf        : Read data buffer.
 * RETURN:
 *      TRUE   : Read EEPROM Success;
 *      FALSE  : Read EEPROM Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadEeprom
(
    IN  UINT16  vStartAddr,
    IN  UINT16  vLen,
    OUT UINT8  *pBuf
)
{
    UINT16  vAddrLoop;
    UINT8   aCmdTable[] = {0xA0, 0x00, 0x00, 0x00};

    for (vAddrLoop = vStartAddr;
         vAddrLoop < (vStartAddr + vLen);
         vAddrLoop++)
    {
        /* set address bits */
        aCmdTable[1] = ((vAddrLoop >> 8) & 0xFF);   /* high address bits */
        aCmdTable[2] = ((vAddrLoop >> 0) & 0xFF);   /* low  address bits */

        /* read EEPROM data */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_WriteEepromBytes
 * DESCRIPTION:
 *      Write n Bytes EEPROM.
 * PARAMETERS:
 *      vStartAddr  : Start Address to be written;
 *      vLen        : Write EEPROM length;
 *      pBuf        : Write data buffer.
 * RETURN:
 *      TRUE   : Write n Bytes EEPROM Success;
 *      FALSE  : Write n Bytes EEPROM Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_WriteEepromBytes
(
    IN  UINT16  vStartAddr,
    IN  UINT16  vLen,
    OUT UINT8  *pBuf
)
{
    UINT16  vAddrLoop;
    UINT8   aCmdTable[] = {0xC0, 0x00, 0x00, 0x00};

    for (vAddrLoop = vStartAddr;
         vAddrLoop < (vStartAddr + vLen);
         vAddrLoop++)
    {
        /* set address bits */
        aCmdTable[1] = ((vAddrLoop >> 8) & 0xFF);   /* high address bits */
        aCmdTable[2] = ((vAddrLoop >> 0) & 0xFF);   /* low  address bits */

        /* set EEPROM data */
        aCmdTable[3] = *pBuf++;

        /* write EEPROM data */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

        /* wait for target done */
        while (isp_ReadBusy())
        {}
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_WriteEepromPage
 * DESCRIPTION:
 *      Write One EEPROM Page.
 * PARAMETERS:
 *      vStartAddr  : Start Address to be written;
 *      pBuf        : Write data buffer.
 * RETURN:
 *      TRUE   : Write One EEPROM Page Success;
 *      FALSE  : Write One EEPROM Page Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_WriteEepromPage
(
    IN  UINT16  vStartAddr,
    OUT UINT8  *pBuf
)
{
    UINT16  vTotalWords;
    UINT8   vLoop;
    UINT8   aCmdTable[] = {0xC1, 0x00, 0x00, 0x00};

    /* format length to 16-bit */
    vTotalWords = _chipSetting.vFlashPageLen >> 1;


/* load EEPROM page */
    for (vLoop = 0; vLoop < vTotalWords; vLoop++)
    {
        /* set address bits */
        aCmdTable[1] = (vLoop >> 8) & 0xFF;
        aCmdTable[2] = (vLoop >> 0) & 0xFF;

        /* load EEPROM data of this address */
        aCmdTable[3] = *pBuf++;
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
    }


/* write EEPROM page */
    /* set commands */
    aCmdTable[0] = 0xC2;
    aCmdTable[1] = ((vStartAddr >> 8) & 0xFF);   /* high address bits */
    aCmdTable[2] = ((vStartAddr >> 0) & 0xFF);   /* low  address bits */
    aCmdTable[3] = 0x00;

    /* write flash page */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

    /* wait for target done */
    while (isp_ReadBusy())
    {}

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadLockBits
 * DESCRIPTION:
 *      Read Lock Bits.
 * PARAMETERS:
 *      pLockBits : Read lock bits buffer.
 * RETURN:
 *      TRUE   : Read Lock Bits Success;
 *      FALSE  : Read Lock Bits Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadLockBits
(
    OUT UINT8  *pLockBits
)
{
    CONST UINT8 aCmdTable[] = {0x58, 0x00, 0x00, 0x00};

    /* read Lock Bits */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
    *pLockBits = ISP_ReadByte();

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_WriteLockBits
 * DESCRIPTION:
 *      Write Lock Bits.
 * PARAMETERS:
 *      vLockBits : Lock bits data to be written.
 * RETURN:
 *      TRUE   : Write Lock Bits Success;
 *      FALSE  : Write Lock Bits Fail.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_WriteLockBits
(
    IN UINT8  vLockBits
)
{
    UINT8 aCmdTable[] = {0xAC, 0xE0, 0x00, 0x00};

    /* set commands */
    aCmdTable[3] = vLockBits | 0xC0;    /* Bit[7:6] are always 1 */

    /* write Lock Bits */
    ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));

    while (isp_ReadBusy())
    {}

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadFuseBytes
 * DESCRIPTION:
 *      Read Fuse Bytes.
 * PARAMETERS:
 *      pBuf  : Read Fush Byte data buffer;
 * RETURN:
 *      TRUE  : Read Fuse Bytes Successfully;
 *      FALSE : Read Fuse Bytes Fail;
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadFuseBytes
(
    OUT UINT8   *pBuf
)
{
    UINT8 aCmdTable[] = {0x50, 0x00, 0x00, 0x00};

    if (_chipSetting.vFuseByteLength >= 1)
    {
        /* read Fuse Low Byte */
        CLR_BIT(aCmdTable[0], 3);           /* select Low Byte   */
        CLR_BIT(aCmdTable[1], 3);           /* select Low Byte   */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    if (_chipSetting.vFuseByteLength >= 2)
    {
        /* read Fuse High Byte */
        SET_BIT(aCmdTable[0], 3);           /* select High Byte  */
        SET_BIT(aCmdTable[1], 3);           /* select High Byte  */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    if (_chipSetting.vFuseByteLength >= 3)
    {
        /* read Extern Fuse Byte */
        CLR_BIT(aCmdTable[0], 3);           /* select Extern Fuse Byte */
        SET_BIT(aCmdTable[1], 3);           /* select Extern Fuse Byte */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    if (_chipSetting.vFuseByteLength >= 4)
    {
        /* does not support too long Fuse Bytes */
        return FALSE;
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_WriteFuseBytes
 * DESCRIPTION:
 *      Write Fuse Bytes.
 * PARAMETERS:
 *      pBuf  : Fush Byte data buffer;
 * RETURN:
 *      TRUE  : Write Fuse Bytes Successfully;
 *      FALSE : Write Fuse Bytes Fail;
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_WriteFuseBytes
(
    IN UINT8   *pBuf
)
{
    UINT8   aCmdTable[] = {0xAC, 0xA0, 0x00, 0x00};

    if (_chipSetting.vFuseByteLength >= 1)
    {
        /* write Fuse Low Byte */
        CLR_BIT(aCmdTable[1], 3);           /* select Low Byte       */
        aCmdTable[3] = *pBuf++;             /* set Low Byte value    */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        while (isp_ReadBusy())              /* wait for target done */
        {}
    }

    if (_chipSetting.vFuseByteLength >= 2)
    {
        /* write Fuse High Byte */
        SET_BIT(aCmdTable[1], 3);           /* select High Byte      */
        aCmdTable[3] = *pBuf++;             /* set High Byte value   */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        while (isp_ReadBusy())              /* wait for target done */
        {}
    }

    if (_chipSetting.vFuseByteLength >= 3)
    {
        /* write Extern Fuse Byte */
        CLR_BIT(aCmdTable[1], 3);           /* select Extern Fuse Byte    */
        SET_BIT(aCmdTable[1], 2);
        aCmdTable[3] = *pBuf++;             /* set Extern Fuse Byte value */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        while (isp_ReadBusy())              /* wait for target done */
        {}
    }

    if (_chipSetting.vFuseByteLength >= 4)
    {
        /* does not support too long Fuse Bytes */
        return FALSE;
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadSignatureBytes
 * DESCRIPTION:
 *      Read Signature Bytes.
 * PARAMETERS:
 *      pBuf  : Read Signature Bytes buffer;
 * RETURN:
 *      TRUE  : Read Signature Bytes Successfully;
 *      FALSE : Read Signature Bytes Fail;
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadSignatureBytes
(
    OUT UINT8   *pBuf
)
{
    UINT8   vLoop;
    UINT8   aCmdTable[] = {0x30, 0x00, 0x00, 0x00};

    for (vLoop = 0; vLoop < 4; vLoop++)
    {
        /* set address bits */
        aCmdTable[2] = vLoop;

        /* read signature Byte from this address */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_ReadCalibrationBytes
 * DESCRIPTION:
 *      Read Calibration Bytes.
 * PARAMETERS:
 *      pBuf  : Read Calibration Bytes buffer;
 * RETURN:
 *      TRUE  : Read Calibration Bytes Successfully;
 *      FALSE : Read Calibration Bytes Fail;
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_ReadCalibrationBytes
(
    OUT UINT8   *pBuf
)
{
    UINT8   vLoop;
    UINT8   aCmdTable[] = {0x38, 0x00, 0x00, 0x00};

    vLoop = 0;
    if (_chipSetting.vCaliByteLength >= 1)
    {
        /* set address bits */
        aCmdTable[2] = vLoop;

        /* read calibration Byte from this address */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    vLoop++;
    if (_chipSetting.vCaliByteLength >= 2)
    {
        /* set address bits */
        aCmdTable[2] = vLoop;

        /* read calibration Byte from this address */
        ISP_WriteBytes(aCmdTable, COUNT_OF(aCmdTable));
        *pBuf++ = ISP_ReadByte();
    }

    vLoop++;
    if (_chipSetting.vFuseByteLength >= 3)
    {
        /* does not support too long Calibration Bytes */
        return FALSE;
    }

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_Connect
 * DESCRIPTION:
 *      Connect to target Chip for ISP.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      TRUE  : Conenct to target Chip Success;
 *      FALSE : Connect to target Chip Fail;
 * NOTES:
 *      This will reset target Chip to ISP Mode.
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_Connect(void)
{
    UINT8   vLoop = 5;

/* Open SPI Bus for ISP */
    ISP_OpenSpiBus();

/* Connect to target Chip */
    while (vLoop--)
    {
        ISP_ReleaseTarget();
        delay(1000);            /* 0.7 ms    (23 ms) */

        ISP_LockTarget();       /* reset lo once     */
        delay(1000);            /* 0.7 ms    (10 ms) */

        ISP_ReleaseTarget();    /* reset hi          */
        delay(1000);            /* 0.7 ms    (23 ms) */

        ISP_LockTarget();       /* reset lo twice    */
        delay(1000);            /* 0.7 ms    (10 ms) */

        ISP_ReleaseTarget();    /* reset hi          */
        delay(1000);            /* 0.7 ms    (23 ms) */

        ISP_LockTarget();       /* reset lo          */

        /* wait at least 20ms before sending programming enable command */
        DelayMs(20);

        if (ISP_EnableProgramming())
        {
            /* Connect to Target Chip Success */
            return TRUE;
        }
    }

    /* Connect to Target Chip Fail */
    return FALSE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_Disconnect
 * DESCRIPTION:
 *      Disconnect from target Chip.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      TRUE  : Disconnect from target Chip Success;
 *      FALSE : Disconnect from target Chip Fail;
 * NOTES:
 *      This will release target Chip to normal Mode.
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_Disconnect(void)
{
    /* Release Target Chip, to exit ISP Mode */
    ISP_ReleaseTarget();

    /* Close SPI Bus */
    ISP_CloseSpiBus();

    return TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      ISP_SetChipConfig
 * DESCRIPTION:
 *      Set Target Chip Config.
 * PARAMETERS:
 *      pChipCfg  : Target Chip Config.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Create/Update
 *****************************************************************************/
BOOL ISP_SetChipConfig(IN ISP_CHIP_CFG_T *pChipCfg)
{
    memcpy(&_chipSetting, pChipCfg, sizeof(_chipSetting));

    return TRUE;
}


#if 1

/******************************************************************************
 * FUNCTION NAME:
 *      upload_tuning_fw
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      This function will implement the serial programming algorithm to
 *       upload the Tuning firmware to ATtiny13.
 *       The incoming data from serial port is one byte of command + 1k Byte
 *       of hex file.
 * HISTORY:
 *      2009.12.25        Panda.Xiong         Update
 *****************************************************************************/
void upload_tuning_fw(void)
{
#define TARGET_CHIP_FLASH_PAGE_LEN      (32)

    UINT8   vPageLoop;
    UINT8   aBuf[TARGET_CHIP_FLASH_PAGE_LEN];

/* Lock Target Chip */
    ISP_LockTarget();

/* TODO: Read Target Chip Signature */
/* TODO: According to Target Chip Signature, to prepare Chip Config */
/* TODO: Set Chip Config */

/* Connect to Target Chip */
    ISP_Connect();

/* write Fuse Bytes */
    /*  Set brown-out detection level at Vcc = 2.7v, EEPROM preserved;
     *  f = 9.6MHz with start-up time of 64ms.
     */
    aBuf[0] = 0x3A;
    aBuf[1] = 0xFB;
    ISP_WriteFuseBytes(aBuf);

/* Re-Connect to Target Chip */
    ISP_Connect();

/* Force Chip Erase */
    ISP_ChipErase();

/* Program Flash */
    for (vPageLoop = 0; vPageLoop < 32; vPageLoop++)
    {
        /* Reset COM RX Buffer, and read one page from COM Port */
        rxIn = 0;
        load_rx_buffer(sizeof(aBuf));

        ISP_WriteFlashPage(vPageLoop * sizeof(aBuf), rxBuffer);
    }

/* Disconnect from Target Chip */
    ISP_Disconnect();


/* Send ACK, to indicate the firmware upgrading is successful */
    cmd_feedback(CMD_COMPLETE);
}

#endif

