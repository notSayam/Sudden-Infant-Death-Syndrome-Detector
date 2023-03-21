/*

PROJECT CREATED FOR THE PURPOSE OF COMPLETING COURSE ECE-GY 6483 F22 REAL TIME EMBEDDED SYSTEMS

CREATORS: 

Sayam Dhingra     Abhav Vohra     Yami Naik       Ankita Gupta
N16451815         N17434068       N10518065       N15402218
sd5292            av3290          yn2224          ag9135

SENSOR: 

Adafruit Sensirion SHT40 Temperature & Humidity Sensor

DATA: 

// Websites: 
https://sensirion.com/products/catalog/SHT40/
https://www.adafruit.com/product/4885

// DataSheet: 
https://cdn-learn.adafruit.com/assets/assets/000/099/223/original/Sensirion_Humidity_Sensors_SHT4x_Datasheet.pdf?1612388531

DETAILS: 

This sensor accurately and precisely detects realtive humidity % (rh) and temperature. 
We use the detected relative humidity % (rh) to detect human breath on the sensor. 
When we breath on the sensor the rh value goes above 79% and when no breath is detected on the sensor the rh goes below 79%. 
For example when the baby breaths out on the sensor the rh values goes beyond 79% and when it breaths in the rh value goed below 79%
Thus, we can accurately detect the exact number of breaths the baby took at any time frame. 

ALARM SYSTEM: 
We took the assumption (after searching online) that a baby breaths 30 to 60 breaths per minute. 
This is equal to 5 breath per 10 seconds or 10 breaths per 20 seconds. 

// When the baby breaths less than 5 breaths per 10 seconds we send a yellow alert mentioning that low breathing rate is detected. 
Similarly for 20 seconds. 

// When the baby breaths 0 breaths per 10 seconds we send out a red alery mentinoing that there is no breathing detected. 
Similarly for 20 seconds. 

*/

#include "mbed.h"
#include <stdio.h>
#include "LCDSTUFF.h"                                     //Custom defined header file

I2C i2c(PC_9, PA_8);                                      //I2C pins PC_9 --> SDA, PA_8 --> SCL

const int addr8bit = 0x44 << 1;                           // 8 bit I2C address
char temp[6];                                             // Temporary storage of read data
char temp1[6];

int tempC[12],t_ticks,rh_pRH,rh_ticks,t_degC;
int bp_twen_s = 0, bp_ten_s = 0;                          // BREATH COUNTERS
                                                          // bp_twen_s: breaths per twenty seconds 
                                                          // bp_ten_s: breaths per ten seconds

