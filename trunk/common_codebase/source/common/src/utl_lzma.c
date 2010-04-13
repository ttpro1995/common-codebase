/******************************************************************************
 *
 * (C) Copyright 2007
 *    Panda Xiong, yaxi1984@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * History:
 *    2007.03.27    Panda Xiong       Create
 *
******************************************************************************/

#include <inc/utl_lzma.h>


#define LZMA_KERNEL_SERVICE         1

#if LZMA_KERNEL_SERVICE

/*
  LzmaDecode.c
  LZMA Decoder (optimized for Speed version)

  LZMA SDK 4.22 Copyright (c) 1999-2005 Igor Pavlov (2005-06-10)
  http://www.7-zip.org/

  LZMA SDK is licensed under two licenses:
  1) GNU Lesser General Public License (GNU LGPL)
  2) Common Public License (CPL)
  It means that you can select one of these two licenses and
  follow rules of that license.

  SPECIAL EXCEPTION:
  Igor Pavlov, as the author of this Code, expressly permits you to
  statically or dynamically link your Code (or bind by name) to the
  interfaces of this file without subjecting your linked Code to the
  terms of the CPL or GNU LGPL. Any modifications or additions
  to this file, however, are subject to the LGPL or CPL terms.
*/

#define LZMA_RESULT_OK              0
#define LZMA_RESULT_DATA_ERROR  1
#define LZMA_RESULT_USER_CANCEL 2

#define LZMA_BASE_SIZE              1846
#define LZMA_LIT_SIZE               768

#define LZMA_PROPERTIES_SIZE        5

typedef struct
{
    int     literalContextBits;
    int     literalPosBits;
    int     posBits;
} CLzmaProperties;

#define LzmaGetNumProbs(lc, lp)         (LZMA_BASE_SIZE + (LZMA_LIT_SIZE << ((lc) + (lp))))

typedef struct _CLzmaDecoderState
{
    CLzmaProperties     Properties;
    LZMA_UI16_T             *Probs;
} CLzmaDecoderState;

#define kNumTopBits                     24
#define kTopValue                   ((LZMA_UI32_T)1 << kNumTopBits)

#define kNumBitModelTotalBits       11
#define kBitModelTotal              (1 << kNumBitModelTotalBits)
#define kNumMoveBits                5

#define kNumPosBitsMax              4
#define kNumPosStatesMax            (1 << kNumPosBitsMax)

#define kLenNumLowBits              3
#define kLenNumLowSymbols           (1 << kLenNumLowBits)
#define kLenNumMidBits              3
#define kLenNumMidSymbols           (1 << kLenNumMidBits)
#define kLenNumHighBits                 8
#define kLenNumHighSymbols      (1 << kLenNumHighBits)

#define LenChoice                   0
#define LenChoice2                  (LenChoice + 1)
#define LenLow                      (LenChoice2 + 1)
#define LenMid                      (LenLow + (kNumPosStatesMax << kLenNumLowBits))
#define LenHigh                         (LenMid + (kNumPosStatesMax << kLenNumMidBits))
#define kNumLenProbs                (LenHigh + kLenNumHighSymbols)

#define kNumStates                  12
#define kNumLitStates               7

#define kStartPosModelIndex             4
#define kEndPosModelIndex           14
#define kNumFullDistances           (1 << (kEndPosModelIndex >> 1))

#define kNumPosSlotBits                 6
#define kNumLenToPosStates          4

#define kNumAlignBits               4
#define kAlignTableSize                 (1 << kNumAlignBits)

#define kMatchMinLen                2

#define IsMatch                         0
#define IsRep                       (IsMatch + (kNumStates << kNumPosBitsMax))
#define IsRepG0                         (IsRep + kNumStates)
#define IsRepG1                         (IsRepG0 + kNumStates)
#define IsRepG2                         (IsRepG1 + kNumStates)
#define IsRep0Long                  (IsRepG2 + kNumStates)
#define PosSlot                         (IsRep0Long + (kNumStates << kNumPosBitsMax))
#define SpecPos                         (PosSlot + (kNumLenToPosStates << kNumPosSlotBits))
#define Align                       (SpecPos + kNumFullDistances - kEndPosModelIndex)
#define LenCoder                    (Align + kAlignTableSize)
#define RepLenCoder                     (LenCoder + kNumLenProbs)
#define Literal                         (RepLenCoder + kNumLenProbs)

