/* ===============================================================
    author: Davide Burchielli

    This file declares the functions, the ISRs and the variables.
 * ===============================================================
*/

#ifndef _INTERRUPT_ROUTINES_H
    #define _INTERRUPT_ROUTINES_H
    
    #include "project.h"
    
    // Create macro for the 2 MUX channel
    #define POTENTIOMETER 0  // The MUX channel 0 is used to acquire the potentiometer signal
    #define PHOTOR 1         // The MUX channel 1 is used to acquire the Photoresistor signal
    
    #define BYTE_TO_SEND 4   // Macro that identifies the number of bytes to be sent 
    #define TRANSMIT_BUFFER_SIZE 1 + BYTE_TO_SEND + 1  // Macro that identifies the number of bytes to be sent including the HEADER and TAIL byte.
                                                       // It identifies the dimention of the array DataBuffer

    // Declare the 2 ISR
    CY_ISR_PROTO (Custom_TIMER_ADC_ISR);
    CY_ISR_PROTO (Custom_UART_RX_ISR);
    
    // Declare the functions
    void Start_HW_Components();
    void Stop_HW_Components();
    void SwitchChannel();
    int32 AcquireData( );
       
    // Declare the variables 
    uint8_t ReceivedChar;  // ReceivedChar variable contains the received byte of data
    volatile _Bool Channel;  // Channel variable identidies the current channel of the MUX
    volatile _Bool FlagAcquireData;  // FlagAcquireData is used to warn the main that a sample must be read by the ADC
    int32 PotentValue;    // PotentValue variable contains the last value read by the ADC from the channel POTENTIOMETER of the MUX
    int32 PhotoResValue;  // PhotoResValue variable contains the last value read by the ADC from the channel PHOTOR of the MUX
    uint8_t DataBuffer[TRANSMIT_BUFFER_SIZE];  // DataBuffer array contains the bytes to be sent including the HEADER and TAIL byte
 
#endif
/* [] END OF FILE */
