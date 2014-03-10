/***************************************************/
/***************************************************/
/** Weather Station TX                            **/
/** by Rob Westbrook                              **/
/** Version 1.0                                   **/
/** March 9, 2014                                 **/
/**                                               **/
/** Uses:                                         **/
/**      Arduino UNO R3                           **/
/**      DHT22 Temperature and Humidity Sersor    **/
/**      BMP8085 Barometric Pressure Sensor       **/
/**      433mHz Radio Transmitter                 **/
/***************************************************/
/***************************************************/
#include <Wire.h>              // for I2C bus used by pressure sensor
#include <Adafruit_Sensor.h>   // Adafruit Sensor Library
#include <Adafruit_BMP085_U.h> // Adafruit Pressure Sensor Library
#include "DHT.h"               // DHT22 Sensor Library
#include <VirtualWire.h>       // 433mHz Radio Library
#include <stdio.h>             // for printf functions
#include <LowPower.h>          // Low Power Library

// create pressure sensor object
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

#define DHTPIN 2          // DHT22 pin input
#define DHTTYPE DHT22     // Define type of sensor for DHT library
DHT dht(DHTPIN, DHTTYPE); // create temperature sensor object

char tBuff[10];   // array for temperature data
char hBuff[10];   // array for humidity data
char pBuff[10];   // array for pressure data

void setup(void) 
{
  // define LED indicator light for transmission
  pinMode(13,OUTPUT);

  //transmitter setup
  vw_set_ptt_inverted(true); // must be set as true
  vw_set_tx_pin(12);         // transmit data on pin 12
  vw_setup(2400);            // transmit data speed (in kB)
  
  Serial.begin(9600);        // for debugging
  
  // Initialise the pressure sensor
  if(!bmp.begin())
  {
    // no pressure sensor data detected
    Serial.print("BMP085 not detected!");
    while(1);
  }
}
 
void loop(void) 
{
  
  //read temperature from DHT22
  float t = dht.readTemperature();
  
  // read humidity from DHT22
  float h = dht.readHumidity();
  
  // begin new event for 1085 pressure sensor
  sensors_event_t event;
  bmp.getEvent(&event);
  // read pressure from 1085
  float p = event.pressure;
  
  /******************************************/
  /* use sprintf to convert sensor data     */
  /* from a float variable to a char string */
  /******************************************/
  sprintf(tBuff, "T%d.%2d", int(t), frac(t));
  sprintf(hBuff, "H%d.%2d", int(h), frac(h));
  sprintf(pBuff, "P%d.%2d", int(p), frac(p));
  
  /************************************/
  /* load buffers up for transmission */
  /************************************/
  printbuffer(tBuff);
  printbuffer(hBuff);
  printbuffer(pBuff);

  /*******************/
  /* T R A N S M I T */
  /*******************/

  //turn on transmit LED  
  digitalWrite(13,1);
  Serial.println("\n Transmitting. . . . \t");
  
  // send temperature data as unsigned 8 bit data
  vw_send((uint8_t *)tBuff, strlen(tBuff));
  
  // wait for data to transmit
  vw_wait_tx();
  Serial.print(tBuff);
  Serial.println("C - Temperature sent!");
  
  // turn off transmit LED
  digitalWrite(13,0);
  
  // wait 5 seconds
  delay(5000);
  
  // turn on transmit LED
  digitalWrite(13,1);
  
  // send humidity data as unsigned 8 bit data
  vw_send((uint8_t *)hBuff, strlen(hBuff));
  
  // wait for data to transmit
  vw_wait_tx();
  
  Serial.print(hBuff);
  Serial.println("% - Humidity sent!");
  
  // turn off transmit LED
  digitalWrite(13,0);
  
  // wait 5 seconds
  delay(5000);
  
  // turn on transmit LED
  digitalWrite(13,1);
  
  // send pressure data as 8 bit unsigned data
  vw_send((uint8_t *)pBuff, strlen(pBuff));
  
  // wait for data to transmit
  vw_wait_tx();
  Serial.print(pBuff);
  Serial.println("hPa - Pressure sent! \t");
  // turn off transmit LED
  digitalWrite(13,0);
  
  Serial.println("Complete!");
  Serial.println("Going to sleep. . . ");
  delay(1000);
  
  // do sleep routine to save battery power
  gotoSleep();
}

// this little function will return the first two digits after the decimal
// point of a float as an int to help with sprintf() (won't work for negative values)
// the .005 is there for rounding.
int frac(float num){
  return( ((num + .005) - (int)num) * 100);
}

// this function prints the characters of a c string one at a time
// without any formatting to confuse or hide things
void printbuffer(char *buffer){
  while(*buffer){
    Serial.write(*buffer++);
  }
}

// function for sleep
// max time using low power library is 8 seconds
// using 7 instances of the sleep call
// 7 instances times 8 seconds = 56 seconds
void gotoSleep() {
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF); 
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                SPI_OFF, USART0_OFF, TWI_OFF);
}
