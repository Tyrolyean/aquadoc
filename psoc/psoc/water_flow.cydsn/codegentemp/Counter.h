/*******************************************************************************
* File Name: Counter.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Counter_H) /* Pins Counter_H */
#define CY_PINS_Counter_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Counter_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Counter__PORT == 15 && ((Counter__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Counter_Write(uint8 value);
void    Counter_SetDriveMode(uint8 mode);
uint8   Counter_ReadDataReg(void);
uint8   Counter_Read(void);
void    Counter_SetInterruptMode(uint16 position, uint16 mode);
uint8   Counter_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Counter_SetDriveMode() function.
     *  @{
     */
        #define Counter_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Counter_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Counter_DM_RES_UP          PIN_DM_RES_UP
        #define Counter_DM_RES_DWN         PIN_DM_RES_DWN
        #define Counter_DM_OD_LO           PIN_DM_OD_LO
        #define Counter_DM_OD_HI           PIN_DM_OD_HI
        #define Counter_DM_STRONG          PIN_DM_STRONG
        #define Counter_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Counter_MASK               Counter__MASK
#define Counter_SHIFT              Counter__SHIFT
#define Counter_WIDTH              1u

/* Interrupt constants */
#if defined(Counter__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Counter_SetInterruptMode() function.
     *  @{
     */
        #define Counter_INTR_NONE      (uint16)(0x0000u)
        #define Counter_INTR_RISING    (uint16)(0x0001u)
        #define Counter_INTR_FALLING   (uint16)(0x0002u)
        #define Counter_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Counter_INTR_MASK      (0x01u) 
#endif /* (Counter__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Counter_PS                     (* (reg8 *) Counter__PS)
/* Data Register */
#define Counter_DR                     (* (reg8 *) Counter__DR)
/* Port Number */
#define Counter_PRT_NUM                (* (reg8 *) Counter__PRT) 
/* Connect to Analog Globals */                                                  
#define Counter_AG                     (* (reg8 *) Counter__AG)                       
/* Analog MUX bux enable */
#define Counter_AMUX                   (* (reg8 *) Counter__AMUX) 
/* Bidirectional Enable */                                                        
#define Counter_BIE                    (* (reg8 *) Counter__BIE)
/* Bit-mask for Aliased Register Access */
#define Counter_BIT_MASK               (* (reg8 *) Counter__BIT_MASK)
/* Bypass Enable */
#define Counter_BYP                    (* (reg8 *) Counter__BYP)
/* Port wide control signals */                                                   
#define Counter_CTL                    (* (reg8 *) Counter__CTL)
/* Drive Modes */
#define Counter_DM0                    (* (reg8 *) Counter__DM0) 
#define Counter_DM1                    (* (reg8 *) Counter__DM1)
#define Counter_DM2                    (* (reg8 *) Counter__DM2) 
/* Input Buffer Disable Override */
#define Counter_INP_DIS                (* (reg8 *) Counter__INP_DIS)
/* LCD Common or Segment Drive */
#define Counter_LCD_COM_SEG            (* (reg8 *) Counter__LCD_COM_SEG)
/* Enable Segment LCD */
#define Counter_LCD_EN                 (* (reg8 *) Counter__LCD_EN)
/* Slew Rate Control */
#define Counter_SLW                    (* (reg8 *) Counter__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Counter_PRTDSI__CAPS_SEL       (* (reg8 *) Counter__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Counter_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Counter__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Counter_PRTDSI__OE_SEL0        (* (reg8 *) Counter__PRTDSI__OE_SEL0) 
#define Counter_PRTDSI__OE_SEL1        (* (reg8 *) Counter__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Counter_PRTDSI__OUT_SEL0       (* (reg8 *) Counter__PRTDSI__OUT_SEL0) 
#define Counter_PRTDSI__OUT_SEL1       (* (reg8 *) Counter__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Counter_PRTDSI__SYNC_OUT       (* (reg8 *) Counter__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Counter__SIO_CFG)
    #define Counter_SIO_HYST_EN        (* (reg8 *) Counter__SIO_HYST_EN)
    #define Counter_SIO_REG_HIFREQ     (* (reg8 *) Counter__SIO_REG_HIFREQ)
    #define Counter_SIO_CFG            (* (reg8 *) Counter__SIO_CFG)
    #define Counter_SIO_DIFF           (* (reg8 *) Counter__SIO_DIFF)
#endif /* (Counter__SIO_CFG) */

/* Interrupt Registers */
#if defined(Counter__INTSTAT)
    #define Counter_INTSTAT            (* (reg8 *) Counter__INTSTAT)
    #define Counter_SNAP               (* (reg8 *) Counter__SNAP)
    
	#define Counter_0_INTTYPE_REG 		(* (reg8 *) Counter__0__INTTYPE)
#endif /* (Counter__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Counter_H */


/* [] END OF FILE */
