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
#include "stdio.h"

#define THRESHOLD_mV 4700
#define PERIOD 255
#define VOLTAGE_mV 5000


int32 CompareValue = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ReceivedByte = 0;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
    state = OFF_SUBTHRE;
    channel = POTENTIOMETER;
    FlagAcquireData = 0;
    FlagBlink = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();      //Start the UART
    UART_PutString("Send:\n - 'B' or 'b' to start the device\n - 'S' or 's' to stop the device \r\n");
   
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Time
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagAcquireData)
        {
            if (state == ON_OVERTHRE)
            {
                channel = POTENTIOMETER;
                SwitchChannel();    
                PotentValue = AcquireData();
                CompareValue = (PotentValue * PERIOD)/VOLTAGE_mV;             
            }
            
            channel = PHOTOR;
            SwitchChannel();
            PhotoResValue = AcquireData();
            sprintf(DataBuffer , "Photoresi: %ld mV\n\n", PhotoResValue);
            
            UART_PutString(DataBuffer);   
            FlagAcquireData = 0 ;
            
                 if (PhotoResValue >= THRESHOLD_mV)
                  {
                    UART_PutString("--OVERTHRESHOLD--\n");
                    state = ON_OVERTHRE;
                    LED_PWM_WriteCompare(CompareValue);     
                    LED_PWM_Start();
                  }
                  else
                  {
                    UART_PutString("--UNDERTHRESHOLD--\n");
                    state = ON_SUBTHRE;
                    LED_PWM_Stop();
                  }          
        }     
        
        if (FlagBlink == 1)
            {
                Communication_PIN_Write(1);
                CyDelay(100);
                Communication_PIN_Write(0);
                FlagBlink = 0 ;
            }          
    }
}


/* [] END OF FILE */
