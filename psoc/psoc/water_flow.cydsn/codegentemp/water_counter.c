/*******************************************************************************
* File Name: water_counter.c  
* Version 3.0
*
*  Description:
*     The Counter component consists of a 8, 16, 24 or 32-bit counter with
*     a selectable period between 2 and 2^Width - 1.  
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

uint8 water_counter_initVar = 0u;


/*******************************************************************************
* Function Name: water_counter_Init
********************************************************************************
* Summary:
*     Initialize to the schematic state
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
*******************************************************************************/
void water_counter_Init(void) 
{
        #if (!water_counter_UsingFixedFunction && !water_counter_ControlRegRemoved)
            uint8 ctrl;
        #endif /* (!water_counter_UsingFixedFunction && !water_counter_ControlRegRemoved) */
        
        #if(!water_counter_UsingFixedFunction) 
            /* Interrupt State Backup for Critical Region*/
            uint8 water_counter_interruptState;
        #endif /* (!water_counter_UsingFixedFunction) */
        
        #if (water_counter_UsingFixedFunction)
            /* Clear all bits but the enable bit (if it's already set for Timer operation */
            water_counter_CONTROL &= water_counter_CTRL_ENABLE;
            
            /* Clear the mode bits for continuous run mode */
            #if (CY_PSOC5A)
                water_counter_CONTROL2 &= ((uint8)(~water_counter_CTRL_MODE_MASK));
            #endif /* (CY_PSOC5A) */
            #if (CY_PSOC3 || CY_PSOC5LP)
                water_counter_CONTROL3 &= ((uint8)(~water_counter_CTRL_MODE_MASK));                
            #endif /* (CY_PSOC3 || CY_PSOC5LP) */
            /* Check if One Shot mode is enabled i.e. RunMode !=0*/
            #if (water_counter_RunModeUsed != 0x0u)
                /* Set 3rd bit of Control register to enable one shot mode */
                water_counter_CONTROL |= water_counter_ONESHOT;
            #endif /* (water_counter_RunModeUsed != 0x0u) */
            
            /* Set the IRQ to use the status register interrupts */
            water_counter_CONTROL2 |= water_counter_CTRL2_IRQ_SEL;
            
            /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
            water_counter_RT1 &= ((uint8)(~water_counter_RT1_MASK));
            water_counter_RT1 |= water_counter_SYNC;     
                    
            /*Enable DSI Sync all all inputs of the Timer*/
            water_counter_RT1 &= ((uint8)(~water_counter_SYNCDSI_MASK));
            water_counter_RT1 |= water_counter_SYNCDSI_EN;

        #else
            #if(!water_counter_ControlRegRemoved)
            /* Set the default compare mode defined in the parameter */
            ctrl = water_counter_CONTROL & ((uint8)(~water_counter_CTRL_CMPMODE_MASK));
            water_counter_CONTROL = ctrl | water_counter_DEFAULT_COMPARE_MODE;
            
            /* Set the default capture mode defined in the parameter */
            ctrl = water_counter_CONTROL & ((uint8)(~water_counter_CTRL_CAPMODE_MASK));
            
            #if( 0 != water_counter_CAPTURE_MODE_CONF)
                water_counter_CONTROL = ctrl | water_counter_DEFAULT_CAPTURE_MODE;
            #else
                water_counter_CONTROL = ctrl;
            #endif /* 0 != water_counter_CAPTURE_MODE */ 
            
            #endif /* (!water_counter_ControlRegRemoved) */
        #endif /* (water_counter_UsingFixedFunction) */
        
        /* Clear all data in the FIFO's */
        #if (!water_counter_UsingFixedFunction)
            water_counter_ClearFIFO();
        #endif /* (!water_counter_UsingFixedFunction) */
        
        /* Set Initial values from Configuration */
        water_counter_WritePeriod(water_counter_INIT_PERIOD_VALUE);
        #if (!(water_counter_UsingFixedFunction && (CY_PSOC5A)))
            water_counter_WriteCounter(water_counter_INIT_COUNTER_VALUE);
        #endif /* (!(water_counter_UsingFixedFunction && (CY_PSOC5A))) */
        water_counter_SetInterruptMode(water_counter_INIT_INTERRUPTS_MASK);
        
        #if (!water_counter_UsingFixedFunction)
            /* Read the status register to clear the unwanted interrupts */
            (void)water_counter_ReadStatusRegister();
            /* Set the compare value (only available to non-fixed function implementation */
            water_counter_WriteCompare(water_counter_INIT_COMPARE_VALUE);
            /* Use the interrupt output of the status register for IRQ output */
            
            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            water_counter_interruptState = CyEnterCriticalSection();
            
            water_counter_STATUS_AUX_CTRL |= water_counter_STATUS_ACTL_INT_EN_MASK;
            
            /* Exit Critical Region*/
            CyExitCriticalSection(water_counter_interruptState);
            
        #endif /* (!water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_Enable
********************************************************************************
* Summary:
*     Enable the Counter
* 
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: 
*   If the Enable mode is set to Hardware only then this function has no effect 
*   on the operation of the counter.
*
*******************************************************************************/
void water_counter_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (water_counter_UsingFixedFunction)
        water_counter_GLOBAL_ENABLE |= water_counter_BLOCK_EN_MASK;
        water_counter_GLOBAL_STBY_ENABLE |= water_counter_BLOCK_STBY_EN_MASK;
    #endif /* (water_counter_UsingFixedFunction) */  
        
    /* Enable the counter from the control register  */
    /* If Fixed Function then make sure Mode is set correctly */
    /* else make sure reset is clear */
    #if(!water_counter_ControlRegRemoved || water_counter_UsingFixedFunction)
        water_counter_CONTROL |= water_counter_CTRL_ENABLE;                
    #endif /* (!water_counter_ControlRegRemoved || water_counter_UsingFixedFunction) */
    
}


