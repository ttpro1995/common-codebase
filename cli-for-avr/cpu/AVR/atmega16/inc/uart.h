/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    Uart driver for AVR
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __UART_H__
#define __UART_H__

#include <basetype.h>

#define USE_LIB_PRINTF   (0)

#if USE_LIB_PRINTF
#include <stdio.h>
#endif

#define UART_BAUDRATE    (19200)

/* Uart control function */
void  uartSendByte(IN const UI8_T sent_data);
void  uartSendStr(IN const UI8_T *p_data);
void  uartSendAscii(IN const UI8_T data_in);
UI8_T uartGetByte(void);

/* Uart initial function */
void uartInit(void);

#endif /* __UART_H__ */
