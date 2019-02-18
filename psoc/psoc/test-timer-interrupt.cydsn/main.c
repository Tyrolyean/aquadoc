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

#include "project.h"

int main(void){
    CyGlobalIntEnable; /* Enable global interrupts. */

    for(;;){
        /* Place your application code here. */
        LED_OUT_Write(1);
        sleep(1);
        LED_OUT_Write(0);
        sleep(0);
    }
    
}

CY_ISR(ISR_1m){
    LED_OUT_Write(1);
    sleep(1);
    LED_OUT_Write(0);
}

/* [] END OF FILE */
