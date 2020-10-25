/* ======================================================================================================
    author: Davide Burchielli

    This file defines the functions and the ISRs.
    Custom_UART_ISR is executed everytime the isr_UART is triggered by the arrival of a byte. 
    Custom_TIMER_ISR is executed everytime the isr_TIMER is triggered by a TC event, so every 5 seconds.  
 * ======================================================================================================
*/

#include "project.h"
#include "InterruptRoutines.h"

#define PERIOD 255
#define VOLTAGE_mV 5000

int32 data_digit;
int32 data_mV;


void Start_HW_Components()
{
    AMUX_Select(channel);
    AMUX_Start();
    ADC_DelSig_Start();
    Timer_ADC_Start();
}

void Stop_HW_Components()
{
    AMUX_Stop();
    Timer_ADC_Stop();
    ADC_DelSig_Stop();
    LED_PWM_Stop();
}

void SwitchChannel()
{
    ADC_DelSig_StopConvert();
    AMUX_Select(channel);
    ADC_DelSig_StartConvert();   
}

int32 AcquireData( )
{
    data_digit = ADC_DelSig_Read32();
    
    if( data_digit < 0) data_digit = 0;
    else if (data_digit > 65535) data_digit= 65535;
    
    data_mV = ADC_DelSig_CountsTo_mVolts(data_digit);
    return data_mV;
}


CY_ISR(Custom_UART_RX_ISR)
{
     if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            ReceivedByte= UART_ReadRxData();
            FlagBlink = 1;

            switch (ReceivedByte)
            {
                case 'B':
                case 'b':
                        state = ON_SUBTHRE;
                        channel = POTENTIOMETER;
                        Start_HW_Components();
                        break;
                case 'S':
                case 's':
                        state = OFF_SUBTHRE;  
                        Stop_HW_Components();
                        break;
                default: break;
            
            }
            
        }        
}    

CY_ISR(Custom_TIMER_ADC_ISR)
{
    Timer_ADC_ReadStatusRegister();
    
    if (state == ON_OVERTHRE)
    {
        channel = POTENTIOMETER;
        SwitchChannel();    
        PotentValue = (AcquireData() * PERIOD)/VOLTAGE_mV;
    }
    
    channel = PHOTOR;
    SwitchChannel();
    PhotoResValue = AcquireData();

    FlagDataAcquired = 1;
}


/* [] END OF FILE */
