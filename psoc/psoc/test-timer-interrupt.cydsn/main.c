#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "project.h"
#include "isrs.h"

int main(void){

	isr_timer();
	Timer_1_Start();
	ISR_1m_Start();
	
	CyGlobalIntEnable;
    
	for(;;){
        
		Timer_1_ReadStatusRegister();
		/* Check if flag is set, change leds accordingly */
		if(pin_val){
			LED_OUT_Write(1);
			CyDelay(1000);

			LED_OUT_Write(0);
			pin_val = false;
		}
	}

}