int main()
{
  setup_background_layer();
  setup_foreground_layer();

/*Locally declared variables 

rh_arr is the array which stores the the relative humidity detected in the past 20 seconds
The size is 40 because we are detecting every 0.5 seconds. 

i is a variable to put read data into a temp storage
j is the 20 seconds counter
k is the 10 seconds counter

char cmd[2] is a varialble used to store the command to tell the sensor to detect relative humidity and temperature 

*/

  int rh_arr[40]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int i = 0;
  int j = 0;                                                // 20 second counter
  int k = 0;                                                // 10 second counter

  char cmd[2];  
  while (1) 
  {
      cmd[0] = 0xfd;                                        // Command to tell the sensor to detect temp and humidity
      cmd[1] = 0xfd;

      i2c.write(addr8bit, cmd, 2, true);                    // Here we execute the command to tell the sensor to detect the data
      ThisThread::sleep_for(500);                           // Waiting for 500ms for the sensor to finish detecting
      i2c.read(addr8bit, temp, 6);                          // Reading the data from the sensor and storing temperature and relative humidity in "temp"

      for(i=0;i<6;i++)
        tempC[i] = uint8_t(temp[i]);                        // converting from char to uint and storing in tempC for further calculations

      t_ticks = tempC[0] * 256 + tempC[1];                  // combining the temperature data by shifting the first 8 bits left by 8 positions to make space for the other 8 bits
      rh_ticks = tempC[3] * 256 + tempC[4];                 // doing the same for relative humidity
                                                            // This is an important step because the sensor gives data in the format: 
                                                            // [first 8 bits of temp] + [second 8 bits of temp] + crc 8-bits
                                                            //       tempC[0]                 tempC[1]             tempC[2]
                                                            // same for relative humidity in tempC[3] [4] and [5]

      t_degC = -45 + 175 * t_ticks/65535;                   // Converting temp to celcius 
      rh_pRH = -6 + 125 * rh_ticks/65535;                   // Calculating relavite humidity %

      if (rh_pRH > 100)                                     // max and min statements as per the sensor documentation needs to be set
        rh_pRH = 100;
      if (rh_pRH < 0)
        rh_pRH = 0;

      printf("\nTempC = %d \nHumidity = %d", t_degC, rh_pRH); // Printing in monitor console for debugging

      // In the below commands we are printing the messages on the screen, further details of what the messages are is below

      lcd.DisplayStringAt(0, LINE(4), (uint8_t *)display_buf[2], LEFT_MODE);
      lcd.DisplayStringAt(0, LINE(2), (uint8_t *)display_buf[1], LEFT_MODE);
      lcd.DisplayStringAt(0, LINE(1), (uint8_t *)display_buf[0], LEFT_MODE);

      // converting the array into a queue to have memory of the past 20 seconds data

      for(int z=38;z>=0;z--)
        rh_arr[z+1]=rh_arr[z];
      
      rh_arr[0] = rh_pRH;                                     // Storing the latest data into the first queue position

      printf("\n\n\t\t\t k = %d , j = %d", k, j);             // printing counter values into monitor console for debugging 

      /* 
      10 SECONDS ALARM SYSTEM
      This is the 10 seconds alarm system which will send an alarm if low breath rate or no breathing is detected in the past 10 seconds of data collected. 
      */

      if(k==19 || k == 39)                                             
      {
        for(int q=0;q<19;q++)
        {
          if(rh_arr[q]>79)
            bp_ten_s++;
        }

        if(bp_ten_s < 5 && bp_ten_s > 0)
        {
          printf("\n\n\t\t WARNING !! low breath in 10s \n\n "); 
          snprintf(display_buf[0],40,"WARNING !!");
          snprintf(display_buf[1],40,"low breath in 10s !! ");
          snprintf(display_buf[2],40,"\\(> <)//");

          ALARM_SYSTEM_YELLOW();
        }
        else 
          KALM();

        if(bp_ten_s == 0 )
        {
          printf("\n\n\t\t WARNING !! NO BREATHING IN 10s \n\n ");
          snprintf(display_buf[0],40,"WARNING !!");
          snprintf(display_buf[1],40,"NO BREATHING IN 10s ");
          snprintf(display_buf[2],40,"\\(> <)/");

          ALARM_SYSTEM_RED();
        }

        printf("bp_ten_s = %d, k = %d", bp_ten_s, k);

        bp_ten_s = 0;
      }

      /* 
      20 SECONDS ALARM SYSTEM
      This is the 20 seconds alarm system which will send an alarm if low breath rate or no breathing is detected in the past 20 seconds of data collected. 
      */

      if(j==39)                                                       
      {
        for(int p=0;p<j;p++)
        {
          if(rh_arr[p]>79)
            bp_twen_s++;
        }

        if(bp_twen_s < 10 && bp_twen_s > 0)
        {
          printf("\n\n\t\t ALERT!! low breath in 20s \n\n "); 
          snprintf(display_buf[0],40,"ALERT!!");
          snprintf(display_buf[1],40,"low breath in 20s !!");
          snprintf(display_buf[2],40,"( > 0 < ;;; )");

          ALARM_SYSTEM_YELLOW();
        }
        else
          KALM();
        
        if(bp_twen_s == 0 )
        {
          printf("\n\n\t\t CALL 911 NO BREATHING \n\n ");
          snprintf(display_buf[0],40,"CALL 911");
          snprintf(display_buf[1],40,"NO BREATHING !!");
          snprintf(display_buf[2],40,"( > 0 < ;;; ) ");

          ALARM_SYSTEM_RED();
        }

        bp_twen_s = 0; 
      }

      j++;
      k++; 

      if(j==40)
        j=0;

      if(k==20)
        k=0;

  }
}
