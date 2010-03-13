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

#include <inc/boot_define.h>
#include <inc/reg.h>


/* CPU IRQ Entry Macro
 * Register initialization
 *      Banked Registers        ; Irq_register
 *      SPSR need to be saved for nested interrupt
 *      R13_irq <- Irq Stack register
 *      R14_irq <- Irq link register
 */
.macro CPU_IRQ_ENTRY

    /* Adjust and save LR_irq in IRQ stack. */
    sub    lr, lr, #4
    stmfd  sp!, {lr}

    /* Write in the IVR to support Protect Mode;
     * No effect in Normal Mode;
     * De-assert the NIRQ and clear the source in Protect Mode.
     */
    ldr    r14, =AIC_BASE_REG
    str    r14, [r14, #(AIC_IVR - AIC_BASE_REG)]

    /* Save SPSR and r0 in IRQ stack */
    mrs    r14, spsr
    stmfd  sp!, {r14}

    /* Enable Interrupt and Switch in Supervisor Mode */
    msr    cpsr_c, #0x1F

    /* Save scratch/used registers and LR in User Stack */
    stmfd  sp!, {r0-r3, r12, r14}

.endm



/* CPU IRQ Exit Macro */
.macro CPU_IRQ_EXIT

    /* Restore scratch/used registers and LR from User Stack */
    ldmia   sp!, { r0-r3, r12, r14}

    /* Disable Interrupt and switch back in IRQ mode */
    msr     cpsr_c, #(IRQ_BIT | IRQ_MODE)

    /* Mark the End of Interrupt on the AIC */
    ldr     r14, =AIC_EOICR
    str     r14, [r14]

    /* Restore SPSR_irq and r0 from IRQ stack */
    ldmia   sp!, {r14}
    msr     spsr_cxsf, r14

    /* Restore adjusted  LR_irq from IRQ stack directly in the PC */
    ldmia  sp!, {pc}^

.endm



/* CPU FIQ Entry Macro
 * FIQ Entry with no nested (reentrant) interrupt allowed.
 * Register initialization
 *      Banked Registers        ; FIQ_register
 *      SPSR does not need to be save because we do not want nested FIQ
 *      R13_fiq <- Same as above. No need to save it.
 *      R14_irq <- link register
 */
.macro CPU_FIQ_ENTRY

    /* Switch in SYS/User Mode to allow User Stack access for C code
     * and Disable FIQ and IRQ Interrupt for this mode,
     * because the FIQ is not yet acknowledged.
     */
    msr    cpsr_c, #(NO_INT | SYS_MODE)

    /* Save scratch/used registers and LR in User Stack */
    stmfd  sp!, {r0-r3, r12, lr}

.endm



/* CPU FIQ Exit */
.macro CPU_FIQ_EXIT

    /* Restore scratch/used registers and LR from User Stack */
    ldmia   sp!, { r0-r3, r12, lr}

    /* Leave Interrupts disabled and switch back in FIQ mode */
    msr     cpsr_c, #(NO_INT | FIQ_MODE)

    /* Restore the Program Counter using the LR_fiq directly in the PC */
    subs    pc, lr, #4

.endm

