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

#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "project.h"
#include "isrs.h"

int main(void){
    isr_timer();
    CyGlobalIntEnable; /* Enable global interrupts. */
    Timer_1_Start();
    ISR_1m_Start();
    
    for(;;){
        /* Place your application code here. */
        Timer_1_ReadStatusRegister();
        LED_OUT_Write(pin_val);
        if(pin_val){
            CyDelay(1000);
            pin_val = false;
        }
    }
    
}



/* [] END OF FILE */
