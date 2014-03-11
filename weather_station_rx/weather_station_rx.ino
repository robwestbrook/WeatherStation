/***************************************************/
/***************************************************/
/** Weather Station RX                            **/
/** by Rob Westbrook                              **/
/** Version 1.0                                   **/
/** March 9, 2014                                 **/
/**                                               **/
/** Uses:                                         **/
/**      Arduino UNO R3                           **/
/**      433mHz Radio Transmitter                 **/
/***************************************************/
/***************************************************/
#include <VirtualWire.h>          // 433mHz Receiver Library
#include "LiquidCrystal.h"        // LCD Library

// create LCD library
LiquidCrystal lcd(6,7,8,9,10,11);

// buffers for received data
char rawBuff[10]; // buffer that initially gets received data
char tBuff[10];   // buffer for temperature data
char hBuff[10];   // buffer for humidity data
char dpBuff[10];  // buffer for dew point data
char pBuff[10];   // buffer for pressure data
char first[1];    // holds first element of received array to determine data type

float f;          // holds temperature after conversion to fahrenheit
float h;          // holds humidity after converting from ASCII to float
float p;          // holds pressure after converting from ASCII to float
float dp;         // holds dew point after conversion

int cnt = 1;      // counter for serial tx of dew point

void setup()
{
  // reciever setup
  vw_set_ptt_inverted(true); // must be set to true
  vw_set_rx_pin(12);         // receive data on pin 12
  vw_setup(2400);            // receive data speed (in kB)
  
  pinMode(13, OUTPUT);       // Pin 13 for LED receiving indicator

  Serial.begin(9600);        // for debugging
  
  vw_rx_start();             // start the reciever
  //Serial.println("Radio On...");
  
  lcd.begin(16,2);           // turn on LCD
}
void loop()
{
  
  // unsigned 8 bit integers to store received data and length
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  
  //wait to receive  
  vw_wait_rx();

  // do this when we receive data
  // data is added to buffer
  if (vw_get_message(buf, &buflen))
    {
      //Serial.println("*******************");
      //Serial.println("**** RECEIVING ****");
      //Serial.println("*******************");
      
      //Serial.print("Loop Count = ");
      
      //turn on receiver LED
      digitalWrite(13,1);
      
      // get first element of buffer to determine data type
      // 'T' = temperature
      // 'H' = humidity
      // 'P' = pressure     
      first[0] = buf[0];
      
      //loop through received buffer and populate our buffer
      for(int i; i < buflen; i++)
        {
          rawBuff[i] = buf[i];
        }
    
    // determine which data is received and process
    
    if(first[0] == 'T') {
      // use memcopy to strip first and last character from buffer
      memcpy(tBuff,rawBuff+1,buflen-1);
      // convert ascii array to a float
      float recoveredTemp = atof(tBuff);
      // convert c to f
      f = convertTemp(recoveredTemp);
      //Serial.print("Temperature Received - Temp: ");
      Serial.print("F");
      Serial.print(f);
      Serial.print(",");
      cnt++;
    }
    else if(first[0] == 'H') {
      memcpy(hBuff,rawBuff+1,buflen-1);
      h = atof(hBuff);
      //Serial.print("Humidity Received - Hum: ");
      Serial.print("H");
      Serial.print(h);
      Serial.print(",");
      cnt++;
    }
    else if(first[0] == 'P') {
      memcpy(pBuff,rawBuff+1,buflen-1);
      float recoveredPressure = atof(pBuff);
      //Serial.print("Pressure Received - Press: ");
      // convert from hPa to inHg
      // .14 is for attitude correction
      // based on actual pressure at wunderground
      p = (recoveredPressure * .030) - .14;
      Serial.print("P");
      Serial.print(p);
      Serial.print(",");
    }

    // if we have temperature and humidity
    // and our count is 3 which means temp and hum are done
    // then calculate dew point
    if(f > 0 && h > 0 && cnt == 3) {
      dp = doDewPoint(f, h);
      Serial.print("D");
      Serial.print(dp);
      Serial.print(",");
      cnt = 1; // reset counter to 1
    }
    
    //wait 50mS for work to get done  
    delay(50);
    
    // turn of receiving LED
    digitalWrite(13,0);
    
    // display on LCD
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.setCursor(2, 0);
    lcd.print(f);
    lcd.setCursor(9,0);
    lcd.print("H:");
    lcd.setCursor(11, 0);
    lcd.print(h);
    lcd.setCursor(0, 1);
    lcd.print("D:");
    lcd.setCursor(2, 1);
    lcd.print(dp);
    lcd.setCursor(9,1);
    lcd.print("P:");
    lcd.setCursor(11,1);
    lcd.print(p);
  } else {
    //Serial.println("Nothing!!");
  }
      
  //Serial.println("-----");
  //loops++;
}

// function to convert celcius to fahrenheit
float convertTemp(float tempC) {
  float tempF = (tempC * 1.8) + 32;
  return tempF;
}

float doDewPoint(float t, float h) {
  float c = .56 * (t - 32);
  float dpc = c - ((100 - h) / 5);
  dp = (dpc * 1.8) + 32;
  return dp;
}
