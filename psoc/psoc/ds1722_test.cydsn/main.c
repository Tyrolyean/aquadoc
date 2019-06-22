
#include "project.h"

#include <stdint.h>
#include <stdio.h>

int main(){
    
    SPI_Start();
    UART_Start();
    CyGlobalIntEnable;
    
    /* Write initial configuration into specified registers */
    SPI_WriteTxData(0x80);
    SPI_WriteTxData(0xE9);
    CyDelay(10);
   
    /* Empty the RX buffer  */
    while(SPI_GetRxBufferSize() == 0){}
    SPI_ReadRxData();
    while(SPI_GetRxBufferSize() == 0){}
    SPI_ReadRxData();
   
    UART_PutString("INIT\r\n");
	
    for(;;){
	/* Fill the SPI TX buffer in order to read the first 3 bytes from the
	 * first 3 registers
	 */
	SPI_WriteTxData(00);
        SPI_WriteTxData(00);
        SPI_WriteTxData(00);
        SPI_WriteTxData(00);
        while(SPI_GetRxBufferSize() == 0){/* Wait for rx buffer to be ready */}
	uint8_t recv1 = SPI_ReadRxData();
	
	while(SPI_GetRxBufferSize() == 0){ /* Wait for rx buffer to be ready */}
	uint8_t recv2 = SPI_ReadRxData();
        
	/* Print out received data to user */
        char buf[100];
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "Recv1: %X\r\n", recv1);
        UART_PutString(buf);
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "Recv2: %X\r\n", recv2);
        UART_PutString(buf);
        
    }

}
