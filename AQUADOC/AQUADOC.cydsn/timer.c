//#######################################################################################################################
//  project:    AQUADOC
//  file:       timer.c
//  author:     Johannes Jaeger
//  date:       25/03/2019
//-----------------------------------------------------------------------------------------------------------------------
//  uart_routines for system timer
//############## definitions ############################################################################################
//  header for system timer
//####### definitions ###################################################################################################
#ifndef _TIMER_H
    #define _TIMER_H
    //---------- includes -----------------------------------------------------------------------------------------------
    #include "public.h"
    //---------- handler prototypes -------------------------------------------------------------------------------------
    CY_ISR_PROTO ( sys_isr_handler);
    #else
        #warning: timer.h already included
#endif
/* [] END OF FILE */