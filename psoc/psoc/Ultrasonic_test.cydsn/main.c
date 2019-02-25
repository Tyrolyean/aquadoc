/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <unistd.h>
#include <stdio.h>

size_t get_hc_sr04(){
    
    /* If the timeout is reached, the sensor is 
     * assumed to have been disconnected
     */
    
    #define ERRROR_CORRECTION 500
    
    #define TIMEOUT 50000
    
    /* Place your application code here. */
    Trigger_Impuls_Write(1);
    CyDelayUs(10);
    Trigger_Impuls_Write(0);
    
    for(int i = 0; i < TIMEOUT && !Echo_Input_Read(); i++){
        CyDelayUs(1);
    }
    
    //ultimate Baustelle
    size_t counter;
    for(counter = 0;Echo_Input_Read() == 1 && counter <= TIMEOUT; counter ++){
        CyDelayUs(1);
    }
    return counter + ERRROR_CORRECTION;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    UART_1_Start();

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_PutString("INIT\r\n");

    for(;;)
    {
        size_t cm =  get_hc_sr04() / 58;
        char str [20];
        snprintf(str, sizeof(str), "Distance: %icm\r\n", cm);
        
        UART_1_PutString(str);
        
        //ultimate Baustelle
        
        
        
        
    }
}

/* [] END OF FILE */
