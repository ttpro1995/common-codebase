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

#include <h/basetype.h>
#include <inc/boot_define.h>
#include <inc/reg.h>
#include <inc/arm_int_ctrl.h>
#include <inc/cpu_int.h>
#include <inc/delay.h>
#include <inc/utl_prt.h>

extern void CPU_ISR_Global_Handler(void);


/* if check this Interrupt valid, return GT_TRUE */
typedef GT_BOOL (*CPU_INT_VECTOR_CHECK)(void);


typedef struct
{
    const CPU_INT_SRC_NAME    src_name;       /* interrupt source name     */
    const CPU_INT_ID          int_id;         /* interrurpt real id        */

    CPU_INT_VECTOR_CHECK      vector_check;   /* interrupt check API       */
    CPU_INT_VECTOR_HANDLER    vector_handler; /* interrupt process handler */

    /* interrurpt enable flag.
     *   =TRUE     : enabled;
     *   =GT_FALSE : disabled.
     */
    GT_BOOL                   int_enable;

} CPU_INT_MAP;

/* check this is a subnet of system peripheral */
#define CPU_INT_IS_SYS_SUBNET(int_num, src_name)                             \
    (((int_num == CPU_INT_ID_SYS)                                            \
            && (src_name != CPU_INT_SRC_SYS_GLOBAL)) ? GT_TRUE : GT_FALSE)


/* PART of interrupt check */
#if 1

static GT_BOOL cpu_int_MC_CheckInt(void)
{
    return GT_FALSE;
}

static GT_BOOL cpu_int_DBGU_CheckInt(void)
{
    GT_UI32     data, mask;

    if ((VP32(DBGU_IMR)) == 0x0)
    {
        return GT_FALSE;
    }

    mask = (1 << 0) | (1 << 5) | (1 << 6) | (1 << 7);
    data = (VP32(DBGU_SR));

    return ((data & mask) ? GT_TRUE : GT_FALSE);
}

static GT_BOOL cpu_int_ST_CheckInt(void)
{
    if ((VP32(ST_IMR)) == 0x0)
    {
        return GT_FALSE;
    }

    /* here, we can't check system timer,
     * cause if when we read the ST_SR register, the status will be self-cleared.
     * in this case, in handler, it can't read the real status.
     * so, we will read it in the system timer handler.
     */
    return GT_TRUE;
}

static GT_BOOL cpu_int_RTC_CheckInt(void)
{
    return GT_FALSE;
}

static GT_BOOL cpu_int_PMC_CheckInt(void)
{
    return GT_FALSE;
}

#endif


/* PART of interrupt map */
#if 1