#define RC_READ_BYTE                (*Buffer++)

#define RC_INIT2                    \
    Code = 0;                       \
    Range = 0xFFFFFFFF;                 \
    {                               \
        int i;                      \
        for(i = 0; i < 5; i++)          \
        {                           \
            RC_TEST;                \
            Code = (Code << 8) | RC_READ_BYTE;  \
        }                           \
    }

#define RC_TEST                         \
    {                               \
        if (Buffer == BufferLim)        \
            return LZMA_RESULT_DATA_ERROR;      \
    }

#define RC_INIT(buffer, bufferSize)     \
    Buffer = buffer;                    \
    BufferLim = buffer + bufferSize;    \
    RC_INIT2

#define RC_NORMALIZE                \
    if (Range < kTopValue)          \
    {                               \
        RC_TEST;                    \
        Range <<= 8;                \
        Code = (Code << 8) | RC_READ_BYTE;  \
    }

#define IfBit0(p)                       \
    RC_NORMALIZE;                   \
    bound = (Range >> kNumBitModelTotalBits) * *(p);    \
    if (Code < bound)

#define UpdateBit0(p)               \
    Range = bound;                  \
    *(p) += (kBitModelTotal - *(p)) >> kNumMoveBits;

#define UpdateBit1(p)               \
    Range -= bound;                     \
    Code -= bound;                  \
    *(p) -= (*(p)) >> kNumMoveBits;

#define RC_GET_BIT2(p, mi, A0, A1)  \
    IfBit0(p)                       \
    {                               \
        UpdateBit0(p);              \
        mi <<= 1;                   \
        A0;                             \
    }                               \
    else                            \
    {                               \
        UpdateBit1(p);              \
        mi = (mi + mi) + 1;             \
        A1;                             \
    }

#define RC_GET_BIT(p, mi)           \
    RC_GET_BIT2(p, mi, ; , ;)

#define RangeDecoderBitTreeDecode(probs, numLevels, res) \
    {                               \
        int i = numLevels;          \
        res = 1;                        \
        do                          \
        {                           \
            LZMA_UI16_T *p = probs + res;   \
            RC_GET_BIT(p, res)      \
        } while(--i != 0);          \
        res -= (1 << numLevels);        \
    }

#if 0
#if Literal != LZMA_BASE_SIZE
StopCompilingDueBUG
#endif
#endif

int LzmaDecodeProperties(CLzmaProperties *propsRes, const unsigned char *propsData, int size)
{
    unsigned char   prop0;

    if (size < LZMA_PROPERTIES_SIZE)
        return LZMA_RESULT_DATA_ERROR;

    prop0 = propsData[0];
    if (prop0 >= (9 * 5 * 5))
        return LZMA_RESULT_DATA_ERROR;

    for (propsRes->posBits = 0; prop0 >= (9 * 5); prop0 -= (9 * 5))
        propsRes->posBits++;

    for (propsRes->literalPosBits = 0; prop0 >= 9; prop0 -= 9)
        propsRes->literalPosBits++;

    propsRes->literalContextBits = prop0;

    return LZMA_RESULT_OK;
}

#define kLzmaStreamWasFinishedId (-1)

