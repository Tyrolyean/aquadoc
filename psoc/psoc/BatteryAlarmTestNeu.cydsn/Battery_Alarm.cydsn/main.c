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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_Start();

    for(;;)
    {
        /* Place your application code here. */
        
        CyDelay(1000);
        //UART_PutString("A\r\n");
        
        if(BatteryAlarm_Read() == 0){
        
            //Does what needs to be done
            UART_PutString("Da Akku is laar, du HUND!\r\n");
            
        }
            
        else if(BatteryAlarm_Read() == 1){
            UART_PutString("Reaktor 4 up and running, blyat!\r\n");
        }
        
        
        
        
    }
}

/* [] END OF FILE */