/*******************************************************************************
* Function Name: water_counter_Start
********************************************************************************
* Summary:
*  Enables the counter for operation 
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Global variables:
*  water_counter_initVar: Is modified when this function is called for the  
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void water_counter_Start(void) 
{
    if(water_counter_initVar == 0u)
    {
        water_counter_Init();
        
        water_counter_initVar = 1u; /* Clear this bit for Initialization */        
    }
    
    /* Enable the Counter */
    water_counter_Enable();        
}


/*******************************************************************************
* Function Name: water_counter_Stop
********************************************************************************
* Summary:
* Halts the counter, but does not change any modes or disable interrupts.
*
* Parameters:  
*  void  
*
* Return: 
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the counter.
*
*******************************************************************************/
void water_counter_Stop(void) 
{
    /* Disable Counter */
    #if(!water_counter_ControlRegRemoved || water_counter_UsingFixedFunction)
        water_counter_CONTROL &= ((uint8)(~water_counter_CTRL_ENABLE));        
    #endif /* (!water_counter_ControlRegRemoved || water_counter_UsingFixedFunction) */
    
    /* Globally disable the Fixed Function Block chosen */
    #if (water_counter_UsingFixedFunction)
        water_counter_GLOBAL_ENABLE &= ((uint8)(~water_counter_BLOCK_EN_MASK));
        water_counter_GLOBAL_STBY_ENABLE &= ((uint8)(~water_counter_BLOCK_STBY_EN_MASK));
    #endif /* (water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_SetInterruptMode
********************************************************************************
* Summary:
* Configures which interrupt sources are enabled to generate the final interrupt
*
* Parameters:  
*  InterruptsMask: This parameter is an or'd collection of the status bits
*                   which will be allowed to generate the counters interrupt.   
*
* Return: 
*  void
*
*******************************************************************************/
void water_counter_SetInterruptMode(uint8 interruptsMask) 
{
    water_counter_STATUS_MASK = interruptsMask;
}


/*******************************************************************************
* Function Name: water_counter_ReadStatusRegister
********************************************************************************
* Summary:
*   Reads the status register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the status register
*
* Side Effects:
*   Status register bits may be clear on read. 
*
*******************************************************************************/
uint8   water_counter_ReadStatusRegister(void) 
{
    return water_counter_STATUS;
}


#if(!water_counter_ControlRegRemoved)
/*******************************************************************************
* Function Name: water_counter_ReadControlRegister
********************************************************************************
* Summary:
*   Reads the control register and returns it's state. This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
uint8   water_counter_ReadControlRegister(void) 
{
    return water_counter_CONTROL;
}


/*******************************************************************************
* Function Name: water_counter_WriteControlRegister
********************************************************************************
* Summary:
*   Sets the bit-field of the control register.  This function should use
*       defined types for the bit-field information as the bits in this
*       register may be permuteable.
*
* Parameters:  
*  void
*
* Return: 
*  (uint8) The contents of the control register
*
*******************************************************************************/
void    water_counter_WriteControlRegister(uint8 control) 
{
    water_counter_CONTROL = control;
}

#endif  /* (!water_counter_ControlRegRemoved) */


#if (!(water_counter_UsingFixedFunction && (CY_PSOC5A)))
/*******************************************************************************
* Function Name: water_counter_WriteCounter
********************************************************************************
* Summary:
*   This funtion is used to set the counter to a specific value
*
* Parameters:  
*  counter:  New counter value. 
*
* Return: 
*  void 
*
*******************************************************************************/
void water_counter_WriteCounter(uint32 counter) \
                                   
{
    #if(water_counter_UsingFixedFunction)
        /* assert if block is already enabled */
        CYASSERT (0u == (water_counter_GLOBAL_ENABLE & water_counter_BLOCK_EN_MASK));
        /* If block is disabled, enable it and then write the counter */
        water_counter_GLOBAL_ENABLE |= water_counter_BLOCK_EN_MASK;
        CY_SET_REG16(water_counter_COUNTER_LSB_PTR, (uint16)counter);
        water_counter_GLOBAL_ENABLE &= ((uint8)(~water_counter_BLOCK_EN_MASK));
    #else
        CY_SET_REG24(water_counter_COUNTER_LSB_PTR, counter);
    #endif /* (water_counter_UsingFixedFunction) */
}
#endif /* (!(water_counter_UsingFixedFunction && (CY_PSOC5A))) */


/*******************************************************************************
* Function Name: water_counter_ReadCounter
********************************************************************************
* Summary:
* Returns the current value of the counter.  It doesn't matter
* if the counter is enabled or running.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) The present value of the counter.
*
*******************************************************************************/
uint32 water_counter_ReadCounter(void) 
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(water_counter_UsingFixedFunction)
		(void)CY_GET_REG16(water_counter_COUNTER_LSB_PTR);
	#else
		(void)CY_GET_REG8(water_counter_COUNTER_LSB_PTR_8BIT);
	#endif/* (water_counter_UsingFixedFunction) */
    
    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(water_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(water_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG24(water_counter_STATICCOUNT_LSB_PTR));
    #endif /* (water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_ReadCapture
********************************************************************************
* Summary:
*   This function returns the last value captured.
*
* Parameters:  
*  void
*
* Return: 
*  (uint32) Present Capture value.
*
*******************************************************************************/
uint32 water_counter_ReadCapture(void) 
{
    #if(water_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(water_counter_STATICCOUNT_LSB_PTR));
    #else
        return (CY_GET_REG24(water_counter_STATICCOUNT_LSB_PTR));
    #endif /* (water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_WritePeriod
********************************************************************************
* Summary:
* Changes the period of the counter.  The new period 
* will be loaded the next time terminal count is detected.
*
* Parameters:  
*  period: (uint32) A value of 0 will result in
*         the counter remaining at zero.  
*
* Return: 
*  void
*
*******************************************************************************/
void water_counter_WritePeriod(uint32 period) 
{
    #if(water_counter_UsingFixedFunction)
        CY_SET_REG16(water_counter_PERIOD_LSB_PTR,(uint16)period);
    #else
        CY_SET_REG24(water_counter_PERIOD_LSB_PTR, period);
    #endif /* (water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_ReadPeriod
********************************************************************************
* Summary:
* Reads the current period value without affecting counter operation.
*
* Parameters:  
*  void:  
*
* Return: 
*  (uint32) Present period value.
*
*******************************************************************************/
uint32 water_counter_ReadPeriod(void) 
{
    #if(water_counter_UsingFixedFunction)
        return ((uint32)CY_GET_REG16(water_counter_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG24(water_counter_PERIOD_LSB_PTR));
    #endif /* (water_counter_UsingFixedFunction) */
}


#if (!water_counter_UsingFixedFunction)
/*******************************************************************************
* Function Name: water_counter_WriteCompare
********************************************************************************
* Summary:
* Changes the compare value.  The compare output will 
* reflect the new value on the next UDB clock.  The compare output will be 
* driven high when the present counter value compares true based on the 
* configured compare mode setting. 
*
* Parameters:  
*  Compare:  New compare value. 
*
* Return: 
*  void
*
*******************************************************************************/
void water_counter_WriteCompare(uint32 compare) \
                                   
{
    #if(water_counter_UsingFixedFunction)
        CY_SET_REG16(water_counter_COMPARE_LSB_PTR, (uint16)compare);
    #else
        CY_SET_REG24(water_counter_COMPARE_LSB_PTR, compare);
    #endif /* (water_counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: water_counter_ReadCompare
********************************************************************************
* Summary:
* Returns the compare value.
*
* Parameters:  
*  void:
*
* Return: 
*  (uint32) Present compare value.
*
*******************************************************************************/
uint32 water_counter_ReadCompare(void) 
{
    return (CY_GET_REG24(water_counter_COMPARE_LSB_PTR));
}


#if (water_counter_COMPARE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: water_counter_SetCompareMode
********************************************************************************
* Summary:
*  Sets the software controlled Compare Mode.
*
* Parameters:
*  compareMode:  Compare Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void water_counter_SetCompareMode(uint8 compareMode) 
{
    /* Clear the compare mode bits in the control register */
    water_counter_CONTROL &= ((uint8)(~water_counter_CTRL_CMPMODE_MASK));
    
    /* Write the new setting */
    water_counter_CONTROL |= compareMode;
}
#endif  /* (water_counter_COMPARE_MODE_SOFTWARE) */


#if (water_counter_CAPTURE_MODE_SOFTWARE)
/*******************************************************************************
* Function Name: water_counter_SetCaptureMode
********************************************************************************
* Summary:
*  Sets the software controlled Capture Mode.
*
* Parameters:
*  captureMode:  Capture Mode Enumerated Type.
*
* Return:
*  void
*
*******************************************************************************/
void water_counter_SetCaptureMode(uint8 captureMode) 
{
    /* Clear the capture mode bits in the control register */
    water_counter_CONTROL &= ((uint8)(~water_counter_CTRL_CAPMODE_MASK));
    
    /* Write the new setting */
    water_counter_CONTROL |= ((uint8)((uint8)captureMode << water_counter_CTRL_CAPMODE0_SHIFT));
}
#endif  /* (water_counter_CAPTURE_MODE_SOFTWARE) */


/*******************************************************************************
* Function Name: water_counter_ClearFIFO
********************************************************************************
* Summary:
*   This function clears all capture data from the capture FIFO
*
* Parameters:  
*  void:
*
* Return: 
*  None
*
*******************************************************************************/
void water_counter_ClearFIFO(void) 
{

    while(0u != (water_counter_ReadStatusRegister() & water_counter_STATUS_FIFONEMP))
    {
        (void)water_counter_ReadCapture();
    }

}
#endif  /* (!water_counter_UsingFixedFunction) */


/* [] END OF FILE */