int LzmaDecode(CLzmaDecoderState *vs,
    const unsigned char *inBuffer, LZMA_UI32_T inSize, LZMA_UI32_T *inSizeProcessed,
    unsigned char *outBuffer, LZMA_UI32_T outSize, LZMA_UI32_T *outSizeProcessed,
    int (*progressCallback)(LZMA_UI32_T totalOutsize, LZMA_UI32_T currentOutsize))
{
    LZMA_UI16_T     *p = vs->Probs;
    LZMA_UI32_T     nowPos = 0;
    LZMA_UI8_T  previousByte = 0;
    LZMA_UI32_T     posStateMask = (1 << (vs->Properties.posBits)) - 1;
    LZMA_UI32_T     literalPosMask = (1 << (vs->Properties.literalPosBits)) - 1;
    int             lc = vs->Properties.literalContextBits;

    int                 state = 0;
    LZMA_UI32_T         rep0 = 1, rep1 = 1, rep2 = 1, rep3 = 1;
    int                 len = 0;
    const LZMA_UI8_T *Buffer;
    const LZMA_UI8_T *BufferLim;
    LZMA_UI32_T         Range;
    LZMA_UI32_T         Code;

    *inSizeProcessed = 0;
    *outSizeProcessed = 0;

    {
        LZMA_UI32_T i;
        LZMA_UI32_T numProbs = Literal + ((LZMA_UI32_T)LZMA_LIT_SIZE << (lc + vs->Properties.literalPosBits));
        for (i = 0; i < numProbs; i++)
            p[i] = kBitModelTotal >> 1;
    }

    RC_INIT(inBuffer, inSize);

    while(nowPos < outSize)
    {
        LZMA_UI16_T     *prob;
        LZMA_UI32_T     bound;
        int         posState = (int)((nowPos) & posStateMask);

        prob = p + IsMatch + (state << kNumPosBitsMax) + posState;

        IfBit0(prob)
        {
            int symbol = 1;

            UpdateBit0(prob)
            prob = p + Literal + (LZMA_LIT_SIZE * ((((nowPos) & literalPosMask) << lc) + (previousByte >> (8 - lc))));

            if (state >= kNumLitStates)
            {
                int         matchByte;
                matchByte = outBuffer[nowPos - rep0];
                do
                {
                    int         bit;
                    LZMA_UI16_T     *probLit;
                    matchByte <<= 1;
                    bit = (matchByte & 0x100);
                    probLit = prob + 0x100 + bit + symbol;
                    RC_GET_BIT2(probLit, symbol, if (bit != 0) break, if (bit == 0) break)
                } while (symbol < 0x100);
            }

            while (symbol < 0x100)
            {
                LZMA_UI16_T *probLit = prob + symbol;
                RC_GET_BIT(probLit, symbol)
            }
            previousByte = (LZMA_UI8_T)symbol;

            outBuffer[nowPos++] = previousByte;
            if (state < 4)
                state = 0;
            else
            {
                if (state < 10)
                    state -= 3;
                else
                    state -= 6;
            }
        }
        else
        {
            UpdateBit1(prob);
            prob = p + IsRep + state;

            IfBit0(prob)
            {
                UpdateBit0(prob);
                rep3 = rep2;
                rep2 = rep1;
                rep1 = rep0;
                state = state < kNumLitStates ? 0 : 3;
                prob = p + LenCoder;
            }
            else
            {
                UpdateBit1(prob);
                prob = p + IsRepG0 + state;

                IfBit0(prob)
                {
                    UpdateBit0(prob);
                    prob = p + IsRep0Long + (state << kNumPosBitsMax) + posState;

                    IfBit0(prob)
                    {
                        UpdateBit0(prob);

                        if (nowPos == 0)
                            return LZMA_RESULT_DATA_ERROR;

                        state = state < kNumLitStates ? 9 : 11;
                        previousByte = outBuffer[nowPos - rep0];
                        outBuffer[nowPos++] = previousByte;
                        continue;
                    }
                    else
                    {
                        UpdateBit1(prob);
                    }
                }
                else
                {
                    LZMA_UI32_T distance;
                    UpdateBit1(prob);
                    prob = p + IsRepG1 + state;

                    IfBit0(prob)
                    {
                        UpdateBit0(prob);
                        distance = rep1;
                    }
                    else
                    {
                        UpdateBit1(prob);
                        prob = p + IsRepG2 + state;

                        IfBit0(prob)
                        {
                            UpdateBit0(prob);
                            distance = rep2;
                        }
                        else
                        {
                            UpdateBit1(prob);
                            distance = rep3;
                            rep3 = rep2;
                        }
                        rep2 = rep1;
                    }

                    rep1 = rep0;
                    rep0 = distance;
                }
                state = state < kNumLitStates ? 8 : 11;
                prob = p + RepLenCoder;
            }

            {
                int     numBits, offset;
                LZMA_UI16_T *probLen = prob + LenChoice;

                IfBit0(probLen)
                {
                    UpdateBit0(probLen);
                    probLen = prob + LenLow + (posState << kLenNumLowBits);
                    offset = 0;
                    numBits = kLenNumLowBits;
                }
                else
                {
                    UpdateBit1(probLen);
                    probLen = prob + LenChoice2;
                    IfBit0(probLen)
                    {
                        UpdateBit0(probLen);
                        probLen = prob + LenMid + (posState << kLenNumMidBits);
                        offset = kLenNumLowSymbols;
                        numBits = kLenNumMidBits;
                    }
                    else
                    {
                        UpdateBit1(probLen);
                        probLen = prob + LenHigh;
                        offset = kLenNumLowSymbols + kLenNumMidSymbols;
                        numBits = kLenNumHighBits;
                    }
                }
                RangeDecoderBitTreeDecode(probLen, numBits, len);
                len += offset;
            }

            if (state < 4)
            {
                int     posSlot;
                state += kNumLitStates;
                prob = p + PosSlot + ((len < kNumLenToPosStates ? len : kNumLenToPosStates - 1) << kNumPosSlotBits);
                RangeDecoderBitTreeDecode(prob, kNumPosSlotBits, posSlot);
                if (posSlot >= kStartPosModelIndex)
                {
                    int numDirectBits = ((posSlot >> 1) - 1);
                    rep0 = (2 | ((LZMA_UI32_T)posSlot & 1));
                    if (posSlot < kEndPosModelIndex)
                    {
                        rep0 <<= numDirectBits;
                        prob = p + SpecPos + rep0 - posSlot - 1;
                    }
                    else
                    {
                        numDirectBits -= kNumAlignBits;
                        do
                        {
                            RC_NORMALIZE
                            Range >>= 1;
                            rep0 <<= 1;
                            if (Code >= Range)
                            {
                                Code -= Range;
                                rep0 |= 1;
                            }
                        } while (--numDirectBits != 0);

                        prob = p + Align;
                        rep0 <<= kNumAlignBits;
                        numDirectBits = kNumAlignBits;
                    }

                    {
                        int     i = 1;
                        int     mi = 1;
                        do
                        {
                            LZMA_UI16_T *prob3 = prob + mi;
                            RC_GET_BIT2(prob3, mi, ; , rep0 |= i);
                            i <<= 1;
                        } while(--numDirectBits != 0);
                    }
                }
                else
                    rep0 = posSlot;

                if (++rep0 == (LZMA_UI32_T)(0))
                {
                    /* it's for stream version */
                    len = kLzmaStreamWasFinishedId;
                    break;
                }
            }

            len += kMatchMinLen;
            if (rep0 > nowPos)
                return LZMA_RESULT_DATA_ERROR;

            do
            {
                previousByte = outBuffer[nowPos - rep0];
                len--;
                outBuffer[nowPos++] = previousByte;
            } while(len != 0 && nowPos < outSize);
        }

        if ((progressCallback) && (!(*progressCallback)(outSize, nowPos)))
        {
            return LZMA_RESULT_USER_CANCEL;
        }
    }

    RC_NORMALIZE;

    *inSizeProcessed = (LZMA_UI32_T)(Buffer - inBuffer);
    *outSizeProcessed = nowPos;
    return LZMA_RESULT_OK;
}

