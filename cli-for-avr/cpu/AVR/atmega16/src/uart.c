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

#include <basetype.h>
#include <uart.h>
#include <cpu_delay.h>

#include <avr/io.h>
#include <util/delay.h>

#if USE_LIB_PRINTF

#include <stdio.h>

static I16_T uart_Printf(I8_T c, FILE *stream);

static FILE uart_stdout = FDEV_SETUP_STREAM(uart_Printf, NULL,
                                            _FDEV_SETUP_WRITE);


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
static I16_T uart_Printf(I8_T c, FILE *stream)
{
    if (c == '\n')
	{
        uart_Printf('\r', stream);
	}

    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;

    return 0;
}
#endif


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
void uartSendByte(IN const UI8_T sent_data)
{
    /* waiting for send buffer empty */
    while (!(UCSRA & (1 << UDRE)))
    {}

    UDR = sent_data;
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
/* Note: the string must ended with 0(NULL). */
void uartSendStr(IN const UI8_T *p_data)
{
	while (*p_data != '\0')
	{
		uartSendByte(*p_data);
		p_data++;
	}
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
void uartSendAscii(IN const UI8_T data_in)
{
	UI8_T count;
	UI8_T cut_data, data_tmp;

	cut_data = data_in;
	for (count=0; count<2; count++)
	{
		data_tmp = (cut_data & 0xF0) >> 4;
		if (data_tmp <= 9)
		{
			uartSendByte(data_tmp + 0x30);
		}
		else if ((data_tmp >= 0xA) && (data_tmp <= 0xF))
		{
			uartSendByte(data_tmp + 0x41-0xA);
		}

		cut_data <<= 4;
	}
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
/* Wait for 1ms to get data        */
/* If not get valid data, return 0 */
UI8_T uartGetByte(void)
{
    UI8_T lvc_loop;

    for (lvc_loop=100; lvc_loop>0; lvc_loop--)
    {
        if (UCSRA & _BV(RXC))   /* waiting for receive buffer full */
        {
            return(UDR);
        }

        CPU_DelayUs(10);
    }

    return(0);
}


/*******************************************************************************
 * FUNCTION NAME:
 *      None
 * DESCRIPTION:
 *      None
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
*******************************************************************************/
void uartInit(void)
{
    /*                f(osc)
     * Baudrate = ----------------
     *            16 * (UBRR + 1)
     *
     * Here, UBRR means (UBRRH, UBRRL).
     *
     *                f(osc)
     * And, UBRR = --------------- - 1
     *              16 * Baudrate
     */

    const UI16_T tmp = (UI16_T)((F_CPU)/(16.)/UART_BAUDRATE) - 1;

    UCSRB = 0x00;       /* disable while setting baud rate */
    UCSRA = 0x00;
    UCSRC = 0x86;
    UBRRH = (UI8_T)(tmp >> 8);
    UBRRL = (UI8_T)(tmp & 0xFF);
    UCSRB = 0x98;       /* use interrupt */

#if USE_LIB_PRINTF
	/* Set standard out module */
	stdout = &uart_stdout;
#endif

}
