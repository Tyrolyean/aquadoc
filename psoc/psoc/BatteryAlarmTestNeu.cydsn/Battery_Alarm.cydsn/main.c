#include "project.h"

int main(void){

	/* Hardware initialisation routines */ 
	UART_Start();
	CyGlobalIntEnable;

	for(;;){
        
		CyDelay(1000);
		
		/* If the digital input is beneath the threshold send out an
		 * alarm, if not, notify otherwise */
		if(BatteryAlarm_Read() == 0){
        
			//Does what needs to be done
			UART_PutString("Reactor 4 is reachig critical "
				"temperature, cyka!\r\n");
            
		} else if(BatteryAlarm_Read() == 1){
			UART_PutString("Reactor 4 up and running, blyat!\r\n");
		}
	}
}
