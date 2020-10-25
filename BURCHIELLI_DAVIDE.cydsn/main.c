/* ========================================

 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

#define THRESHOLD_mV 4400
#define PERIOD 255
#define VOLTAGE_mV 5000


int32 CompareValue = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ReceivedByte = 0;  // Define and initialize the variable ReceivedByte on which it is saved the byte recived
    channel = PHOTOR;
    FlagAcquireData = 0;
    FlagBlink = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();      //Start the UART
    //UART_PutString("Send:\n - 'B' or 'b' to start the device\n - 'S' or 's' to stop the device \r\n");
   
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Time
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagBlink == 1)
            {
                Communication_PIN_Write(1);
                CyDelay(100);
                Communication_PIN_Write(0);
                FlagBlink = 0 ;
            } 

        if (FlagAcquireData)
        {
            channel = PHOTOR;
            SwitchChannel();
            PhotoResValue = AcquireData();
            if (PhotoResValue >= THRESHOLD_mV)
            {
                channel = POTENTIOMETER;
                SwitchChannel();    
                PotentValue = AcquireData();
                CompareValue = (PotentValue * PERIOD)/VOLTAGE_mV; 
                LED_PWM_WriteCompare(CompareValue);     
                LED_PWM_Start();                  
            }                
            else
                LED_PWM_Stop();
            
            DataBuffer[1] = PhotoResValue >> 8 ;
            DataBuffer[2] = PhotoResValue & 0xFF ;
            DataBuffer[3] = PotentValue >> 8 ;
            DataBuffer[4] = PotentValue & 0xFF ;
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);    
                
            FlagAcquireData = 0 ;
        }       
    }
}

/* [] END OF FILE */
