/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    None
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#ifndef __CPU_DELAY_H__
#define __CPU_DELAY_H__

#include <basetype.h>


/*******************************************************************************
 * FUNCTION NAME:
 *      CPU_DelayUs
 * DESCRIPTION:
 *      Delay n us
 * INPUT:
 *      n : delay how many us
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      For AVR : The maximal possible delay is 768 us / F_CPU in MHz.
 *                So, don't try to delay more than 750us.
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void CPU_DelayUs(IN const UI16_T n);


/*******************************************************************************
 * FUNCTION NAME:
 *      CPU_DelayMs
 * DESCRIPTION:
 *      Delay n ms
 * INPUT:
 *      n : delay how many ms
 * OUTPUT:
 *      None
 * RETURN:
 *      None
 * NOTES:
 *      For AVR : The maximal possible delay is 262.14 ms / F_CPU in MHz.
 *                So, don't try to delay more than 255ms.
 * HISTORY:
 *      Ver1.00     2007.01.29      Panda Xiong         Create
 ******************************************************************************/
void CPU_DelayMs(IN const UI16_T n);

#endif /* __CPU_DELAY_H__ */

