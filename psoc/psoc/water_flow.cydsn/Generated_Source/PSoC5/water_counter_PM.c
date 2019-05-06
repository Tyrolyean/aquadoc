/*******************************************************************************
* File Name: water_counter_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "water_counter.h"

static water_counter_backupStruct water_counter_backup;


/*******************************************************************************
* Function Name: water_counter_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  water_counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void water_counter_SaveConfig(void) 
{
    #if (!water_counter_UsingFixedFunction)

        water_counter_backup.CounterUdb = water_counter_ReadCounter();

        #if(!water_counter_ControlRegRemoved)
            water_counter_backup.CounterControlRegister = water_counter_ReadControlRegister();
        #endif /* (!water_counter_ControlRegRemoved) */

    #endif /* (!water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  water_counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void water_counter_RestoreConfig(void) 
{      
    #if (!water_counter_UsingFixedFunction)

       water_counter_WriteCounter(water_counter_backup.CounterUdb);

        #if(!water_counter_ControlRegRemoved)
            water_counter_WriteControlRegister(water_counter_backup.CounterControlRegister);
        #endif /* (!water_counter_ControlRegRemoved) */

    #endif /* (!water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  water_counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void water_counter_Sleep(void) 
{
    #if(!water_counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(water_counter_CTRL_ENABLE == (water_counter_CONTROL & water_counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            water_counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            water_counter_backup.CounterEnableState = 0u;
        }
    #else
        water_counter_backup.CounterEnableState = 1u;
        if(water_counter_backup.CounterEnableState != 0u)
        {
            water_counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!water_counter_ControlRegRemoved) */
    
    water_counter_Stop();
    water_counter_SaveConfig();
}


/*******************************************************************************
* Function Name: water_counter_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  water_counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void water_counter_Wakeup(void) 
{
    water_counter_RestoreConfig();
    #if(!water_counter_ControlRegRemoved)
        if(water_counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            water_counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!water_counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
