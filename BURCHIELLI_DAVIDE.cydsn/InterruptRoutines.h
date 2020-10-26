/* ===============================================================
    author: Davide Burchielli

    This file declares the functions, the ISRs and the variables.
 * ===============================================================
*/

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    
    // Define the 2 possible value of the MUX channel
    #define POTENTIOMETER 0  // The MUX channel 0 is used to acquire the potentiometer signal
    #define PHOTOR 1         // The MUX channel 1 is used to acquire the potentiometer signal
    
    #define BYTE_TO_SEND 4
    #define TRANSMIT_BUFFER_SIZE 1 + BYTE_TO_SEND + 1
    
    #include "project.h"

    CY_ISR_PROTO (Custom_TIMER_ADC_ISR);
    CY_ISR_PROTO (Custom_UART_RX_ISR);
    
    void Start_HW_Components();
    void Stop_HW_Components();
    void SwitchChannel();
    int32 AcquireData( );
       
    uint8_t ReceivedChar;  // Define and initialize the variable ReceivedChar on which it is saved the byte recived
    volatile _Bool Channel;
    volatile _Bool FlagAcquireData;

    int32 PotentValue, PhotoResValue;
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];
    int32 Threshold;
    
    
#endif
/* [] END OF FILE */
