/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    UART_Start();      //Start the UART
    UART_PutString("Send:\n - 'B' or 'b' to start the device\n - 'S' or 's' to stop the device \r\n");
   
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Time
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (ReceivedByte != 0)
            {
                Communication_PIN_Write(1);
                CyDelay(100);
                Communication_PIN_Write(0);
            }   
        if (FlagAcquiredData)
        {
            switch (channel)
            {
                case POTENTIOMETER:
                                   if (state == ON_OVERTHRE)
                                    {
                                        
                                    
                                    }
                                    break;
                case PHOTOR:
                            if (data_mV >= THRESHOLD_mV)
                            {
                                state = ON_OVERTHRE;
                                LED_PWM_Start();

                            
                            }
                            else
                            {
                                state = ON_SUBTHRE;
                            
                            }
                            break;

                                    
            
            
            }
        
        
        }
    }
}

/* [] END OF FILE */
