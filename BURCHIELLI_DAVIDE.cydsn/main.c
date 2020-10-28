/* =====================================================================================================================
    author: Davide Burchielli

    This file manages the acquisition of the photoresistor and the potentiometer signals only when the variable
    FLagAcquireData is set = 1 by the calling of Custom_TIMER_ADC_ISR, which occurs every 100 ms (frequency of
    10 Hz). In particular, the potentiometer signal is acquired only when the photoresistor signal (the brigthness)
    is < or = than a specified threshold (see the macro THRESHOLD). Based on this potentiometer signal, the LED 
    brightness intensity is driven through the PWM. 
    
    Finally, the photoresistor and the potentiometer signals, saved in DataBuffer variable, are sent to the 
    serial port, in a packet structure composed by:
    
                                        HEADER : 0XA0
                                        Photoresistor data:  2 bytes
                                        Potentiometer data : 2 bytes
                                        TAIL : 0xC0
    
    NOTE: When the system is overthreshold, the potentiometer bytes sent rappresent the last value sampled 
    by the potentiometer when the system was underthreshold or , if the system has never been under threshold,
    it is sent 0. 
 * =====================================================================================================================
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

#define PERIOD 255                 // Macro that rappresents the maximum value for the PWM Period
#define MAX_VALUE 65535            // Macro that rappresents the maximum value of the potentiometer signal (since 16 bit)
#define THRESHOLD MAX_VALUE * 3/4  // Macro that rappresents the Threshold under which the Lamp_LED is switched on
                                   // NOTE: the threshold is set 3/4 the maximum value of the photoresistor signal (which is 65535, 16 bit)
                                   // since sperimentally it seems a reasonable brightness value.

int32 CompareValue = 0;  // CompareValue variable contains the Compare Value of the PWM obtained form the potentiometer signal 

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Initialize all the variables:
    ReceivedChar = 0;  
    FlagAcquireData = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();   //Start the UART
    
    DataBuffer[0] = 0xA0;  // Write the HEADER byte as the first array element
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; // Write the TAIL byte as the last array element
    
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Timer
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagAcquireData) // If the Custom_TIMER_ADC_ISR is called FlagAcquireData = 1 
        {
            Channel = PHOTOR;  
            SwitchChannel();                 // Switch to PHOTOR MUX channel to sample the photoresistor signal  
            PhotoResValue = AcquireData();   // Save in PhotoResValue the returned value of the photoresistor

            if (PhotoResValue <= THRESHOLD)  // If the read brightness is <= than the threshold:
            {
                
                Channel = POTENTIOMETER;
                SwitchChannel();              // Switch to POTENTIOMETER MUX channel to sample the potentiometer signal
                PotentValue = AcquireData();  // Save in PotentValue the returned value of the potentiometer
                CompareValue = (PotentValue * PERIOD)/MAX_VALUE;  // Map PotentValue into the range [0;PERIOD] 

                LED_PWM_WriteCompare(CompareValue);   // Update the PWM Compare Value.  
                LED_PWM_Start();   // Start the PWM                 
            }                
            else
                LED_PWM_Stop();    // Stop the PWM, therefore switching off Lamp_LED.
                
               
            DataBuffer[1] = PhotoResValue >> 8 ;    // Save the MSB of PhotoResValue as the 2° element of DataBuffer array
            DataBuffer[2] = PhotoResValue & 0xFF ;  // Save the LSB of PhotoResValue as the 3° element of DataBuffer array
            DataBuffer[3] = PotentValue >> 8 ;      // Save the MSB of PotentValue as the 4° element of DataBuffer array
            DataBuffer[4] = PotentValue & 0xFF ;    // Save the LSB of PotentValue as the 5° element of DataBuffer array
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);  // Transmit DataBuffer

            FlagAcquireData = 0 ;  // Reset FlagAcquireData 
        }       
    }
}

/* [] END OF FILE */
