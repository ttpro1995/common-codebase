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
 *   drv_i2c_lpt.c
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.4        PANDA         Create/Update
 *
*****************************************************************************/

#include <windows.h>
#include <typedef.h>
#include <drv_i2c_lpt.h>


#ifndef USE_INPOUT32_DLL

extern short Inp32(short portaddr);
extern void Out32(short portaddr, short datum);
extern int Opendriver(void);
extern void Closedriver(void);

#define inp32   Inp32
#define oup32   Out32

#else

#define LIB_INPOUT32_DLL_NAME   "inpout32.dll"

typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);

static HINSTANCE  hLib_inpout32;
static inpfuncPtr inp32;
static oupfuncPtr oup32;

#endif


/* default LPT base address is 0x378 */
static  GT_U32   vLptBaseAddr = 0x378;
#define LPT_OUT  (vLptBaseAddr + 0)
#define LPT_IN   (vLptBaseAddr + 1)

#define LPT_SET_REG(vReg, vData)    oup32((vReg), (vData))
#define LPT_GET_REG(vReg)           inp32(vReg)


typedef struct
{
    GT_U8       vBit;
    GT_BOOL     bInverse;
} I2C_BIT_S;

typedef struct
{
    I2C_BIT_S   vSCLOut;
    I2C_BIT_S   vSDAOut;
    I2C_BIT_S   vSCLIn;
    I2C_BIT_S   vSDAIn;
} I2C_CN_CFG_S;


/* use this to speed up the I2C operation speed */
static GT_U8    vLptOutRegVal;


#ifdef USE_INPOUT32_DLL
static GT_BOOL _load_dll(void)
{
    static GT_BOOL b_init = GT_FALSE;

    if (!b_init)
    {
        /* Load the library */
        hLib_inpout32 = LoadLibrary(LIB_INPOUT32_DLL_NAME);
        if (hLib_inpout32 == NULL)
        {
            printf("Load Library %s Failed.\n", LIB_INPOUT32_DLL_NAME);
            return GT_FALSE;
        }

        /* get the address of the Inp32 function */
        inp32 = (inpfuncPtr)GetProcAddress(hLib_inpout32, "Inp32");
        if (inp32 == NULL)
        {
            printf("GetProcAddress for Inp32 Failed.\n");
            return GT_FALSE;
        }

        /* get the address of the Oup32 function */
        oup32 = (oupfuncPtr)GetProcAddress(hLib_inpout32, "Out32");
        if (oup32 == NULL)
        {
            printf("GetProcAddress for Oup32 Failed.\n");
            return GT_FALSE;
        }

        b_init = GT_TRUE;
    }

    return GT_TRUE;
}
#endif


/******************************************************************************
 * FUNCTION NAME:
 *      _OpenDevice
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.6.16        PANDA         Create/Update
 *****************************************************************************/
static GT_BOOL _OpenDevice(void)
{
#ifndef USE_INPOUT32_DLL
    Opendriver();
    return TRUE;
#else
    return _load_dll();
#endif
}


/******************************************************************************
 * FUNCTION NAME:
 *      _CloseDevice
 * DESCRIPTION:
 *      N/A
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.6.16        PANDA         Create/Update
 *****************************************************************************/
static GT_BOOL _CloseDevice(void)
{
#ifndef USE_INPOUT32_DLL
    Closedriver();
    return TRUE;
#else
    return GT_TRUE;
#endif
}


/******************************************************************************
 * FUNCTION NAME:
 *      _SetSCL
 * DESCRIPTION:
 *      Set SCL data.
 * PARAMETERS:
 *      pChCfg : Point to I2C Channel Configuration.
 *      value  : =0, set SCL to low  level.
 *               =1, set SDA to high level.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.15        PANDA         Create/Update
 *****************************************************************************/
static void _SetSCL(IN const I2C_CN_CFG_S *pChCfg, int value)
{
    if (value == 0)
    {
        if (pChCfg->vSCLOut.bInverse)
        {
            SET_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
        }
        else
        {
            CLR_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
        }
    }
    else
    {
        if (pChCfg->vSCLOut.bInverse)
        {
            CLR_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
        }
        else
        {
            SET_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
        }
    }

    LPT_SET_REG(LPT_OUT, vLptOutRegVal);
}


