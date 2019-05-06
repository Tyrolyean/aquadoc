#include "project.h"
#include <string.h>
#include <stdio.h>

int main(void)
{
    /* Initialize Hardware */
    water_counter_Start();
    UART_Start();
    UART_PutString("INIT\r\n");
    
    /* Aftter initialisation enable global interrupts */
    CyGlobalIntEnable;
    
    for(;;){
        CyDelay(1);
        char buffer[30];
        memset(buffer, 0, sizeof(buffer));
        snprintf(buffer, sizeof(buffer), "Stand: %lu0L\r\n", water_counter_ReadCounter());
        UART_PutString(buffer);
    }
}