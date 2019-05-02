/*******************************************************************************
* File Name: UART_tempINT.c
* Version 2.50
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UART_temp.h"
#include "cyapicallbacks.h"


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (UART_temp_RX_INTERRUPT_ENABLED && (UART_temp_RX_ENABLED || UART_temp_HD_ENABLED))
    /*******************************************************************************
    * Function Name: UART_temp_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_temp_rxBuffer - RAM buffer pointer for save received data.
    *  UART_temp_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  UART_temp_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  UART_temp_rxBufferOverflow - software overflow flag. Set to one
    *     when UART_temp_rxBufferWrite index overtakes
    *     UART_temp_rxBufferRead index.
    *  UART_temp_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when UART_temp_rxBufferWrite is equal to
    *    UART_temp_rxBufferRead
    *  UART_temp_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  UART_temp_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_temp_RXISR)
    {
        uint8 readData;
        uint8 readStatus;
        uint8 increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART_temp_RXISR_ENTRY_CALLBACK
        UART_temp_RXISR_EntryCallback();
    #endif /* UART_temp_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART_temp_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = UART_temp_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in UART_temp_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (UART_temp_RX_STS_BREAK | 
                            UART_temp_RX_STS_PAR_ERROR |
                            UART_temp_RX_STS_STOP_ERROR | 
                            UART_temp_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                UART_temp_errorStatus |= readStatus & ( UART_temp_RX_STS_BREAK | 
                                                            UART_temp_RX_STS_PAR_ERROR | 
                                                            UART_temp_RX_STS_STOP_ERROR | 
                                                            UART_temp_RX_STS_OVERRUN);
                /* `#START UART_temp_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef UART_temp_RXISR_ERROR_CALLBACK
                UART_temp_RXISR_ERROR_Callback();
            #endif /* UART_temp_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & UART_temp_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = UART_temp_RXDATA_REG;
            #if (UART_temp_RXHW_ADDRESS_ENABLED)
                if(UART_temp_rxAddressMode == (uint8)UART_temp__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & UART_temp_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & UART_temp_RX_STS_ADDR_MATCH) != 0u)
                        {
                            UART_temp_rxAddressDetected = 1u;
                        }
                        else
                        {
                            UART_temp_rxAddressDetected = 0u;
                        }
                    }
                    if(UART_temp_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        UART_temp_rxBuffer[UART_temp_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    UART_temp_rxBuffer[UART_temp_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                UART_temp_rxBuffer[UART_temp_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (UART_temp_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(UART_temp_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        UART_temp_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    UART_temp_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(UART_temp_rxBufferWrite >= UART_temp_RX_BUFFER_SIZE)
                    {
                        UART_temp_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(UART_temp_rxBufferWrite == UART_temp_rxBufferRead)
                    {
                        UART_temp_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (UART_temp_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            UART_temp_RXSTATUS_MASK_REG  &= (uint8)~UART_temp_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(UART_temp_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (UART_temp_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & UART_temp_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START UART_temp_RXISR_END` */

        /* `#END` */

    #ifdef UART_temp_RXISR_EXIT_CALLBACK
        UART_temp_RXISR_ExitCallback();
    #endif /* UART_temp_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (UART_temp_RX_INTERRUPT_ENABLED && (UART_temp_RX_ENABLED || UART_temp_HD_ENABLED)) */


#if (UART_temp_TX_INTERRUPT_ENABLED && UART_temp_TX_ENABLED)
    /*******************************************************************************
    * Function Name: UART_temp_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  UART_temp_txBuffer - RAM buffer pointer for transmit data from.
    *  UART_temp_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  UART_temp_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(UART_temp_TXISR)
    {
    #if(CY_PSOC3)
        uint8 int_en;
    #endif /* (CY_PSOC3) */

    #ifdef UART_temp_TXISR_ENTRY_CALLBACK
        UART_temp_TXISR_EntryCallback();
    #endif /* UART_temp_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START UART_temp_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((UART_temp_txBufferRead != UART_temp_txBufferWrite) &&
             ((UART_temp_TXSTATUS_REG & UART_temp_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(UART_temp_txBufferRead >= UART_temp_TX_BUFFER_SIZE)
            {
                UART_temp_txBufferRead = 0u;
            }

            UART_temp_TXDATA_REG = UART_temp_txBuffer[UART_temp_txBufferRead];

            /* Set next pointer */
            UART_temp_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START UART_temp_TXISR_END` */

        /* `#END` */

    #ifdef UART_temp_TXISR_EXIT_CALLBACK
        UART_temp_TXISR_ExitCallback();
    #endif /* UART_temp_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (UART_temp_TX_INTERRUPT_ENABLED && UART_temp_TX_ENABLED) */


/* [] END OF FILE */
