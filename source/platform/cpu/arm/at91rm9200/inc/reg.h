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

#ifndef __REG_H
#define __REG_H

#ifdef __cplusplus
extern "C" {
#endif

/* This registers are under DBGU (debug unit) */
#define DBGU_BASE_REG       (0xFFFFF200)
#define DBGU_CR             (DBGU_BASE_REG + 0x0000)
#define DBGU_MR             (DBGU_BASE_REG + 0x0004)
#define DBGU_IER            (DBGU_BASE_REG + 0x0008)
#define DBGU_IDR            (DBGU_BASE_REG + 0x000C)
#define DBGU_IMR            (DBGU_BASE_REG + 0x0010)
#define DBGU_SR             (DBGU_BASE_REG + 0x0014)
#define DBGU_RHR            (DBGU_BASE_REG + 0x0018)
#define DBGU_THR            (DBGU_BASE_REG + 0x001C)
#define DBGU_BRGR           (DBGU_BASE_REG + 0x0020)
#define DBGU_CIDR           (DBGU_BASE_REG + 0x0040)
#define DBGU_EXID           (DBGU_BASE_REG + 0x0044)
#define DBGU_FNR            (DBGU_BASE_REG + 0x0048)

/* This registers are under TC (timer counter) */
#define TC_BASE_REG         (0xFFFA0000)
#define TC0_BASE_REG        (TC_BASE_REG + 0x0000)
#define TC1_BASE_REG        (TC_BASE_REG + 0x0040)
#define TC2_BASE_REG        (TC_BASE_REG + 0x0080)
#define TC_CCR(x)           ((x) + 0x0000)  /*  x range :                */
#define TC_CMR(x)           ((x) + 0x0004)  /*     TC0_BASE_REG          */
#define TC_CV(x)            ((x) + 0x0010)  /*     TC1_BASE_REG          */
#define TC_RA(x)            ((x) + 0x0014)  /*     TC2_BASE_REG          */
#define TC_RB(x)            ((x) + 0x0018)
#define TC_RC(x)            ((x) + 0x001C)
#define TC_SR(x)            ((x) + 0x0020)
#define TC_IER(x)           ((x) + 0x0024)
#define TC_IDR(x)           ((x) + 0x0028)
#define TC_IMR(x)           ((x) + 0x002C)
#define TC_BCR(x)           (TC_BASE_REG + 0x00C0)
#define TC_BMR(x)           (TC_BASE_REG + 0x00C4)

/* This registers are under AIC (advanced interrupt controller) */
#define AIC_BASE_REG        (0xFFFFF000)
#define AIC_SMR(x)          (AIC_BASE_REG + (x)*0x4)          /* 0 <= x <= 31 */
#define AIC_SVR(x)          (AIC_BASE_REG + 0x0080 + (x)*0x4) /* 0 <= x <= 31 */
#define AIC_IVR             (AIC_BASE_REG + 0x0100)
#define AIC_FVR             (AIC_BASE_REG + 0x0104)
#define AIC_ISR             (AIC_BASE_REG + 0x0108)
#define AIC_IPR             (AIC_BASE_REG + 0x010C)
#define AIC_IMR             (AIC_BASE_REG + 0x0110)
#define AIC_CISR            (AIC_BASE_REG + 0x0114)
#define AIC_IECR            (AIC_BASE_REG + 0x0120)
#define AIC_IDCR            (AIC_BASE_REG + 0x0124)
#define AIC_ICCR            (AIC_BASE_REG + 0x0128)
#define AIC_ISCR            (AIC_BASE_REG + 0x012C)
#define AIC_EOICR           (AIC_BASE_REG + 0x0130)
#define AIC_SPU             (AIC_BASE_REG + 0x0134)
#define AIC_DCR             (AIC_BASE_REG + 0x0138)
#define AIC_FFER            (AIC_BASE_REG + 0x0140)
#define AIC_FFDR            (AIC_BASE_REG + 0x0144)
#define AIC_FFSR            (AIC_BASE_REG + 0x0148)

/* This registers are under PIO (perallel input/output controller) */
#define PIOA_BASE_REG       (0xFFFFF400)
#define PIOB_BASE_REG       (0xFFFFF600)
#define PIOC_BASE_REG       (0xFFFFF800)
#define PIOD_BASE_REG       (0xFFFFFA00)
#define PIO_PER(x)          ((x) + 0x0000) /*  x range :                */
#define PIO_PDR(x)          ((x) + 0x0004) /*     PIOA_BASE_REG         */
#define PIO_PSR(x)          ((x) + 0x0008) /*     PIOB_BASE_REG         */
#define PIO_OER(x)          ((x) + 0x0010) /*     PIOC_BASE_REG         */
#define PIO_ODR(x)          ((x) + 0x0014) /*     PIOD_BASE_REG         */
#define PIO_OSR(x)          ((x) + 0x0018)
#define PIO_IFER(x)         ((x) + 0x0020)
#define PIO_IFDR(x)         ((x) + 0x0024)
#define PIO_IFSR(x)         ((x) + 0x0028)
#define PIO_SODR(x)         ((x) + 0x0030)
#define PIO_CODR(x)         ((x) + 0x0034)
#define PIO_ODSR(x)         ((x) + 0x0038)
#define PIO_PDSR(x)         ((x) + 0x003C)
#define PIO_IER(x)          ((x) + 0x0040)
#define PIO_IDR(x)          ((x) + 0x0044)
#define PIO_IMR(x)          ((x) + 0x0048)
#define PIO_ISR(x)          ((x) + 0x004C)
#define PIO_MDER(x)         ((x) + 0x0050)
#define PIO_MDDR(x)         ((x) + 0x0054)
#define PIO_MDSR(x)         ((x) + 0x0058)
#define PIO_PUDR(x)         ((x) + 0x0060)
#define PIO_PUER(x)         ((x) + 0x0064)
#define PIO_PUSR(x)         ((x) + 0x0068)
#define PIO_ASR(x)          ((x) + 0x0070)
#define PIO_BSR(x)          ((x) + 0x0074)
#define PIO_ABSR(x)         ((x) + 0x0078)
#define PIO_OWER(x)         ((x) + 0x00A0)
#define PIO_OWDR(x)         ((x) + 0x00A4)
#define PIO_OWSR(x)         ((x) + 0x00A8)

/* This registers are under PMC (power management controller) */
#define PMC_BASE_REG        (0xFFFFFC00)
#define PMC_SCER            (PMC_BASE_REG + 0x0000)
#define PMC_SCDR            (PMC_BASE_REG + 0x0004)
#define PMC_SCSR            (PMC_BASE_REG + 0x0008)
#define PMC_PCER            (PMC_BASE_REG + 0x0010)
#define PMC_PCDR            (PMC_BASE_REG + 0x0014)
#define PMC_PCSR            (PMC_BASE_REG + 0x0018)
#define CKGR_MOR            (PMC_BASE_REG + 0x0020)
#define CKGR_MCFR           (PMC_BASE_REG + 0x0024)
#define CKGR_PLLAR          (PMC_BASE_REG + 0x0028)
#define CKGR_PLLBR          (PMC_BASE_REG + 0x002C)
#define PMC_MCKR            (PMC_BASE_REG + 0x0030)
#define PMC_PCK0            (PMC_BASE_REG + 0x0040)
#define PMC_PCK1            (PMC_BASE_REG + 0x0044)
#define PMC_PCK2            (PMC_BASE_REG + 0x0048)
#define PMC_PCK3            (PMC_BASE_REG + 0x004C)
#define PMC_IER             (PMC_BASE_REG + 0x0060)
#define PMC_IDR             (PMC_BASE_REG + 0x0064)
#define PMC_SR              (PMC_BASE_REG + 0x0068)
#define PMC_IMR             (PMC_BASE_REG + 0x006C)

/* This registers are under ST (system timmer) */
#define ST_BASE_REG         (0xFFFFFD00)
#define ST_CR               (ST_BASE_REG + 0x0000)
#define ST_PIMR             (ST_BASE_REG + 0x0004)
#define ST_WDMR             (ST_BASE_REG + 0x0008)
#define ST_RTMR             (ST_BASE_REG + 0x000C)
#define ST_SR               (ST_BASE_REG + 0x0010)
#define ST_IER              (ST_BASE_REG + 0x0014)
#define ST_IDR              (ST_BASE_REG + 0x0018)
#define ST_IMR              (ST_BASE_REG + 0x001C)
#define ST_RTAR             (ST_BASE_REG + 0x0020)
#define ST_CRTR             (ST_BASE_REG + 0x0024)

/* This registers are under RTC (real-time controller) */
#define RTC_BASE_REG        (0xFFFFFE00)
#define RTC_CR              (RTC_BASE_REG + 0x0000)
#define RTC_MR              (RTC_BASE_REG + 0x0004)
#define RTC_TIMR            (RTC_BASE_REG + 0x0008)
#define RTC_CALR            (RTC_BASE_REG + 0x000C)
#define RTC_TIMALR          (RTC_BASE_REG + 0x0010)
#define RTC_CALALR          (RTC_BASE_REG + 0x0014)
#define RTC_SR              (RTC_BASE_REG + 0x0018)
#define RTC_SCCR            (RTC_BASE_REG + 0x001C)
#define RTC_IER             (RTC_BASE_REG + 0x0020)
#define RTC_IDR             (RTC_BASE_REG + 0x0024)
#define RTC_IMR             (RTC_BASE_REG + 0x0028)
#define RTC_VER             (RTC_BASE_REG + 0x002C)

/* This registers are under MC (memory controller) */
#if 1
#define MC_BASE_REG         (0xFFFFFF00)
#define MC_RCR              (MC_BASE_REG + 0x0000)
#define MC_ASR              (MC_BASE_REG + 0x0004)
#define MC_AASR             (MC_BASE_REG + 0x0008)
#define MC_MPR              (MC_BASE_REG + 0x000C)

#define EBI_BASE_REG        (MC_BASE_REG + 0x0060)
#define EBI_CSA             (EBI_BASE_REG + 0x0000)
#define EBI_CFGR            (EBI_BASE_REG + 0x0004)

#define SMC_BASE_REG        (EBI_BASE_REG + 0x0010)
#define SMC_CSR(x)          (SMC_BASE_REG + (x)*0x4)    /* 0 <= x <= 7 */

#define SDRAMC_BASE_REG     (EBI_BASE_REG + 0x0030)
#define SDRAMC_MR           (SDRAMC_BASE_REG + 0x0000)
#define SDRAMC_TR           (SDRAMC_BASE_REG + 0x0004)
#define SDRAMC_CR           (SDRAMC_BASE_REG + 0x0008)
#define SDRAMC_SRR          (SDRAMC_BASE_REG + 0x000C)
#define SDRAMC_LPR          (SDRAMC_BASE_REG + 0x0010)
#define SDRAMC_IER          (SDRAMC_BASE_REG + 0x0014)
#define SDRAMC_IDR          (SDRAMC_BASE_REG + 0x0018)
#define SDRAMC_IMR          (SDRAMC_BASE_REG + 0x001C)
#define SDRAMC_ISR          (SDRAMC_BASE_REG + 0x0020)

#define BFC_BASE_REG        (EBI_BASE_REG + 0x0060)
#endif


#ifdef __cplusplus
}
#endif

#endif /* __REG_H */

