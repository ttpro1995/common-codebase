/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    System initialize function.
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#include <basetype.h>
#include <sys_init.h>
#include <uart.h>

#include <avr/io.h>
#include <avr/interrupt.h>


/******************************************************************************/
/*                   Initial I/O                                              */
/******************************************************************************/
static void initPort(void)
{
    DDRA  = 0xFF;
    PORTA = 0xFF;

    DDRB  = 0xFF;
    PORTB = 0xFF;

    DDRC  = 0xFF;
    PORTC = 0xFF;

    DDRD  = 0xFF;
    PORTD = 0xFF;
}


/******************************************************************************/
/*                 System initialize function                                 */
/******************************************************************************/
void sysInit(void)
{
	initPort();
	uartInit();
}

