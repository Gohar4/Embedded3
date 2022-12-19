#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "ADC.h"
#include "Uart.h"
#include "PWM.h"
#include "Rtc.h"
#include "I2c.h"
#include "Eeprom.h"  // adding ADC, Rtc, I2c, Eeprom, PWM, UART, LCD, and delay libraries 

#define led 2 //led variable set as pin 3 PORT D

void init();

int main(void){

  init(); 
  
  int tempmax = EEP_Read(0); // temperature max set in Eeprom memory in channel 0
  int tempmin = EEP_Read(1); // temperature min set in Eeprom memory in channel 1

  int duty = 0; // initial duty 0

  char buffer[20];
	sprintf(buffer, "EEP Addr:%d TempMax:%02d \n", 0, tempmax);
	Uart_SendString(buffer); // saves tempmax at memory address and sends to Uart

  sprintf(buffer, "EEP Addr:%d TempMin:%02d \n", 1, tempmin);
	Uart_SendString(buffer); // saves tempmax at memory address and sends to Uart

  char tempC;
  char day;
  int hour;
  int second;
  int minute;
  //int TimeFormat12;
  char year; 
  char month;
  char date;
  
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};


//while loop start
  while (1)
  {
    int  button = ADC_Read(0); // reads analogue input at channel 0 (button pressed on LCD keypad)
    float temp = ADC_Read(1); // reads analogue input at channel 1 (temperature sensor)
    float tempC = temp/4.8; // divides temperature to become more realistic as sensor is not accurate

    LCD_Clear(); //clears LCD
  
    char temp_string[5];
    itoa(tempC, temp_string, 10);
    //converts int data to string data

    LCD_String_xy (0, 0, "Temp:"); // prints "Temperature:" at row 0 and column 0
    LCD_String_xy (0, 5, temp_string); // prints temperature at row 0 and column 12

   char temp_max[5];
    itoa(tempmax, temp_max, 10);

    LCD_String_xy (1, 0, "Max:"); // prints "Max:" at row 1 and column 0
    LCD_String_xy (1, 4, temp_max); // prints temperature max at row 1 and column 4

     char temp_min[5];
    itoa(tempmin, temp_min, 10);

    LCD_String_xy (1, 8, "Min:"); // prints "Min:" at row 1 and column 8
    LCD_String_xy (1, 12, temp_min); // prints temperature min at row 1 and column 12
    
    _delay_ms(500); 


    if (button < 60){ 
      tempmin ++;
          //right button adds one to temperature min
        EEP_Write(1, tempmin);

        sprintf(buffer, "EEP Addr:%d TempMin:%02d \n", 1, tempmin);
	      Uart_SendString(buffer);
        //LCD_String_xy (0, 1, tempmin); 
        //print new min value (+1) in LCD and saves in Eeprom
      }
    else if (button  < 200) {
        //up  button adds one to temperature max
        tempmax ++;
        EEP_Write(0, tempmax);

        sprintf(buffer, "EEP Addr:%d TempMax:%02d \n", 0, tempmax);
	      Uart_SendString(buffer);
        // LCD_String_xy (0, 1, tempmax); //print new max value (+1)
      }
    else if (button  < 400){
        tempmax --;  //down button decreases one from temperature max
         //LCD_String_xy (0, 1, tempmax); 
         //print new max value (-1)
         EEP_Write(0, tempmax );

        sprintf(buffer, "EEP Addr:%d TempMax:%02d \n", 0, tempmax);
	      Uart_SendString(buffer);
      }  

     else if (button  < 600){
        tempmin--;
        EEP_Write(1, tempmin);

        sprintf(buffer, "EEP Addr:%d TempMin:%02d \n", 1, tempmin);
	      Uart_SendString(buffer);  //left button decreases one from temperature min
         //LCD_String_xy (0, 1, tempmin); //print new min value (-1)
        }

    if (tempC < tempmin){ //if statement for tempmin value is exceeded
      PORTD = PORTD | (1 << led); // led on
      
    }
    else { //temrature value stays between limits
        PORTD = PORTD & ~ (1 << led); // led off 
    }

     if(tempC > tempmax)   //if statement for tempmax value is exceeded
    {
      duty = duty + 2; //counter for duty cycle
    }  
    else { //temrature value stays between limits
      duty = 0;
    }
  Set_DutyCycle(duty); //duty cycle runs with new value
    }
    
  return 0; 
}
//while loop end

void init(){
  
  I2C_Init();		// Initialize I2C 

	Uart_Init();	// Initialize Uart 
	
  ADC_Init();   // initilize ADC

  PWM_init();  // initilize PWM

  LCD_Init (); // initilize LCD

  DDRD = DDRD | (1 << led); // led pin set as output 
}