/******************************************************************************
 * FUNCTION NAME:
 *      _SetSDA
 * DESCRIPTION:
 *      Set SDA data.
 * PARAMETERS:
 *      pChCfg : Point to I2C Channel Configuration.
 *      value  : =0, set SDA to low  level.
 *               =1, set SDA to high level.
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.15        PANDA         Create/Update
 *****************************************************************************/
static void _SetSDA(IN const I2C_CN_CFG_S *pChCfg, int value)
{
    if (value == 0)
    {
        if (pChCfg->vSDAOut.bInverse)
        {
            SET_BIT(vLptOutRegVal, pChCfg->vSDAOut.vBit);
        }
        else
        {
            CLR_BIT(vLptOutRegVal, pChCfg->vSDAOut.vBit);
        }
    }
    else
    {
        if (pChCfg->vSDAOut.bInverse)
        {
            CLR_BIT(vLptOutRegVal, pChCfg->vSDAOut.vBit);
        }
        else
        {
            SET_BIT(vLptOutRegVal, pChCfg->vSDAOut.vBit);
        }
    }

    LPT_SET_REG(LPT_OUT, vLptOutRegVal);
}


/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_LPT_GetSCL
 * DESCRIPTION:
 *      Get SCL data.
 * PARAMETERS:
 *      pChCfg : Point to I2C Channel Configuration.
 * RETURN:
 *      0 : SCL is low  level.
 *      1 : SCL is high level.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.15        PANDA         Create/Update
 *****************************************************************************/
static int _GetSCL(IN const I2C_CN_CFG_S *pChCfg)
{
/* currently, has no way to read the SCL line level,
 * so, only return the OUTPUT SCL line level.
 */
#if 0
    if (pCurrentI2cChCfg->vSCLIn.bInverse)
    {
        return !READ_BIT(LPT_GET_REG(LPT_IN), pCurrentI2cChCfg->vSCLIn.vBit);
    }
    else
    {
        return READ_BIT(LPT_GET_REG(LPT_IN), pCurrentI2cChCfg->vSCLIn.vBit);
    }
#else
    if (pChCfg->vSCLOut.bInverse)
    {
        return !READ_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
    }
    else
    {
        return READ_BIT(vLptOutRegVal, pChCfg->vSCLOut.vBit);
    }
#endif
}


/******************************************************************************
 * FUNCTION NAME:
 *      Drv_I2C_LPT_GetSDA
 * DESCRIPTION:
 *      Get SDA data.
 * PARAMETERS:
 *      pChCfg : Point to I2C Channel Configuration.
 * RETURN:
 *      0 : SDA is low  level.
 *      1 : SDA is high level.
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.15        PANDA         Create/Update
 *****************************************************************************/
static int _GetSDA(IN const I2C_CN_CFG_S *pChCfg)
{
    if (pChCfg->vSDAIn.bInverse)
    {
        return !READ_BIT(LPT_GET_REG(LPT_IN), pChCfg->vSDAIn.vBit);
    }
    else
    {
        return READ_BIT(LPT_GET_REG(LPT_IN), pChCfg->vSDAIn.vBit);
    }
}


/******************************************************************************
 * FUNCTION NAME:
 *      _Init
 * DESCRIPTION:
 *      I2C LPT Init.
 * PARAMETERS:
 *      N/A
 * RETURN:
 *      N/A
 * NOTES:
 *      N/A
 * HISTORY:
 *      2009.5.15        PANDA         Create/Update
 *****************************************************************************/
static GT_BOOL _Init(void)
{
    /* read the LPT OUT register value */
    vLptOutRegVal = LPT_GET_REG(LPT_OUT);

    return GT_TRUE;
}

#if 1

static const I2C_CN_CFG_S   vI2cLptaCfgTable =
{
    { 0, GT_TRUE  },    /* SCL_OUT */
    { 1, GT_TRUE  },    /* SDA_OUT */
    { 6, GT_TRUE  },    /* SCL_IN  */
    { 5, GT_FALSE },    /* SDA_IN  */
};

