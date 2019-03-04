
#include "hcsr04.h"
#include "project.h"

uint32_t get_hc_sr04(){
    
    /* If the timeout is reached, the sensor is 
     * assumed to have been disconnected
     */
    
    #define ERRROR_CORRECTION 0
    #define CORRECTION_FACTOR ((double)2.7)
    
    #define TIMEOUT 50000
    CyGlobalIntDisable;
    
    /* Place your application code here. */
    Trigger_Impuls_Write(1);
    CyDelayUs(10);
    Trigger_Impuls_Write(0);
    
    
    for(int i = 0; i < TIMEOUT && !Echo_Input_Read(); i++){
        CyDelayUs(1);
    }
    
    //ultimate Baustelle
    uint32_t counter;
    
    for(counter = 0;Echo_Input_Read() == 1 && counter <= TIMEOUT; counter ++){
        CyDelayUs(1);
    }
    CyGlobalIntEnable;
    return (counter * CORRECTION_FACTOR) + ERRROR_CORRECTION;
}

uint32_t timer_us_hcsr04 = 0;

int init_hcsr04(){
    
    return 0;
}