
#include "hcsr04.h"
#include "project.h"

uint32_t get_hc_sr04(){
    
    /* If the timeout is reached, the sensor is 
     * assumed to have been disconnected
     */
    
    
    /* We assumed that a linear function correcting the
     * measurements would be enough. We took measurements at 1m and 2m and got as
     * results the values .36m and .72m respectively. We then perceeded to
     * put a linear function through the points (.36, 100) and (.72, 100)
     * which resulted in a slope of 2.77777 or 25/9 and an offset of 0.
     * Therefore we assumed an only linear distortion. We took further measurements
     * and got errors in the range of 1-3% of the real distances, which was deemed enough.
     */
    #define CORRECTION_FACTOR ((double)25.0/9.0)
    
    #define TIMEOUT 50000
    
    /* Avoid further distortion through triggered interrupts. */
    CyGlobalIntDisable;
    
    /* Place your application code here. */
    Trigger_Impuls_Write(1);
    CyDelayUs(10);
    Trigger_Impuls_Write(0);
    
    
    for(int i = 0; i < TIMEOUT && !Echo_Input_Read(); i++){
        CyDelayUs(1);
    }
    
    uint32_t counter;
    
    for(counter = 0;Echo_Input_Read() == 1 && counter <= TIMEOUT; counter ++){
        CyDelayUs(1);
    }
    CyGlobalIntEnable;
    
    /* This statement performes an integer to float converstion and an
     * float to integer conversion. The calculation itself happens as
     * (double precision) float.
     */
    return (counter * CORRECTION_FACTOR);
}

uint32_t timer_us_hcsr04 = 0;

int init_hcsr04(){
    
    return 0;
}