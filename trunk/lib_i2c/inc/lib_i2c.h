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
 *   lib_i2c.h
 * NODULE NAME:
 *   N/A
 * DESCRIPTION:
 *   N/A
 * HISTORY:
 *   2009.5.11        Panda.Xiong         Create/Update
 *
*****************************************************************************/

#ifndef __LIB_I2C_H
#define __LIB_I2C_H


#include <typedef.h>


typedef enum
{
    LIB_I2C_HOSTTYPE_INVALID = 0,

    LIB_I2C_HOSTTYPE_LPTA,
    LIB_I2C_HOSTTYPE_LPTB,
    LIB_I2C_HOSTTYPE_USB
} LIB_I2C_HOSTTYPE_E;


#ifdef _LIB_I2C_BUILD_DLL_
 #define DECLEAR_DLL        __declspec(dllexport)
#else
 #define DECLEAR_DLL        __declspec(dllimport)
#endif


#endif /* __LIB_I2C_H */