/* CPU interrupt MAP table */
static CPU_INT_MAP cpu_int_map[] =
{
    /* System Peripheral.
     * here, we should put the ST_PIT at first.
     * cause if put it to the end, it will take more time to deal the handler.
     * this is considering of optimizing timing for RTOS.
     *
     * And, in all the interrupt handler, we should do less things.
     */
    {CPU_INT_SRC_SYS_GLOBAL, CPU_INT_ID_SYS,    cpu_int_ST_CheckInt,    NULL, GT_FALSE},
    {CPU_INT_SRC_ST_PIT,     CPU_INT_ID_SYS,    NULL,/* must be NULL */ NULL, GT_FALSE},
    {CPU_INT_SRC_ST_RTT,     CPU_INT_ID_SYS,    NULL,/* must be NULL */ NULL, GT_FALSE},
    {CPU_INT_SRC_ST_WDT,     CPU_INT_ID_SYS,    NULL,/* must be NULL */ NULL, GT_FALSE},
    {CPU_INT_SRC_ST_ALARM,   CPU_INT_ID_SYS,    NULL,/* must be NULL */ NULL, GT_FALSE},
    {CPU_INT_SRC_MC,         CPU_INT_ID_SYS,    cpu_int_MC_CheckInt,    NULL, GT_FALSE},
    {CPU_INT_SRC_DBGU_UART,  CPU_INT_ID_SYS,    cpu_int_DBGU_CheckInt,  NULL, GT_FALSE},
    {CPU_INT_SRC_RTC,        CPU_INT_ID_SYS,    cpu_int_RTC_CheckInt,   NULL, GT_FALSE},
    {CPU_INT_SRC_PMC,        CPU_INT_ID_SYS,    cpu_int_PMC_CheckInt,   NULL, GT_FALSE},

    /* Parallel IO Controller */
    {CPU_INT_SRC_PIOA,       CPU_INT_ID_PIOA,   NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_PIOB,       CPU_INT_ID_PIOB,   NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_PIOC,       CPU_INT_ID_PIOC,   NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_PIOD,       CPU_INT_ID_PIOD,   NULL,                   NULL, GT_FALSE},

    /* USART */
    {CPU_INT_SRC_USART0,     CPU_INT_ID_USART0, NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_USART1,     CPU_INT_ID_USART1, NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_USART2,     CPU_INT_ID_USART2, NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_USART3,     CPU_INT_ID_USART3, NULL,                   NULL, GT_FALSE},

    /* Multimedia Card Interface */
    {CPU_INT_SRC_MCI,        CPU_INT_ID_MCI,    NULL,                   NULL, GT_FALSE},

    /* USB Device Port */
    {CPU_INT_SRC_UDP,        CPU_INT_ID_UDP,    NULL,                   NULL, GT_FALSE},

    /* Two-Wire Interface */
    {CPU_INT_SRC_TWI,        CPU_INT_ID_TWI,    NULL,                   NULL, GT_FALSE},

    /* Serial Peripheral Interface */
    {CPU_INT_SRC_SPI,        CPU_INT_ID_SPI,    NULL,                   NULL, GT_FALSE},

    /* Serial Synchronous Controller */
    {CPU_INT_SRC_SSC0,       CPU_INT_ID_SSC0,   NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_SSC1,       CPU_INT_ID_SSC1,   NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_SSC2,       CPU_INT_ID_SSC2,   NULL,                   NULL, GT_FALSE},

    /* Timer Counter */
    {CPU_INT_SRC_TC0,        CPU_INT_ID_TC0,    NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_TC1,        CPU_INT_ID_TC1,    NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_TC2,        CPU_INT_ID_TC2,    NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_TC3,        CPU_INT_ID_TC3,    NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_TC4,        CPU_INT_ID_TC4,    NULL,                   NULL, GT_FALSE},
    {CPU_INT_SRC_TC5,        CPU_INT_ID_TC5,    NULL,                   NULL, GT_FALSE},

    /* USB Host port */
    {CPU_INT_SRC_UHP,        CPU_INT_ID_UHP,    NULL,                   NULL, GT_FALSE},

    /* Ethernet MAC */
    {CPU_INT_SRC_EMAC,       CPU_INT_ID_EMAC,   NULL,                   NULL, GT_FALSE},

};


#define CPU_INT_INVALID_MAP_ID  (-1)

static GT_I32 cpu_int_GetIntMapID(IN const CPU_INT_SRC_NAME src_name)
{
    const CPU_INT_MAP *end_int_map = cpu_int_map + sizeof(cpu_int_map)/sizeof(cpu_int_map[0]);
    CPU_INT_MAP       *p_int_map;

    if (CPU_INT_CHECK_SRC_NAME(src_name))
    {
        for (p_int_map = cpu_int_map; p_int_map < end_int_map; p_int_map++)
        {
            if (CPU_INT_CHECK_SRC_NAME(src_name)
                && (p_int_map->src_name == src_name))
            {
                return (p_int_map - cpu_int_map);
            }
        }
    }

    return CPU_INT_INVALID_MAP_ID;
}

#endif


/* PART of interrupt handler */
#if 1

