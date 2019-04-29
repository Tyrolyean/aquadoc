/* ========================================
 *
 * Copyright Manuel Eiter, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/
#include <project.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "OneWire.h"

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

	int preValue=-1;
	int currentValue;
	int i=0;
    char *end;
	uint8 crc=0xff;
	unsigned char buf[8]={0,};    
    char sBuffer[8];
    int t;
    float temp;
    int zw[2];
	
    CyGlobalIntEnable; 

	//LCD_Start();
	//LCD_ClearDisplay();
	UART_temp_Start();
	CyDelay(1000);
	
	//LCD_ClearDisplay();
	//LCD_PrintString("Reset:");
	

	UART_temp_PutString("###TEST TEST TEST###");
    
    for(;;)
    {
        currentValue = OW_Detect(Pin_Sensor_0);	// check for presence of a 1-Wire device
        OW_WriteByte(0xCC);

        OW_WriteByte(0x44);
        CyDelayUs(220);
        OW_Detect(Pin_Sensor_0);
        OW_WriteByte(0xCC);
        OW_WriteByte(0xBE);
        for(i=0; i <=2; i++){
           zw[i] = OW_ReadByte();
        }
        OW_Detect(Pin_Sensor_0);
        
        t = (zw[1] << 8) + (zw[0]>>1);    // shift um 1 da mal 2 - siehe DB Seite 6

        
        sprintf(sBuffer, "%d.%d C\n\r", t, (zw[0]&0x01) ? 5:0);
        UART_temp_PutString(sBuffer);
        CyDelay(1000);
    }
}

/* [] END OF FILE */
