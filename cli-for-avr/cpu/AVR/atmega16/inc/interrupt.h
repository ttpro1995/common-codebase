/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    Interrupt service rountine for ATmega16
NOTES:
    When you use this, you should know that which hardware module your MCU have.
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include <avr/interrupt.h>

/* global interrupt control */
#define ENABLE_INT()        sei()
#define DISABLE_INT()       cli()


/******************************************************************************
 *            Interrupt vectors
 ******************************************************************************/
/* External Interrupt Request 0 */
#define SIG_EXT_INT_0           SIG_INTERRUPT0
/* External Interrupt Request 1 */
#define SIG_EXT_INT_1           SIG_INTERRUPT1
/* External Interrupt Request 2 */
#define SIG_EXT_INT_2           SIG_INTERRUPT2

/* Timer/Counter0 Overflow */
#define SIG_TIMER_0_OVERFLOW    SIG_OVERFLOW0
/* Timer/Counter1 Overflow */
#define SIG_TIMER_1_OVERFLOW    SIG_OVERFLOW1
/* Timer/Counter2 Overflow */
#define SIG_TIMER_2_OVERFLOW    SIG_OVERFLOW2

/* Timer/Counter1 Capture Event */
#define SIG_TIMER_1_CAPTURE     SIG_INPUT_CAPTURE1

/* Timer/Counter0 Compare Match */
#define SIG_TIMER_0_COMP        SIG_OUTPUT_COMPARE0
/* Timer/Counter1 Compare Match A */
#define SIG_TIMER_1_COMP_A      SIG_OUTPUT_COMPARE1A
/* Timer/Counter1 Compare Match B */
#define SIG_TIMER_1_COMP_B      SIG_OUTPUT_COMPARE1B
/* Timer/Counter2 Compare Match */
#define SIG_TIMER_2_COMP        SIG_OUTPUT_COMPARE2

/* Serial Transfer Complete */
#define SIG_SPI_TX_OK           SIG_SPI

/* 2-wire Serial Interface */
#define SIG_IIC                 SIG_2WIRE_SERIAL

/* USART, Rx Complete */
#define SIG_UART_RX_OK          SIG_UART_RECV
/* USART, Tx Complete */
#define SIG_TX_OK               SIG_UART_TRANS
/* USART Data Register Empty */
#define SIG_UART_DATA_EMPTY     SIG_UART_DATA

/* ADC Conversion Complete */
#define SIG_ADC_CONVERT_OK      SIG_ADC

/* EEPROM Ready */
/********** already defined in avr/interrupt.h ***************
#define SIG_EEPROM_READY        SIG_EEPROM_READY
**************************************************************/

/* Analog Comparator */
#define SIG_ANALOG_COMP         SIG_COMPARATOR

/* Store Program Memory Ready */
#define SIG_PROGRAM_READY       SIG_SPM_READY


/******************************************************************************
 *            Interrupt Isr Macro
 ******************************************************************************
 * Useage:
 *      ISR_DISABLE_OTHER_INT(SIG_NUMBER)
 *      {
 *         ... Detail Interrupt Deal ...
 *      }
 *
 *      ISR_ENABLE_OTHER_INT(SIG_NUMBER)
 *      {
 *         ... Detail Interrupt Deal ...
 *      }
*******************************************************************************/
/* use this isr, when this isr happen, the other interrupts are disabled */
#define ISR_DISABLE_OTHER_INT(n)    SIGNAL(n)

/* use this isr, when this isr happen, the other interrupts are still enabled */
#define ISR_ENABLE_OTHER_INT(n)     INTERRUPT(n)


#endif /* __INTERRUPT_H__ */

