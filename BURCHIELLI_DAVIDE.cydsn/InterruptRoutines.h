/* =================================================
    author: Davide Burchielli

    This file declares the functions and the ISRs.
 * =================================================
*/

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    
    #define ON 1
    #define OFF 0
    #define POTENTIOMETER 0
    #define PHOTOR 1
    #define THRESHOLD_mV 2.5
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
    
    
    uint8_t ReceivedByte = 0;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
    volatile StateEnum state = OFF;
    volatile _Bool channel = POTENTIOMETER;
    volatile _Bool FlagAcquiredData = 0;
    int32 data_mV;
    
    char DataBuffer[TRANSMIT_BUFFER_SIZE];
    
#endif
/* [] END OF FILE */
