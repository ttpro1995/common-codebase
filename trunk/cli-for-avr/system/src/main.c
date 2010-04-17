/*******************************************************************************
COPYRIGHT:
    None
MODULE NAME:
    None
VERSION:
    1.00
DESCRIPTION:
    System start from here
NOTES:
    None
HISTORY:
    Ver1.00:    2007.03.02    Panda Xiong(yaxi1984@gmail.com)    create;
*******************************************************************************/

#include <basetype.h>
#include <interrupt.h>
#include <sys_init.h>
#include <cli.h>


/******************************************************************************/
/*                           main function                                    */
/******************************************************************************/
int main(void)
{
    DISABLE_INT();

    sysInit();

    CLI_Init();

    /* Never reach here */
    while (1)
    {}

    return 0;
}