GT_BOOL DRV_I2C_LPTA_OpenDevice(void)
{
    _OpenDevice();
    return GT_TRUE;
}


GT_BOOL DRV_I2C_LPTA_CloseDevice(void)
{
    _CloseDevice();
    return GT_TRUE;
}


void DRV_I2C_LPTA_SetSCL(int vData)
{
    _SetSCL(&vI2cLptaCfgTable, vData);
}


void DRV_I2C_LPTA_SetSDA(int vData)
{
    _SetSDA(&vI2cLptaCfgTable, vData);
}


int DRV_I2C_LPTA_GetSCL(void)
{
    return _GetSCL(&vI2cLptaCfgTable);
}


int DRV_I2C_LPTA_GetSDA(void)
{
    return _GetSDA(&vI2cLptaCfgTable);
}


GT_BOOL DRV_I2C_LPTA_Init(void)
{
    if (!_Init())
    {
        return GT_FALSE;
    }

    /* Stretch SCL to 0, to Lock I2C Bus,
     *  to prevent other I2C Devices sending un-wanted data on the I2C Bus.
     */
    _SetSCL(&vI2cLptaCfgTable, 0);

    /* Set SDA to 0, then read, check the SDA can be controlled by software */
    _SetSDA(&vI2cLptaCfgTable, 0);
    if (_GetSDA(&vI2cLptaCfgTable) != 0)
    {
        return GT_FALSE;
    }

    /* Set SDA to 1, then read, check the SDA can be controlled by software */
    _SetSDA(&vI2cLptaCfgTable, 1);
    if (_GetSDA(&vI2cLptaCfgTable) != 1)
    {
        return GT_FALSE;
    }

    /* Release I2C Bus */
    _SetSCL(&vI2cLptaCfgTable, 1);

    /* I2C Bus can be controlled by software, assume this I2C Host path is OK */
    return GT_TRUE;
}

#endif

#if 1

static const I2C_CN_CFG_S   vI2cLptbCfgTable =
{
    { 0, GT_TRUE },    /* SCL_OUT */
    { 1, GT_TRUE },    /* SDA_OUT */
    { 6, GT_TRUE },    /* SCL_IN  */
    { 5, GT_TRUE },    /* SDA_IN  */
};

GT_BOOL DRV_I2C_LPTB_OpenDevice(void)
{
    _OpenDevice();
    return GT_TRUE;
}


GT_BOOL DRV_I2C_LPTB_CloseDevice(void)
{
    _CloseDevice();
    return GT_TRUE;
}


void DRV_I2C_LPTB_SetSCL(int vData)
{
    _SetSCL(&vI2cLptbCfgTable, vData);
}


void DRV_I2C_LPTB_SetSDA(int vData)
{
    _SetSDA(&vI2cLptbCfgTable, vData);
}


int DRV_I2C_LPTB_GetSCL(void)
{
    return _GetSCL(&vI2cLptbCfgTable);
}


int DRV_I2C_LPTB_GetSDA(void)
{
    return _GetSDA(&vI2cLptbCfgTable);
}


GT_BOOL DRV_I2C_LPTB_Init(void)
{
    if (!_Init())
    {
        return GT_FALSE;
    }

    /* Stretch SCL to 0, to Lock I2C Bus,
     *  to prevent other I2C Devices sending un-wanted data on the I2C Bus.
     */
    _SetSCL(&vI2cLptbCfgTable, 0);

    /* Set SDA to 0, then read, check the SDA can be controlled by software */
    _SetSDA(&vI2cLptbCfgTable, 0);
    if (_GetSDA(&vI2cLptbCfgTable) != 0)
    {
        return GT_FALSE;
    }

    /* Set SDA to 1, then read, check the SDA can be controlled by software */
    _SetSDA(&vI2cLptbCfgTable, 1);
    if (_GetSDA(&vI2cLptbCfgTable) != 1)
    {
        return GT_FALSE;
    }

    /* Release I2C Bus */
    _SetSCL(&vI2cLptbCfgTable, 1);

    /* I2C Bus can be controlled by software, assume this I2C Host path is OK */
    return GT_TRUE;
}

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
)
{
    vLptBaseAddr = vLptAddress;
    return GT_TRUE;
}