#endif


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_LZMA_GetDecodeBufSize
 * DESCRIPTION:
 *      Get LZMA decode buffer size
 * INPUT:
 *      flash_base : kernel image base address in flash, exclude the header.
 * OUTPUT:
 *      None
 * RETURN:
 *      >0  : Buffer Size
 *      =0  : Fail
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00   2007.02.14    Panda Xiong     Porting from 7-zip's LZMA SDK.
******************************************************************************/
LZMA_UI32_T UTL_LZMA_GetDecodeBufSize(LZMA_UI8_T *flash_base)
{
    CLzmaDecoderState   state;
    LZMA_UI32_T         decodeBufferSize;

    if (LZMA_RESULT_OK != LzmaDecodeProperties(&state.Properties,
                                               flash_base,
                                               LZMA_PROPERTIES_SIZE))
    {
        return 0;
    }

    decodeBufferSize = LzmaGetNumProbs(
                            state.Properties.literalContextBits,
                            state.Properties.literalPosBits)*sizeof(LZMA_UI16_T);

    /* align to 4 byte */
    return ((decodeBufferSize + sizeof(LZMA_UI32_T) - 1) & 0xFFFFFFFC);
}


/******************************************************************************
 * FUNCTION NAME:
 *      UTL_LZMA_Decompress
 * DESCRIPTION:
 *      Decompress kernel image use LZMA for bootloader.
 * INPUT:
 *      flash_base        : kernel image base address in flash, exclude the header.
 *      img_size          : kernel compressed image size.
 *      ram_base          : kernel base address in RAM.
 *      limit_size        : MAX kernel image size.
 *      decode_buf_base   : LZMA decode buffer base address.
 *      callback          : progress callback.
 *                          if NULL do nothing, else call it by decompress.
 *                          return 0: cancel; 1:continue.
 * OUTPUT:
 *      None
 * RETURN:
 *      >0   : Decompressed Size
 *      =0   : Fail
 *      =-1  : User Cancel (by progressCallback return 0)
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00   2007.02.14    Panda Xiong     Porting from 7-zip's LZMA SDK.
******************************************************************************/
int UTL_LZMA_Decompress
(
    LZMA_UI8_T   *flash_base,
    LZMA_UI32_T  img_size,
    LZMA_UI8_T   *ram_base,
    LZMA_UI32_T  limit_size,
    LZMA_UI8_T   *decode_buf_base,
    int          (*callback)(LZMA_UI32_T total_size, LZMA_UI32_T current_size)
)
{
    CLzmaDecoderState   state;
    LZMA_UI32_T         realImageDataSize;
    LZMA_UI32_T         outSize;
    LZMA_UI32_T         inProcessed, outProcessed;
    LZMA_UI8_T          abyte;
    int                 i;

    if ((img_size > limit_size) || (img_size < LZMA_PROPERTIES_SIZE + 8))
    {
        return 0;
    }

    if ((flash_base[LZMA_PROPERTIES_SIZE+0] == 0xFF)
        && (flash_base[LZMA_PROPERTIES_SIZE+1] == 0xFF)
        && (flash_base[LZMA_PROPERTIES_SIZE+2] == 0xFF)
        && (flash_base[LZMA_PROPERTIES_SIZE+3] == 0xFF))
    {
        return 0;
    }

    if (LZMA_RESULT_OK != LzmaDecodeProperties(&state.Properties,
                                               flash_base,
                                               LZMA_PROPERTIES_SIZE))
    {
        return 0;
    }

    realImageDataSize = img_size - (LZMA_PROPERTIES_SIZE + 8);

    outSize = 0;
    for (i=0; i<4; i++)
    {
        abyte    = flash_base[LZMA_PROPERTIES_SIZE+i];
        outSize += (((LZMA_UI32_T)abyte) << (i*8));
    }
    if (outSize > limit_size)
    {
        return 0;
    }

    state.Probs = (LZMA_UI16_T *)decode_buf_base;

    i = LzmaDecode(&state,
                   (LZMA_UI8_T *)((LZMA_UI32_T)flash_base + (LZMA_PROPERTIES_SIZE + 8)),
                   realImageDataSize,
                   &inProcessed,
                   ram_base,
                   outSize,
                   &outProcessed,
                   callback);

    switch (i)
    {
        case LZMA_RESULT_OK:
            return (int)outProcessed;

        case LZMA_RESULT_USER_CANCEL:
            return -1;

        default:
            return 0;
    }
}
