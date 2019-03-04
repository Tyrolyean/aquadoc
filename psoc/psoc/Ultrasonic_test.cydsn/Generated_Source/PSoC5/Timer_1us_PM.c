/*******************************************************************************
* File Name: Timer_1us_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Timer_1us.h"

static Timer_1us_backupStruct Timer_1us_backup;


/*******************************************************************************
* Function Name: Timer_1us_SaveConfig
********************************************************************************
*
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
*  Timer_1us_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Timer_1us_SaveConfig(void) 
{
    #if (!Timer_1us_UsingFixedFunction)
        Timer_1us_backup.TimerUdb = Timer_1us_ReadCounter();
        Timer_1us_backup.InterruptMaskValue = Timer_1us_STATUS_MASK;
        #if (Timer_1us_UsingHWCaptureCounter)
            Timer_1us_backup.TimerCaptureCounter = Timer_1us_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Timer_1us_UDB_CONTROL_REG_REMOVED)
            Timer_1us_backup.TimerControlRegister = Timer_1us_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Timer_1us_RestoreConfig
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
*  Timer_1us_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_1us_RestoreConfig(void) 
{   
    #if (!Timer_1us_UsingFixedFunction)

        Timer_1us_WriteCounter(Timer_1us_backup.TimerUdb);
        Timer_1us_STATUS_MASK =Timer_1us_backup.InterruptMaskValue;
        #if (Timer_1us_UsingHWCaptureCounter)
            Timer_1us_SetCaptureCount(Timer_1us_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Timer_1us_UDB_CONTROL_REG_REMOVED)
            Timer_1us_WriteControlRegister(Timer_1us_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Timer_1us_Sleep
********************************************************************************
*
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
*  Timer_1us_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Timer_1us_Sleep(void) 
{
    #if(!Timer_1us_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Timer_1us_CTRL_ENABLE == (Timer_1us_CONTROL & Timer_1us_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Timer_1us_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Timer_1us_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Timer_1us_Stop();
    Timer_1us_SaveConfig();
}


/*******************************************************************************
* Function Name: Timer_1us_Wakeup
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
*  Timer_1us_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Timer_1us_Wakeup(void) 
{
    Timer_1us_RestoreConfig();
    #if(!Timer_1us_UDB_CONTROL_REG_REMOVED)
        if(Timer_1us_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Timer_1us_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
