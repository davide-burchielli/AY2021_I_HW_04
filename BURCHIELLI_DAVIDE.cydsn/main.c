/* ========================================

 * ========================================
*/
#include "project.h"
#include "InterruptRoutines.h"
#include "stdio.h"

#define PERIOD 255
#define VOLTAGE_mV 5000
#define NUMB_CAL 10
#define ADD_THRESH_MEAN 100

int32 CompareValue = 0;
int32 accum = 0;
int32 cont = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    ReceivedChar = 0;  
    Channel = PHOTOR;
    FlagAcquireData = 0;
    PotentValue = 0;
    PhotoResValue = 0;

    UART_Start();   //Start the UART
    //UART_PutString("Send:\n - 'B' or 'b' to start the device\n - 'S' or 's' to stop the device \r\n");
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0;
    
    isr_TIMER_ADC_StartEx(Custom_TIMER_ADC_ISR); //Start the ISR of the Timer
    isr_UART_RX_StartEx(Custom_UART_RX_ISR);   //Start the ISR of the UART
    
    for(;;)
    {
        if (FlagCalibration)
            {  
              accum = 0;
              Threshold = 0;
              cont = 0;
              for ( cont=0; cont < NUMB_CAL ; cont ++)
                {       
                        while (!FlagAcquireData);
                        PhotoResValue = AcquireData();
                        accum += PhotoResValue;
                        FlagAcquireData = 0;
                }
              Threshold = ( accum / NUMB_CAL ) + ADD_THRESH_MEAN;
              FlagCalibration = 0;
            }

        if (FlagAcquireData)
        {
            Channel = PHOTOR;
            SwitchChannel();
            PhotoResValue = AcquireData();
            if (PhotoResValue >= Threshold)
            {
                Channel = POTENTIOMETER;
                SwitchChannel();    
                PotentValue = AcquireData();
                CompareValue = (PotentValue * PERIOD)/VOLTAGE_mV; 
                LED_PWM_WriteCompare(CompareValue);     
                LED_PWM_Start();                  
            }                
            else
                LED_PWM_Stop();
                
            FlagAcquireData = 0 ;
           
            Communication_LED_PIN_Write(1);
            DataBuffer[1] = PhotoResValue >> 8 ;
            DataBuffer[2] = PhotoResValue & 0xFF ;
            DataBuffer[3] = PotentValue >> 8 ;
            DataBuffer[4] = PotentValue & 0xFF ;
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);  
            Communication_LED_PIN_Write(0);

        }       
    }
}

/* [] END OF FILE */
