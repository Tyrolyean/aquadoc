/* BIM-2 Testprogramm */

#include "project.h"

int main(){
	
	/* Hardware initialisation */
	UART_Start();

	/* Set the ARM's global interupt enable flag */
	CyIntEnable;

	UART_PutString("INIT\r\n");

	for(;;){
		/* Print output to the bim module */
		UART_PutString("UU");
		CyDelay(100);
	}
		

}
