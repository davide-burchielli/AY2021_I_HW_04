/* ========================================

 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

#define PERIOD 255
#define MAX_VALUE 65535
#define THRESHOLD MAX_VALUE * 3/4

int32 CompareValue = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ReceivedChar = 0;  
    Channel = PHOTOR;
    FlagAcquireData = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();   //Start the UART
    
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Timer
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagAcquireData)
        {
            Channel = PHOTOR;
            SwitchChannel();
            PhotoResValue = AcquireData();

            if (PhotoResValue <= THRESHOLD)
            {
                
                Channel = POTENTIOMETER;
                SwitchChannel();    
                PotentValue = AcquireData();
                CompareValue = (PotentValue * PERIOD)/MAX_VALUE; 

                LED_PWM_WriteCompare(CompareValue);     
                LED_PWM_Start();                  
            }                
            else
                LED_PWM_Stop();
                
            FlagAcquireData = 0 ;
                      
            DataBuffer[1] = PhotoResValue >> 8 ;
            DataBuffer[2] = PhotoResValue & 0xFF ;
            DataBuffer[3] = PotentValue >> 8 ;
            DataBuffer[4] = PotentValue & 0xFF ;
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);  

        }       
    }
}

/* [] END OF FILE */
