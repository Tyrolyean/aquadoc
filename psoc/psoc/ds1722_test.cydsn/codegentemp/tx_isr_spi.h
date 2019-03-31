/*******************************************************************************
* File Name: tx_isr_spi.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_tx_isr_spi_H)
#define CY_ISR_tx_isr_spi_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void tx_isr_spi_Start(void);
void tx_isr_spi_StartEx(cyisraddress address);
void tx_isr_spi_Stop(void);

CY_ISR_PROTO(tx_isr_spi_Interrupt);

void tx_isr_spi_SetVector(cyisraddress address);
cyisraddress tx_isr_spi_GetVector(void);

void tx_isr_spi_SetPriority(uint8 priority);
uint8 tx_isr_spi_GetPriority(void);

void tx_isr_spi_Enable(void);
uint8 tx_isr_spi_GetState(void);
void tx_isr_spi_Disable(void);

void tx_isr_spi_SetPending(void);
void tx_isr_spi_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the tx_isr_spi ISR. */
#define tx_isr_spi_INTC_VECTOR            ((reg32 *) tx_isr_spi__INTC_VECT)

/* Address of the tx_isr_spi ISR priority. */
#define tx_isr_spi_INTC_PRIOR             ((reg8 *) tx_isr_spi__INTC_PRIOR_REG)

/* Priority of the tx_isr_spi interrupt. */
#define tx_isr_spi_INTC_PRIOR_NUMBER      tx_isr_spi__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable tx_isr_spi interrupt. */
#define tx_isr_spi_INTC_SET_EN            ((reg32 *) tx_isr_spi__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the tx_isr_spi interrupt. */
#define tx_isr_spi_INTC_CLR_EN            ((reg32 *) tx_isr_spi__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the tx_isr_spi interrupt state to pending. */
#define tx_isr_spi_INTC_SET_PD            ((reg32 *) tx_isr_spi__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the tx_isr_spi interrupt. */
#define tx_isr_spi_INTC_CLR_PD            ((reg32 *) tx_isr_spi__INTC_CLR_PD_REG)


#endif /* CY_ISR_tx_isr_spi_H */


/* [] END OF FILE */
