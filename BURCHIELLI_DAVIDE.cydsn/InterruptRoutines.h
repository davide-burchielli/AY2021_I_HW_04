/* =================================================
    author: Davide Burchielli

    This file declares the functions and the ISRs.
 * =================================================
*/

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    
    #define POTENTIOMETER 0
    #define PHOTOR 1
    
    #define TRANSMIT_BUFFER_SIZE 16
    
    #include "project.h"
    
    typedef enum {
                    OFF_SUBTHRE,   // 0
                    ON_SUBTHRE,    // 1
                    OFF_OVERTHRE,  // 2
                    ON_OVERTHRE,   // 3
        
                  } StateEnum;    
    
    CY_ISR_PROTO (Custom_TIMER_ADC_ISR);
    CY_ISR_PROTO (Custom_UART_RX_ISR);
    
    void Start_HW_Components();
    void Stop_HW_Components();
    void SwitchChannel();
    int32 AcquireData( );
    
    
    uint8_t ReceivedByte;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
    volatile StateEnum state;
    volatile _Bool channel;
    volatile _Bool FlagDataAcquired;
    volatile _Bool FlagBlink;

    int32 PotentValue, PhotoResValue;
    
    char DataBuffer[TRANSMIT_BUFFER_SIZE];
    
#endif
/* [] END OF FILE */
