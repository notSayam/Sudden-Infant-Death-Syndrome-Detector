# Read Me

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

NOTE: This project/code is not meant to diagnose or detect any disease and should not be used to do so. 
