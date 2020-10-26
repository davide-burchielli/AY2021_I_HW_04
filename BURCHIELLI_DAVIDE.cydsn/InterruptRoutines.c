/* ======================================================================================================
    author: Davide Burchielli

    This file defines the functions and the ISRs.
    Custom_UART_ISR is executed everytime the isr_UART is triggered by the arrival of a byte. 
    Custom_TIMER_ISR is executed everytime the isr_TIMER is triggered by a TC event, so every 5 seconds.  
 * ======================================================================================================
*/

#include "project.h"
#include "InterruptRoutines.h"

int32 data_digit;
int32 data_mV;

void Start_HW_Components()
{  
    AMUX_Start();
    AMUX_FastSelect(PHOTOR);
    ADC_DelSig_Start();
    Timer_ADC_Start();
}

void Stop_HW_Components()
{
    Timer_ADC_Stop();
    AMUX_DisconnectAll();
    ADC_DelSig_Stop();
    LED_PWM_Stop();
}

int32 AcquireData( )
{
    data_digit = ADC_DelSig_Read32();

    if( data_digit < 0) data_digit = 0;
    else if (data_digit > 65535) data_digit= 65535;

    return data_digit;
}

void SwitchChannel()
{
    ADC_DelSig_StopConvert();
    AMUX_FastSelect(Channel);
    ADC_DelSig_StartConvert();  
}

CY_ISR(Custom_UART_RX_ISR)
{
  ReceivedChar = UART_GetChar();
      switch (ReceivedChar)
      {
        case 'B':
        case 'b':
                Start_HW_Components();  
                Communication_LED_PIN_Write(1);
                break;
        case 'S':
        case 's':
                Stop_HW_Components();
                Communication_LED_PIN_Write(0);
                break;
        default: break;           
      }     
}    

CY_ISR(Custom_TIMER_ADC_ISR)
{
    Timer_ADC_ReadStatusRegister();
    if (!FlagAcquireData)
        FlagAcquireData = 1;
}

/* [] END OF FILE */
