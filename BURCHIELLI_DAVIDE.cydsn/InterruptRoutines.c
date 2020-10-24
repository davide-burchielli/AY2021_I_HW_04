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

CY_ISR(Custom_UART_RX_ISR)
{
     if (UART_ReadRxStatus() == UART_RX_STS_FIFO_NOTEMPTY)
        {
            
            ReceivedByte= UART_ReadRxData();
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
            ReceivedByte = 0;
        }        
}    

CY_ISR(Custom_TIMER_ADC_ISR)
{
    Timer_ADC_ReadStatusRegister();
    data_digit = ADC_DelSig_Read32();
    
    if( data_digit < 0) data_digit = 0;
    else if (data_digit > 65535) data_digit= 65535;
    data_mV = ADC_DelSig_CountsTo_mVolts(data_digit);
    FlagAcquiredData = 1;
    
}

void Start_HW_Components()
{
    AMUX_Select(POTENTIOMETER);
    AMUX_Start();
    ADC_DelSig_Start();
    Timer_ADC_Start();
}

void Stop_HW_Components()
{
    AMUX_Stop();
    ADC_DelSig_Stop();
    Timer_ADC_Stop();
}





/* [] END OF FILE */