static void cpu_int_ST_Handler(void)
{
    #define CPU_INT_ST_MASK     (0xF)

    const CPU_INT_SRC_NAME  src_map[] =
    {
        CPU_INT_SRC_ST_PIT,   /* bit  0, Periodic Interval Timer */
        CPU_INT_SRC_ST_WDT,   /* bit  1, WatchDog Timer          */
        CPU_INT_SRC_ST_RTT,   /* bit  2, Real-Time Timer         */
        CPU_INT_SRC_ST_ALARM, /* bit  3, Alarm                   */
        CPU_INT_SRC_UNKNOWN,  /* bit  4, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit  5, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit  6, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit  7, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit  8, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit  9, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 10, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 11, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 12, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 13, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 14, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 15, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 16, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 17, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 18, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 19, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 20, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 21, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 22, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 23, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 24, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 25, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 26, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 27, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 28, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 29, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 30, unknown                 */
        CPU_INT_SRC_UNKNOWN,  /* bit 31, unknown                 */
    };

    GT_UI32                 data;
    GT_I32                  int_map_id;
    GT_UI32                 shift_bit;
    CPU_INT_SRC_NAME        src_name;
    CPU_INT_VECTOR_HANDLER  handler;

    data = (VP32(ST_SR)) & CPU_INT_ST_MASK;

    /* if ST global interrupt not enabled, skip */
    int_map_id = cpu_int_GetIntMapID(CPU_INT_SRC_SYS_GLOBAL);
    if ((int_map_id == CPU_INT_INVALID_MAP_ID)
        || !(cpu_int_map[int_map_id].int_enable))
    {
        return;
    }

    //UTL_PRT_Printf("\n\r-- ST Vector Detected (data:%02X) --", data);

    for (shift_bit = 0;
         (data != 0) && (shift_bit < 32);
         shift_bit++)
    {
        if ((data & (1 << shift_bit)) == 0)
        {
            continue;
        }

        /* clear this bit flag */
        data &= ~(1 << shift_bit);

        src_name = src_map[shift_bit];
        if (src_name == CPU_INT_SRC_UNKNOWN)
        {
            continue;
        }

        int_map_id = cpu_int_GetIntMapID(src_name);
        if (int_map_id == CPU_INT_INVALID_MAP_ID)
        {
            continue;
        }

        if (cpu_int_map[int_map_id].int_enable)
        {
            handler = cpu_int_map[int_map_id].vector_handler;
            if (handler != NULL)
            {
                /* call the vector handler */
                (*handler)();
            }
        }
    }

    return;
}

