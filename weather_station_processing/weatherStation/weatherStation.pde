// display weather station data

import processing.serial.*; // use processing serial library
Serial port;                // create serial port
String buffer = "";         // serial port buffer
String data = "";           // data received in buffer
int index = 0;
String portName = Serial.list()[1]; // serial port - processing sees it as 1st port
PFont font;                 // font object
int day;                    // to hold day of date
int month;                  // to hold month of date
int year;                   // to hold year of date
int hour;                   // to hold hour of time
int min;                    // to hold minute of time
// Temperature Variables
String f = "";
String fTemp = "";
float t = 0;
float tMx = 0;
float tMn = 200;
String tMax = "";
String tMin = "";

// Humidity Variables
String h = "";
String hTemp = "";
float hh = 0;
float hMx = 0;
float hMn = 200;
String hMax = "";
String hMin = "";

// Pressure Variables
String p = "";
String pTemp = "";
float pp = 0;
float pMx = 0;
float pMn = 200;
String pMax = "";
String pMin = "";

// Dew Point Variables
String d = "";
String dTemp = "";
float dp = 0;
float dMx = 0;
float dMn = 200;
String dMax = "";
String dMin = "";

// date and time variables
String myDate = "";
String myTime = "";

void setup() {
  size(800,400);                            // window size
  port = new Serial(this, portName, 9600);  // port communications
  port.bufferUntil(',');                    // data is comma delmited
  font = loadFont("ArialNarrow-200.vlw");   // font to use
  textFont(font, 48);                       // set font size to 48
  
}

void draw() {
  background(0, 0, 0);
  fill(255,255,255);
  doTime();
  text(myDate, 70, 50);
  text(myTime, 270, 50);
  text("Max", 540, 50);
  text("Min", 665, 50);
  
  fill(192, 0, 0);
  textAlign(RIGHT);
  text("Temperature: ", 250, 100);
  textAlign(LEFT);
  text(f, 270, 100);
  text(tMax, 525, 100);
  text(tMin, 650, 100);

  fill(0, 128, 0);
  textAlign(RIGHT);
  text("Humidity: ", 250, 175);
  textAlign(LEFT);
  text(h, 270, 175);
  text(hMax, 525, 175);
  text(hMin, 650, 175);

  fill(209, 46, 2);
  textAlign(RIGHT);
  text("Dew Point: ", 250, 250);
  textAlign(LEFT);
  text(d, 270, 250);
  text(dMax, 525, 250);
  text(dMin, 650, 250);

  fill(0, 102, 153);
  textAlign(RIGHT);
  text("Pressure: ", 250, 325);
  textAlign(LEFT);
  text(p, 270, 325);
  text(pMax, 525, 325);
  text(pMin, 650, 325);

}

void serialEvent(Serial port) {
  data = port.readStringUntil(',');
  data = data.substring(0, data.length() - 1);
  buffer = data.substring(0,1);
  
if(buffer.equals("F") == true) {
    doTemp();
  } 
  else if(buffer.equals("H") == true) {
    doHum();
  } 
  else if(buffer.equals("P") == true) {
    doPress();
  }
  else if(buffer.equals("D") == true) {
    doDew();
  }
}

void doTime() {
  day = day();
  month = month();
  year = year();
  hour = hour();
  min = minute();
  myDate = String.valueOf(month) + "/" + String.valueOf(day) + "/" + String.valueOf(year);
  myTime = String.valueOf(hour) + ":" + String.valueOf(min);
}
  
void doTemp() {
  f = data.substring(1, data.length()) + "°F";
  fTemp = data.substring(1, data.length());
  t = Float.parseFloat(fTemp);
  tMx = doMax(t,tMx);
  tMax = Float.toString(tMx);
  tMn = doMin(t, tMn);
  tMin = Float.toString(tMn);
}

void doHum() {
  h = data.substring(1, data.length()) + "%";
  hTemp = data.substring(1, data.length());
  hh = Float.parseFloat(hTemp);
  hMx = doMax(hh,hMx);
  hMax = Float.toString(hMx);
  hMn = doMin(hh, hMn);
  hMin = Float.toString(hMn);
}

void doPress() {
  p = data.substring(1, data.length()) + " inHg";
  pTemp = data.substring(1, data.length());
  pp = Float.parseFloat(pTemp);
  pMx = doMax(pp,pMx);
  pMax = Float.toString(pMx);
  pMn = doMin(pp, pMn);
  pMin = Float.toString(pMn);
}

void doDew() {
  d = data.substring(1, data.length()) + "°F";
  dTemp = data.substring(1, data.length());
  dp = Float.parseFloat(dTemp);
  dMx = doMax(dp,dMx);
  dMax = Float.toString(dMx);
  dMn = doMin(dp, dMn);
  dMin = Float.toString(dMn);
}

float doMax(float current, float max) {
  print(current);
  print(" - ");
  println(max);
  if(current > max) {
    max = current;
  } else {
    max = max;
  }
  println(max);
  return max;
}  

float doMin(float current, float min) {
  print(current);
  print(" - ");
  println(min);
  if(current < min) {
    min = current;
  } else {
    min = min;
  }
  println(min);
  return min;
} 
