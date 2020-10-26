/* ======================================================================================================
    author: Davide Burchielli

    This file defines the functions and the ISRs.
    Custom_UART_RX_ISR is executed everytime the isr_UART_RX is triggered by the arrival of a byte. 
    Custom_TIMER_ADC_ISR is executed everytime the isr_TIMER_ADC is triggered by a TC event, so every 5 seconds.  
 * ======================================================================================================
*/

#include "project.h"
#include "InterruptRoutines.h"


int32 read_data;  // read_data variable contains the value read from the ADC

void Start_HW_Components()   // This function starts the hardware components: MUX, ADC and Timer
{  
    AMUX_Start();            // Start the MUX
    AMUX_FastSelect(PHOTOR); // Disconnect the last connection and connect the PHOTOR channel
    ADC_DelSig_Start();      // Start the ADC
    Timer_ADC_Start();       // Start the Timer
}

void Stop_HW_Components()   // This function stops the hardware components: MUX, ADC and Timer
{
    Timer_ADC_Stop();       // Stop the Timer
    AMUX_DisconnectAll();   // Disconnect all the channel of the MUX
    ADC_DelSig_Stop();      // Stop the ADC
    LED_PWM_Stop();         // Stop the PWM associated to the Lamp_LED, therefore switching it off.
}

int32 AcquireData( )       // This function reads and returns the ADC result
{
    read_data = ADC_DelSig_Read32();  // Save the ADC result in read_data variable

    // To be sure that read_data is in the range [0;65535]:
    if( read_data < 0) read_data = 0;
    else if (read_data > 65535) read_data= 65535;

    return read_data;  // Return the ADC result
}

void SwitchChannel()  // This function switches channel of the MUX
{
    // NOTE: since the ADC_DelSig Convertion Mode is set to 'Multi-sample', before switching the MUX channel
    // it is necessary to stop the ADC convertion, otherwise the results could be contaminated.
    
    ADC_DelSig_StopConvert();  // Stop the ADC convertion
    AMUX_FastSelect(Channel);  // Change the channel of the MUX (note: it is used FastSelect API since MUX AtMostOneActive is active )
    ADC_DelSig_StartConvert(); // Start the ADC convertion
}

// Define Custom_UART_RX_ISR
CY_ISR(Custom_UART_RX_ISR)
{
  ReceivedChar = UART_GetChar();  // Save in ReceivedChar the last received byte of data.
      switch (ReceivedChar)
      {
        case 'B':
        case 'b':
                Start_HW_Components();  // Starts the Hardware components; the sampling of the photoresistor signal starts.
                Communication_LED_PIN_Write(1);  // Turn on the Communication_LED to warn that the Psoc is sending data.
                break;
        case 'S':
        case 's':
                Stop_HW_Components();  // Stops the Hardware components; the ADC stops to sample and convert and the Lamp_LED is switched off
                Communication_LED_PIN_Write(0);  // Turn off the Communication_LED.
                break;
        default: break;           
      }     
}    

// Define Custom_TIMER_ADC_ISR
CY_ISR(Custom_TIMER_ADC_ISR)
{
    Timer_ADC_ReadStatusRegister();  // Read Timer status register to bring interrupt line low
    if (!FlagAcquireData)  // To be sure that the last value has been read (and so FlagAcquireData has been reset to 0 by the main )
        FlagAcquireData = 1; 
}

/* [] END OF FILE */
