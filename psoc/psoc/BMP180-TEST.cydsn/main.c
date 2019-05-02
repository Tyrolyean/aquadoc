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

#include <stdint.h>
#include <stdio.h>

#include "project.h"

uint16_t get_temperature(){
    static char buf [100];
    
    UART_PutString("Start of conversion\r\n");
    uint8_t read = I2C_MasterSendStart(0x77, 0);

    if(read != I2C_MSTR_NO_ERROR){
        I2C_MasterSendStop();
        UART_PutString("Failed to address SLAVE\r\n");
        return 0;
    }else{
        
        read = I2C_MasterWriteByte(0xD0);
        if(read != I2C_MSTR_NO_ERROR){
            I2C_MasterSendStop();
            snprintf(buf, sizeof(buf), "FAIL RECV: %u \r\n", read);
            UART_PutString(buf);
            return 0;
        }
        read = I2C_MasterSendRestart(0x77, 1);
        if(read != I2C_MSTR_NO_ERROR){
            I2C_MasterSendStop();
            snprintf(buf, sizeof(buf), "FAIL RECV rest: %u \r\n", read);
            UART_PutString(buf);
            return 0;
        }
        uint8_t data = I2C_MasterReadByte(I2C_ACK_DATA);
        I2C_MasterSendStop();
        if(read == I2C_MSTR_NO_ERROR){
            UART_PutString("Successfully addressed SLAVE\r\n");
        }else{
            UART_PutString("NOT sucessfully addressed SLAVE\r\n");
        }
        
        snprintf(buf, sizeof(buf), "RECV: %u \r\n", data);
        UART_PutString(buf);
    }
     return 0;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    I2C_Start();
    UART_Start();
    UART_PutString("INIT\r\n");
    
    
    for(;;){
        get_temperature();
    }
}

/* [] END OF FILE */
