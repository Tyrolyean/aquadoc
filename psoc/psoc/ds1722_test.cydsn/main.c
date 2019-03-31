
#include "project.h"

#include <stdint.h>
#include <stdio.h>

int main(){
    
    SPI_Start();
    UART_Start();
	CyGlobalIntEnable;
    
    UART_PutString("INIT\r\n");
	
	for(;;){
	    SPI_WriteTxData(00);
        SPI_WriteTxData(00);
        while(SPI_GetRxBufferSize() == 0){}
        uint8_t recv1 = SPI_ReadRxData();
	    while(SPI_GetRxBufferSize() == 0){}
	    uint8_t recv2 = SPI_ReadRxData();
        
        char buf[100];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "Recv1: %X\r\n", recv1);
        UART_PutString(buf);
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "Recv2: %X\r\n", recv2);
        UART_PutString(buf);
        
	}

}
