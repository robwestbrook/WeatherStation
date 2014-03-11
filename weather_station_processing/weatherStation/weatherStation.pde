// display weather station data
import processing.serial.*;
Serial port;
String f = "";
String h = "";
String p = "";
String d = "";
String buffer = "";
String data = "";
int index = 0;
String portName = Serial.list()[1];
PFont font;
int day;
int month;
int year;
int hour;
int min;
String myDate = "";
String myTime = "";

void setup() {
  size(600,400);
  port = new Serial(this, portName, 9600);
  port.bufferUntil(',');
  font = loadFont("ArialNarrow-200.vlw");
  textFont(font, 48);
}

void draw() {
  background(0, 0, 0);
  day = day();
  month = month();
  year = year();
  hour = hour();
  min = minute();
  myDate = String.valueOf(month) + "/" + String.valueOf(day) + "/" + String.valueOf(year);
  myTime = String.valueOf(hour) + ":" + String.valueOf(min);
  fill(255,255,255);
  text(myDate, 70, 50);
  text(myTime, 270, 50);
  fill(192, 0, 0);
  textAlign(RIGHT);
  text("Temperature: ", 250, 100);
  textAlign(LEFT);
  text(f, 270, 100);
  fill(0, 128, 0);
  textAlign(RIGHT);
  text("Humidity: ", 250, 175);
  textAlign(LEFT);
  text(h, 270, 175);
  fill(209, 46, 2);
  textAlign(RIGHT);
  text("Dew Point: ", 250, 250);
  textAlign(LEFT);
  text(d, 270, 250);
  fill(0, 102, 153);
  textAlign(RIGHT);
  text("Pressure: ", 250, 325);
  textAlign(LEFT);
  text(p, 270, 325);
}

void serialEvent(Serial port) {
  data = port.readStringUntil(',');
  data = data.substring(0, data.length() - 1);
  buffer = data.substring(0,1);
  
if(buffer.equals("F") == true) {
    f = data.substring(1, data.length()) + "°F";
  } 
  else if(buffer.equals("H") == true) {
    h = data.substring(1, data.length()) + "%";
  } 
  else if(buffer.equals("P") == true) {
    p = data.substring(1, data.length()) + "inHg";
  }
  else if(buffer.equals("D") == true) {
    d = data.substring(1, data.length()) + "°F";
  }
}