/******************************************************************************
FUNCTION NAME:
    SYS_ISR_SYS_Handler
DESCRIPTION:
    process System Peripherals IRQ ISR.
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    If you want to add new handler in System p_vector_process ISR,
    please add it into all_handler_api.
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
void CPU_INT_Global_Handler(void)
{
    const CPU_INT_MAP *end_int_map = cpu_int_map + sizeof(cpu_int_map)/sizeof(cpu_int_map[0]);
    CPU_INT_MAP       *p_int_map;

    CPU_INT_SetGlobalState(GT_FALSE);

    for (p_int_map = cpu_int_map; p_int_map < end_int_map; p_int_map++)
    {
        if ((p_int_map->int_enable)
            && (p_int_map->vector_check != NULL)
            && (*p_int_map->vector_check)())
        {
            if (p_int_map->vector_handler != NULL)
            {
                (*p_int_map->vector_handler)();
            }
        }
    }

    CPU_INT_SetGlobalState(GT_TRUE);
}

#endif


#if 1

/******************************************************************************
FUNCTION NAME:
    CPU_INT_VectorSpurious
DESCRIPTION:
    process Spurious exception
INPUT:
    NONE
OUTPUT:
    NONE
RETURN:
    NONE
NOTES:
    NONE
HISTORY:
    2007.03.27        Panda Xiong          Create
******************************************************************************/
static void cpu_int_VectorSpurious(void)
{
    UTL_PRT_Printf("\n\r--%s(line%d) : Spurious exception--",
                   __FILE__, __LINE__);

    /* clear all interrupt */
    (VP32(AIC_ICCR)) = ~0;

    while (1)
    {}
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_HookInterrupt
 * DESCRIPTION:
 *      Hook ISR into AIC.
 * INPUT:
 *      src_name  : interrupt source peripherals name;
 *      prio      : interrupt priority;
 *      src_type  : interrupt trigger source mode;
 *      handler   : interrupt handler.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : hook success;
 *      GT_FALSE : hook fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL cpu_int_HookRealInt
(
    IN const CPU_INT_SRC_NAME   src_name,
    IN const GT_UI32            prio,
    IN const GT_UI32            src_type,
    IN CPU_INT_VECTOR_HANDLER   handler
)
{
    GT_I32      int_map_id;
    CPU_INT_ID  int_num;

    if ((handler == NULL)
        || !CPU_INT_CHECK_SRC_NAME(src_name)
        || !CPU_INT_CHECK_PRIORITY(prio))
    {
        return GT_FALSE;
    }

    int_map_id = cpu_int_GetIntMapID(src_name);
    if (int_map_id == CPU_INT_INVALID_MAP_ID)
    {
        return GT_FALSE;
    }

    int_num = cpu_int_map[int_map_id].int_id;
    if (int_num == CPU_INT_ID_UNKNOWN)
    {
        return GT_FALSE;
    }

    if (!CPU_INT_IS_SYS_SUBNET(int_num, src_name))
    {
        /* disable interrupt controller */
        (VP32(AIC_IDCR)) = (1 << int_num);

        /* set source interrupt hanlder vector */
        (VP32(AIC_SVR(int_num))) = (GT_UI32)handler;

        /* set source mode register */
        (VP32(AIC_SMR(int_num))) = (prio << 0) | (src_type << 5);

        /* clear interrupt */
        (VP32(AIC_ICCR)) = (1 << int_num);
    }

    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_HookInterrupt
 * DESCRIPTION:
 *      Hook ISR into AIC.
 * INPUT:
 *      src_name  : interrupt source peripherals name;
 *      prio      : interrupt priority;
 *      src_type  : interrupt trigger source mode;
 *      handler   : interrupt handler.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : hook success;
 *      GT_FALSE : hook fail.
 * NOTES:
 *      This hook API will auto disable the corresponding interrupt,
 *      you must re-enable the interrupt, if you really want to that.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_HookInterrupt
(
    IN const CPU_INT_SRC_NAME   src_name,
    IN const GT_UI32            prio,
    IN const GT_UI32            src_type,
    IN CPU_INT_VECTOR_HANDLER   handler
)
{
    GT_I32      int_map_id;
    CPU_INT_ID  int_num;

    if ((handler == NULL)
        || !CPU_INT_CHECK_SRC_NAME(src_name)
        || !CPU_INT_CHECK_PRIORITY(prio))
    {
        return GT_FALSE;
    }

    int_map_id = cpu_int_GetIntMapID(src_name);
    if (int_map_id == CPU_INT_INVALID_MAP_ID)
    {
        return GT_FALSE;
    }

    int_num = cpu_int_map[int_map_id].int_id;
    if (int_num == CPU_INT_ID_UNKNOWN)
    {
        return GT_FALSE;
    }

    /* hook interrupt vector hanlder */
    cpu_int_map[int_map_id].vector_handler = handler;

    if (!CPU_INT_IS_SYS_SUBNET(int_num, src_name))
    {
        /* disable interrupt controller */
        (VP32(AIC_IDCR)) = (1 << int_num);

        /* set source mode register */
        (VP32(AIC_SMR(int_num))) = (prio << 0) | (src_type << 5);

        /* clear interrupt */
        (VP32(AIC_ICCR)) = (1 << int_num);
    }

    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_SetInterrupt
 * DESCRIPTION:
 *      Enable/disable corresponding interrupt.
 * INPUT:
 *      int_num   : interrupt number;
 *      enable    : =TRUE: enable interrupt; =FALSE: disable interrupt.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : set interrupt success;
 *      GT_FALSE : set interrupt fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_SetInterrupt
(
    IN const CPU_INT_SRC_NAME   src_name,
    IN const GT_BOOL            enable
)
{
    GT_I32      int_map_id;
    CPU_INT_ID  int_num;

    if (!CPU_INT_CHECK_SRC_NAME(src_name))
    {
        return GT_FALSE;
    }

    int_map_id = cpu_int_GetIntMapID(src_name);
    if (int_map_id == CPU_INT_INVALID_MAP_ID)
    {
        return GT_FALSE;
    }

    int_num = cpu_int_map[int_map_id].int_id;
    if (int_num == CPU_INT_ID_UNKNOWN)
    {
        return GT_FALSE;
    }

    /* set new interrupt enable state */
    cpu_int_map[int_map_id].int_enable = enable;

    if (!CPU_INT_IS_SYS_SUBNET(int_num, src_name))
    {
        if (enable == GT_TRUE)
        {
            (VP32(AIC_IECR)) = (1 << int_num);
        }
        else
        {
            (VP32(AIC_IDCR)) = (1 << int_num);
        }
    }

    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_SetGlobalState
 * DESCRIPTION:
 *      Enable/Disable CPU global interrupt.
 * INPUT:
 *      enable : =GT_TRUE, enable;
 *               =GT_FALSE, disable.
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : success;
 *      GT_FALSE : fail.
 * NOTES:
 *      None
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_SetGlobalState(IN const GT_BOOL enable)
{
    if (enable)
    {
        /* enable CPU global interrupt */
        ARM_INT_GLOBAL_ENABLE(IRQ_BIT);
        ARM_INT_GLOBAL_ENABLE(FIQ_BIT);
    }
    else
    {
        /* disable CPU global interrupt */
        ARM_INT_GLOBAL_DISABLE(IRQ_BIT);
        ARM_INT_GLOBAL_DISABLE(FIQ_BIT);
    }

    return GT_TRUE;
}


/******************************************************************************
 * FUNCTION NAME:
 *      CPU_INT_Init
 * DESCRIPTION:
 *      Init AIC for interrupt.
 * INPUT:
 *      None
 * OUTPUT:
 *      None
 * RETURN:
 *      GT_TRUE  : init success;
 *      GT_FALSE : init fail.
 * NOTES:
 *      Before we do this, the vector address in 0x18-0x1C must be:
 *      ldr pc, [pc, #-0xF20]  -- that is 0xE51FFF20.
 * HISTORY:
 *      Ver1.00     2007.02.14      Panda Xiong         Create
******************************************************************************/
GT_BOOL CPU_INT_Init(void)
{
    CPU_INT_SRC_NAME  src_name;

    (VP32(AIC_IDCR)) = ~0; /* disable all interrupt controller */
    (VP32(AIC_ICCR)) = ~0; /* clear all interrupt              */

    for (src_name = CPU_INT_SRC_START; src_name <= CPU_INT_SRC_END; src_name++)
    {
        /* hook interrupt */
        cpu_int_HookRealInt(src_name,
                            CPU_INT_LOWEST_AIC_PRIORITY,
                            CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                            CPU_ISR_Global_Handler);
    }

    /* set spurious handler */
    (VP32(AIC_SPU)) = (GT_UI32)cpu_int_VectorSpurious;

    /* disable the protecting mode, and enable AIC to control the IRQ and FIQ */
    (VP32(AIC_DCR)) = 0;

    /* write 8 end of interrupt command.
     * cause the AT91RM9200 CPU have 8 level hardware stack,
     * we must pop all hardware stack when init AIC.
     */
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;
    (VP32(AIC_EOICR)) = 0;

    /* hook global system peripheral interrupt */
    CPU_INT_HookInterrupt(CPU_INT_SRC_SYS_GLOBAL,
                          CPU_INT_HIGHEST_AIC_PRIORITY,
                          CPU_INT_SRC_TYPE_INT_LEVEL_SENSITIVE,
                          cpu_int_ST_Handler);

    /* enable global system peripherals interrupt */
    CPU_INT_SetInterrupt(CPU_INT_SRC_SYS_GLOBAL, GT_TRUE);

    return GT_TRUE;
}

#endif

