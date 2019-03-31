/*******************************************************************************
* File Name: BatteryAlarm.h  
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

#if !defined(CY_PINS_BatteryAlarm_H) /* Pins BatteryAlarm_H */
#define CY_PINS_BatteryAlarm_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "BatteryAlarm_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 BatteryAlarm__PORT == 15 && ((BatteryAlarm__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    BatteryAlarm_Write(uint8 value);
void    BatteryAlarm_SetDriveMode(uint8 mode);
uint8   BatteryAlarm_ReadDataReg(void);
uint8   BatteryAlarm_Read(void);
void    BatteryAlarm_SetInterruptMode(uint16 position, uint16 mode);
uint8   BatteryAlarm_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the BatteryAlarm_SetDriveMode() function.
     *  @{
     */
        #define BatteryAlarm_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define BatteryAlarm_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define BatteryAlarm_DM_RES_UP          PIN_DM_RES_UP
        #define BatteryAlarm_DM_RES_DWN         PIN_DM_RES_DWN
        #define BatteryAlarm_DM_OD_LO           PIN_DM_OD_LO
        #define BatteryAlarm_DM_OD_HI           PIN_DM_OD_HI
        #define BatteryAlarm_DM_STRONG          PIN_DM_STRONG
        #define BatteryAlarm_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define BatteryAlarm_MASK               BatteryAlarm__MASK
#define BatteryAlarm_SHIFT              BatteryAlarm__SHIFT
#define BatteryAlarm_WIDTH              1u

/* Interrupt constants */
#if defined(BatteryAlarm__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in BatteryAlarm_SetInterruptMode() function.
     *  @{
     */
        #define BatteryAlarm_INTR_NONE      (uint16)(0x0000u)
        #define BatteryAlarm_INTR_RISING    (uint16)(0x0001u)
        #define BatteryAlarm_INTR_FALLING   (uint16)(0x0002u)
        #define BatteryAlarm_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define BatteryAlarm_INTR_MASK      (0x01u) 
#endif /* (BatteryAlarm__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define BatteryAlarm_PS                     (* (reg8 *) BatteryAlarm__PS)
/* Data Register */
#define BatteryAlarm_DR                     (* (reg8 *) BatteryAlarm__DR)
/* Port Number */
#define BatteryAlarm_PRT_NUM                (* (reg8 *) BatteryAlarm__PRT) 
/* Connect to Analog Globals */                                                  
#define BatteryAlarm_AG                     (* (reg8 *) BatteryAlarm__AG)                       
/* Analog MUX bux enable */
#define BatteryAlarm_AMUX                   (* (reg8 *) BatteryAlarm__AMUX) 
/* Bidirectional Enable */                                                        
#define BatteryAlarm_BIE                    (* (reg8 *) BatteryAlarm__BIE)
/* Bit-mask for Aliased Register Access */
#define BatteryAlarm_BIT_MASK               (* (reg8 *) BatteryAlarm__BIT_MASK)
/* Bypass Enable */
#define BatteryAlarm_BYP                    (* (reg8 *) BatteryAlarm__BYP)
/* Port wide control signals */                                                   
#define BatteryAlarm_CTL                    (* (reg8 *) BatteryAlarm__CTL)
/* Drive Modes */
#define BatteryAlarm_DM0                    (* (reg8 *) BatteryAlarm__DM0) 
#define BatteryAlarm_DM1                    (* (reg8 *) BatteryAlarm__DM1)
#define BatteryAlarm_DM2                    (* (reg8 *) BatteryAlarm__DM2) 
/* Input Buffer Disable Override */
#define BatteryAlarm_INP_DIS                (* (reg8 *) BatteryAlarm__INP_DIS)
/* LCD Common or Segment Drive */
#define BatteryAlarm_LCD_COM_SEG            (* (reg8 *) BatteryAlarm__LCD_COM_SEG)
/* Enable Segment LCD */
#define BatteryAlarm_LCD_EN                 (* (reg8 *) BatteryAlarm__LCD_EN)
/* Slew Rate Control */
#define BatteryAlarm_SLW                    (* (reg8 *) BatteryAlarm__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define BatteryAlarm_PRTDSI__CAPS_SEL       (* (reg8 *) BatteryAlarm__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define BatteryAlarm_PRTDSI__DBL_SYNC_IN    (* (reg8 *) BatteryAlarm__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define BatteryAlarm_PRTDSI__OE_SEL0        (* (reg8 *) BatteryAlarm__PRTDSI__OE_SEL0) 
#define BatteryAlarm_PRTDSI__OE_SEL1        (* (reg8 *) BatteryAlarm__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define BatteryAlarm_PRTDSI__OUT_SEL0       (* (reg8 *) BatteryAlarm__PRTDSI__OUT_SEL0) 
#define BatteryAlarm_PRTDSI__OUT_SEL1       (* (reg8 *) BatteryAlarm__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define BatteryAlarm_PRTDSI__SYNC_OUT       (* (reg8 *) BatteryAlarm__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(BatteryAlarm__SIO_CFG)
    #define BatteryAlarm_SIO_HYST_EN        (* (reg8 *) BatteryAlarm__SIO_HYST_EN)
    #define BatteryAlarm_SIO_REG_HIFREQ     (* (reg8 *) BatteryAlarm__SIO_REG_HIFREQ)
    #define BatteryAlarm_SIO_CFG            (* (reg8 *) BatteryAlarm__SIO_CFG)
    #define BatteryAlarm_SIO_DIFF           (* (reg8 *) BatteryAlarm__SIO_DIFF)
#endif /* (BatteryAlarm__SIO_CFG) */

/* Interrupt Registers */
#if defined(BatteryAlarm__INTSTAT)
    #define BatteryAlarm_INTSTAT            (* (reg8 *) BatteryAlarm__INTSTAT)
    #define BatteryAlarm_SNAP               (* (reg8 *) BatteryAlarm__SNAP)
    
	#define BatteryAlarm_0_INTTYPE_REG 		(* (reg8 *) BatteryAlarm__0__INTTYPE)
#endif /* (BatteryAlarm__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_BatteryAlarm_H */


/* [] END OF FILE */
