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

#include "hcsr04.h"

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
