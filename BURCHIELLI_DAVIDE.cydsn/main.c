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

#define THRESHOLD_mV 2.5


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ReceivedByte = 0;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
    state = OFF_SUBTHRE;
    channel = POTENTIOMETER;
    FlagDataAcquired = 0;
    FlagBlink = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();      //Start the UART
    UART_PutString("Send:\n - 'B' or 'b' to start the device\n - 'S' or 's' to stop the device \r\n");
   
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Time
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagBlink)
            {
                Communication_PIN_Write(1);
                CyDelay(1000);
                Communication_PIN_Write(0);
                FlagBlink = 0 ;
            }   
            
        if (FlagDataAcquired)
        {
            if(channel == PHOTOR)
            {
                 if (PhotoResValue >= THRESHOLD_mV)
                  {
                    state = ON_OVERTHRE;
                    LED_PWM_WriteCompare(PotentValue);     
                    LED_PWM_Start();
                  }
                  else
                  {
                    state = ON_SUBTHRE;
                    LED_PWM_Stop();
                  }          
            }
            FlagDataAcquired = 0 ;
        }
    }
}

/* [] END OF FILE */
